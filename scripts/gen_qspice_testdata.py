#!/usr/bin/env python3
#
# SPDX-FileCopyrightText: © 2026 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""
Generate a synthetic QSPICE ``.prot`` test fixture.

This is a deliberately independent encoder for the QSPICE protection scheme
described in SPECIFICATIONS/qspice.md.  It wraps a known plaintext body in a
``.subckt`` … ``.prot`` … ``.unprot`` … ``.ends`` structure so the decryptor
can be regression-tested without redistributing third-party vendor models.

The Mersenne Twister, the keystream walk, and the glyph encoding are
re-implemented here from the specification rather than imported from the
library, so the fixture cross-checks the library implementation.  Only the
shared keystream table constant is reused.

Usage:
    python scripts/gen_qspice_testdata.py
"""

from __future__ import annotations

import textwrap
import zlib
from pathlib import Path

from spice_crypt.qspice._keystream import KEYSTREAM_TABLE, KEYSTREAM_TABLE_LEN

ALPHABET = ",vUxFKnmJwVOl2YQZRrDPH8f0uedITN7zqMcyih3WpEojA1k56Lts&b9XgGaS4CB"
_MASK32 = 0xFFFFFFFF


def _mt_bytes(seed: int, count: int) -> list[int]:
    """MT19937 with geometric seeding (state[k] = seed * 6069**k); low bytes."""
    state = [0] * 624
    # QSPICE substitutes 1 for a zero seed before seeding the MT.
    state[0] = (seed & _MASK32) or 1
    for k in range(1, 624):
        state[k] = (6069 * state[k - 1]) & _MASK32
    index = 624

    def twist() -> None:
        for i in range(624):
            y = (state[i] & 0x80000000) | (state[(i + 1) % 624] & 0x7FFFFFFF)
            val = state[(i + 397) % 624] ^ (y >> 1)
            if y & 1:
                val ^= 0x9908B0DF
            state[i] = val

    out = []
    for _ in range(count):
        if index >= 624:
            twist()
            index = 0
        y = state[index]
        index += 1
        y ^= y >> 11
        y ^= (y << 7) & 0x9D2C5680
        y ^= (y << 15) & 0xEFC60000
        y ^= y >> 18
        out.append(y & 0xFF)
    return out


def _encrypt(plaintext: bytes, seed: int) -> bytes:
    """zlib-compress then XOR with the two seed-derived keystreams."""
    comp = zlib.compress(plaintext, 6)
    mt = _mt_bytes(seed, len(comp))
    stride = (seed >> 3) & 0xFF
    idx = seed % KEYSTREAM_TABLE_LEN
    body = bytearray(len(comp))
    for i, byte in enumerate(comp):
        body[i] = byte ^ mt[i] ^ KEYSTREAM_TABLE[idx]
        idx = (idx + stride) % KEYSTREAM_TABLE_LEN
    return bytes(body)


def _encode(data: bytes) -> str:
    """Encode bytes to glyphs (always row 0; the decoder ignores the row)."""
    glyphs = []
    for byte in data:
        glyphs.append(ALPHABET[byte >> 4])
        glyphs.append(ALPHABET[byte & 0x0F])
    return "\n".join(textwrap.wrap("".join(glyphs), 76))


def make_prot_file(header: str, protected_body: bytes, footer: str, seed: int) -> str:
    """Assemble a full ``.subckt`` file with one ``.prot`` block."""
    data = seed.to_bytes(4, "little") + _encrypt(protected_body, seed)
    return f"{header}\n.prot\n{_encode(data)}\n.unprot\n{footer}\n"


if __name__ == "__main__":
    out_dir = Path(__file__).resolve().parent.parent / "tests" / "data" / "qspice"
    out_dir.mkdir(parents=True, exist_ok=True)

    # Decrypted output (passthrough header + protected body + passthrough footer)
    # equals tests.conftest.PLAINTEXT_BODY.
    fixture = make_prot_file(
        header=".subckt TEST_RES 1 2",
        protected_body=b"R1 1 2 1k\n",
        footer=".ends TEST_RES",
        seed=0x1234ABCD,
    )
    (out_dir / "basic.lib").write_text(fixture)

    # A fixture with leading comment lines and trailing content after the block.
    multi = "* synthetic QSPICE test model\n*\n" + make_prot_file(
        header=".subckt TEST_RES 1 2",
        protected_body=b"R1 1 2 1k\n",
        footer=".ends TEST_RES",
        seed=0x00000007,  # exercises a small seed / zero stride edge case
    )
    (out_dir / "comments.lib").write_text(multi)

    # A fixture that exercises QSPICE keyword tokenization.  The protected body
    # carries high-bit Windows-1252 tokens -- the Ã (gm-block) and Ø (.DLL)
    # device prefixes, the ¥ reserved-pin marker, the « » bus-group delimiters,
    # the ´ separator, and the µ micro sign -- which must survive decryption as
    # their documented characters (with µ normalized to ASCII "u").
    token_body = (
        "ã1 vdd vss out in- in+ ¥ ¥ ¥ ¥ ¥ ¥ ¥ ¥ ¥ ¥ multgmamp gm=650µ ref=.5\n"
        "ø´x1 «in´d out´d» «com» mymodule cout=100p\n"
        "c1 out com 1µ\n"
    ).encode("cp1252")
    tokens = make_prot_file(
        header=".subckt TOKENS vdd vss",
        protected_body=token_body,
        footer=".ends TOKENS",
        seed=0x0BADF00D,
    )
    (out_dir / "tokens.lib").write_text(tokens)

    # A fixture whose *passthrough* (non-encrypted) lines carry a high-bit
    # Windows-1252 character -- the © sign -- so decryption is exercised on a
    # file that is not pure ASCII outside the protected block.  Written as
    # CP1252, matching how QSPICE stores model files on disk.
    passthrough = "* © 2026 Example Corp.  All rights reserved.\n" + make_prot_file(
        header=".subckt CP1252 1 2",
        protected_body=b"R1 1 2 1k\n",
        footer=".ends CP1252",
        seed=0x00C0FFEE,
    )
    (out_dir / "passthrough.lib").write_text(passthrough, encoding="cp1252")

    # A fixture with two protected blocks (distinct seeds) in a single file, to
    # exercise multi-block decryption and the block counter.
    multi = make_prot_file(
        header=".subckt FIRST 1 2",
        protected_body=b"R1 1 2 1k\n",
        footer=".ends FIRST",
        seed=0x11112222,
    ) + make_prot_file(
        header=".subckt SECOND 3 4",
        protected_body=b"C1 3 4 1n\n",
        footer=".ends SECOND",
        seed=0x33334444,
    )
    (out_dir / "multi.lib").write_text(multi)

    print(f"Wrote fixtures to {out_dir}")

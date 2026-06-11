# SPDX-FileCopyrightText: © 2026 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""
QSPICE ``.prot`` cipher implementation.

A protected QSPICE block is recovered in four stages (see
SPECIFICATIONS/qspice.md for the full description):

1. **Decode** — each ciphertext glyph maps to a 4-bit nibble; two glyphs form
   one byte.  The first four decoded bytes are a little-endian 32-bit *seed*;
   the remainder is the encrypted payload.
2. **Keystream** — the seed drives two byte streams: a Mersenne-Twister stream
   (custom seeding, standard tempering) and an additive walk over a fixed
   9973-byte table.
3. **Decrypt** — XOR each payload byte with both keystream bytes.
4. **Inflate** — the result is a zlib (RFC 1950) stream; decompressing it
   yields the plaintext sub-circuit body.

The seed is stored in the clear, so the cipher provides obfuscation only; there
is no user key.  See SPECIFICATIONS/qspice.md Section 7 (Security Assessment).
"""

import zlib

from spice_crypt.qspice._keystream import KEYSTREAM_TABLE, KEYSTREAM_TABLE_LEN

# 64-glyph encoding alphabet defined by the QSPICE protection scheme, laid out
# as four rows of sixteen.  A glyph's nibble value is its index modulo 16; the
# row (index // 16) is chosen at random by the encoder and carries no information.
ALPHABET = ",vUxFKnmJwVOl2YQZRrDPH8f0uedITN7zqMcyih3WpEojA1k56Lts&b9XgGaS4CB"

# Reverse map: glyph -> nibble value (0 to 15).
_NIBBLE = {glyph: index % 16 for index, glyph in enumerate(ALPHABET)}

_MASK32 = 0xFFFFFFFF

# Mersenne Twister (MT19937) parameters.
_N = 624
_M = 397
_MATRIX_A = 0x9908B0DF
_UPPER_MASK = 0x80000000
_LOWER_MASK = 0x7FFFFFFF

# Custom seeding multiplier: state[k] = seed * 6069**k (mod 2**32).
_SEED_MULT = 6069


class _MersenneTwister:
    """MT19937 with QSPICE's geometric seeding; yields one keystream byte per word."""

    __slots__ = ("_index", "_state")

    def __init__(self, seed: int):
        state = [0] * _N
        # QSPICE substitutes 1 for a zero seed before seeding the MT (the
        # binary does ``if (!seed) seed = 1``); the table-walk keystream still
        # uses the raw seed.  See SPECIFICATIONS/qspice.md Section 3.1.
        state[0] = (seed & _MASK32) or 1
        for k in range(1, _N):
            state[k] = (_SEED_MULT * state[k - 1]) & _MASK32
        self._state = state
        self._index = _N  # force a twist before the first output

    def _twist(self) -> None:
        state = self._state
        for i in range(_N):
            y = (state[i] & _UPPER_MASK) | (state[(i + 1) % _N] & _LOWER_MASK)
            next_val = state[(i + _M) % _N] ^ (y >> 1)
            if y & 1:
                next_val ^= _MATRIX_A
            state[i] = next_val
        self._index = 0

    def next_byte(self) -> int:
        """Return the low byte of the next tempered MT19937 output word."""
        if self._index >= _N:
            self._twist()
        y = self._state[self._index]
        self._index += 1
        y ^= y >> 11
        y ^= (y << 7) & 0x9D2C5680
        y ^= (y << 15) & 0xEFC60000
        y ^= y >> 18
        return y & 0xFF


# ---------------------------------------------------------------------------
# Keyword tokenization (Windows-1252)
# ---------------------------------------------------------------------------
#
# QSPICE stores its netlists in the Windows-1252 (CP1252) code page.  The
# "tokens" described in SPECIFICATIONS/qspice.md Section 5 -- the device
# prefixes and operators carried as single bytes with the high bit set -- are
# ordinary CP1252 characters, so decoding the decompressed payload as CP1252
# expands every token to the character QSPICE documents for it:
#
#   byte  char  QSPICE meaning
#   0xC3  Ã     Gm-block device prefix (stored lowercased as 0xE3 'ã')
#   0xD8  Ø     .DLL device prefix, C++/Verilog (stored lowercased as 0xF8 'ø')
#   0xA5  ¥     gate/flip-flop device prefix; also the reserved-pin marker
#   0x80  €     12-bit DAC device prefix
#   0xA3  £     (de)multiplexer / gate-driver device prefix
#   0xD7  ×     saturating-transformer device prefix
#   0xAB  «     node-group open       0xBB  »   node-group close
#   0xB4  ´     separator between a device-type prefix and the instance name
#   0xB5  µ     micro (1e-6) SI prefix on numeric values
#
# Of these, only the micro sign has a standard-SPICE ASCII equivalent: ngspice,
# Xyce, PySpice and PSpice all expect "u".  It is rewritten to "u" so numeric
# values parse in other tools.  The proprietary device prefixes name
# QSPICE-only behavioral devices that have no equivalent elsewhere, so they are
# left as the characters QSPICE itself documents.
QSPICE_CODEPAGE = "cp1252"
_MICRO_SIGN = "µ"


class QSpiceCipher:
    """Decoder/decryptor for a single QSPICE ``.prot`` block."""

    @staticmethod
    def detokenize(data: bytes) -> str:
        """Expand a decompressed QSPICE payload to plaintext netlist text.

        The payload is Windows-1252 text (see the table above); decoding it as
        CP1252 turns every high-bit token byte into the character QSPICE
        documents for it.  The micro sign (``µ``) is additionally rewritten to
        the ASCII ``u`` that other SPICE tools accept.

        See SPECIFICATIONS/qspice.md Section 5.
        """
        return data.decode(QSPICE_CODEPAGE).replace(_MICRO_SIGN, "u")

    @staticmethod
    def decode(text: str) -> bytes:
        """Decode encoded glyphs (whitespace ignored) into raw bytes.

        Each pair of glyphs yields one byte: ``(high_nibble << 4) | low_nibble``.
        Any trailing unpaired glyph is dropped, matching the QSPICE decoder.
        """
        nibbles = [_NIBBLE[c] for c in text if c in _NIBBLE]
        return bytes((nibbles[i] << 4) | nibbles[i + 1] for i in range(0, len(nibbles) - 1, 2))

    @staticmethod
    def xor_decrypt(payload: bytes, seed: int) -> bytes:
        """XOR *payload* with the seed-derived MT and table keystreams."""
        mt = _MersenneTwister(seed)
        stride = (seed >> 3) & 0xFF
        index = seed % KEYSTREAM_TABLE_LEN
        out = bytearray(len(payload))
        for i, byte in enumerate(payload):
            out[i] = byte ^ mt.next_byte() ^ KEYSTREAM_TABLE[index]
            index = (index + stride) % KEYSTREAM_TABLE_LEN
        return bytes(out)

    @classmethod
    def decrypt_block(cls, text: str) -> bytes:
        """Decode, decrypt, and inflate one ``.prot`` block to plaintext bytes.

        Args:
            text: The encoded glyph stream between ``.prot`` and ``.unprot``.

        Returns:
            The decompressed plaintext sub-circuit body.

        Raises:
            ValueError: If the block is too short to hold a seed or the payload
                is not a valid zlib stream.
        """
        data = cls.decode(text)
        if len(data) < 4:
            raise ValueError("QSPICE .prot block too short to contain a seed")
        seed = int.from_bytes(data[:4], "little")
        compressed = cls.xor_decrypt(data[4:], seed)
        try:
            return zlib.decompress(compressed)
        except zlib.error as e:
            raise ValueError(f"QSPICE payload is not a valid zlib stream: {e}") from e

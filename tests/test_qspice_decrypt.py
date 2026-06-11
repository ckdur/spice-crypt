# SPDX-FileCopyrightText: © 2026 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""Integration tests for QSPICE ``.prot`` decryption.

The fixtures in ``tests/data/qspice`` are synthetic: each was produced by
``scripts/gen_qspice_testdata.py``, an independent encoder for the scheme
documented in SPECIFICATIONS/qspice.md, wrapping the shared
``tests.conftest.PLAINTEXT_BODY`` plaintext.  Using an independently written
encoder means a regression in the library decoder cannot be masked by a
matching bug in the fixture generator.
"""

from __future__ import annotations

import io
import zlib
from pathlib import Path

import pytest

from spice_crypt import decrypt_stream
from spice_crypt.qspice.cipher import ALPHABET, QSpiceCipher
from spice_crypt.qspice.decrypt import QSpiceFileParser, _detect_qspice_format
from tests.conftest import PLAINTEXT_BODY, extract_body

DATA_DIR = Path(__file__).parent / "data" / "qspice"


def _decrypt_file(path: Path) -> str:
    """Decrypt a QSPICE file and return the full decrypted text."""
    content, _ = decrypt_stream(str(path))
    return content


# ---------------------------------------------------------------------------
# Decryption of protected blocks
# ---------------------------------------------------------------------------


class TestQSpiceDecryption:
    """Verify decryption of QSPICE ``.prot`` protected files."""

    def test_basic_block(self):
        result = _decrypt_file(DATA_DIR / "basic.lib")
        assert extract_body(result) == PLAINTEXT_BODY

    def test_block_with_comments(self):
        result = _decrypt_file(DATA_DIR / "comments.lib")
        assert extract_body(result) == PLAINTEXT_BODY

    def test_markers_removed(self):
        result = _decrypt_file(DATA_DIR / "basic.lib")
        assert ".prot" not in result.lower()
        assert ".unprot" not in result.lower()

    def test_passthrough_preserved(self):
        result = _decrypt_file(DATA_DIR / "comments.lib")
        assert "* synthetic QSPICE test model" in result

    def test_block_count(self):
        with open(DATA_DIR / "basic.lib") as f:
            parser = QSpiceFileParser(f)
            list(parser.decrypt_stream())
        assert parser.block_count == 1

    def test_multiple_blocks(self):
        # A file with two protected blocks: both bodies are recovered and the
        # surrounding (passthrough) sub-circuit lines are preserved in order.
        result = _decrypt_file(DATA_DIR / "multi.lib")
        assert "R1 1 2 1k" in result
        assert "C1 3 4 1n" in result
        assert ".subckt FIRST 1 2" in result
        assert ".subckt SECOND 3 4" in result
        assert result.index("FIRST") < result.index("SECOND")

    def test_multiple_blocks_count(self):
        with open(DATA_DIR / "multi.lib") as f:
            parser = QSpiceFileParser(f)
            list(parser.decrypt_stream())
        assert parser.block_count == 2

    def test_output_to_file(self, tmp_path):
        # The write-to-disk path encodes the recovered CP1252 text as UTF-8.
        out = tmp_path / "out.lib"
        content, verification = decrypt_stream(str(DATA_DIR / "tokens.lib"), output_file=str(out))
        assert content is None
        assert verification == (1, 0)
        written = out.read_text(encoding="utf-8")
        assert "gm=650u" in written
        assert "¥" in written

    def test_crlf_line_endings(self, tmp_path):
        # Vendor files originate on Windows; CRLF terminators must decrypt too.
        crlf = tmp_path / "crlf.lib"
        crlf.write_bytes((DATA_DIR / "basic.lib").read_text().replace("\n", "\r\n").encode("utf-8"))
        result, _ = decrypt_stream(str(crlf))
        assert extract_body(result) == PLAINTEXT_BODY


# ---------------------------------------------------------------------------
# Keyword tokenization (Windows-1252)
# ---------------------------------------------------------------------------


class TestQSpiceTokenization:
    """Verify that high-bit QSPICE keyword tokens are expanded to text."""

    def test_device_prefixes_and_operators_decoded(self):
        # The Ã/Ø device prefixes, the ¥ reserved-pin marker, the « » bus-group
        # delimiters, and the ´ separator decode to their QSPICE characters.
        result = _decrypt_file(DATA_DIR / "tokens.lib")
        assert "ã1 vdd vss out in- in+ ¥ ¥ ¥ ¥ ¥ ¥ ¥ ¥ ¥ ¥ multgmamp" in result
        assert "ø´x1 «in´d out´d» «com» mymodule cout=100p" in result

    def test_micro_sign_normalized_to_ascii(self):
        # The micro sign is the one token with a standard-SPICE equivalent and
        # is rewritten to ASCII "u" so values parse in other tools.
        result = _decrypt_file(DATA_DIR / "tokens.lib")
        assert "gm=650u" in result
        assert "c1 out com 1u" in result
        assert "µ" not in result

    def test_cp1252_passthrough_preserved(self):
        # High-bit Windows-1252 characters in plaintext (passthrough) lines are
        # decoded as CP1252, not corrupted into the U+FFFD replacement char.
        result = _decrypt_file(DATA_DIR / "passthrough.lib")
        assert "© 2026 Example Corp." in result
        assert "�" not in result


# ---------------------------------------------------------------------------
# Format auto-detection
# ---------------------------------------------------------------------------


class TestAutoDetection:
    """Verify that decrypt_stream auto-detects QSPICE format."""

    @pytest.mark.parametrize("filename", ["basic.lib", "comments.lib"])
    def test_auto_detect(self, filename):
        result = _decrypt_file(DATA_DIR / filename)
        assert "R1 1 2 1k" in result

    def test_detect_true(self):
        with open(DATA_DIR / "basic.lib") as f:
            assert _detect_qspice_format(f) is True
            # Position must be restored so decryption can re-read the stream.
            assert f.tell() == 0

    def test_detect_false_on_plaintext(self):
        assert _detect_qspice_format(io.StringIO(".subckt X 1 2\nR1 1 2 1k\n.ends X\n")) is False


# ---------------------------------------------------------------------------
# QSpiceCipher unit behavior
# ---------------------------------------------------------------------------


class TestQSpiceCipher:
    """Test the QSpiceCipher primitives directly."""

    def test_decode_roundtrip_nibbles(self):
        # ".prot" payload decodes two glyphs per byte; verify a known mapping.
        # ALPHABET[0]=',' -> nibble 0, ALPHABET[17]='R' -> nibble 1.
        assert QSpiceCipher.decode(",,") == b"\x00"
        assert QSpiceCipher.decode("BB") == b"\xff"

    def test_xor_decrypt_is_involutive(self):
        seed = 0xDEADBEEF
        payload = bytes(range(256))
        once = QSpiceCipher.xor_decrypt(payload, seed)
        twice = QSpiceCipher.xor_decrypt(once, seed)
        assert twice == payload

    def test_mt_seed_zero_substitutes_one(self):
        # QSPICE seeds the MT with `state[0] = (seed or 1)` (the binary does
        # `if (!seed) seed = 1`), so a zero seed yields the same MT keystream as
        # a seed of 1.  The table-walk keystream still uses the raw seed.
        from spice_crypt.qspice.cipher import _MersenneTwister

        zero = _MersenneTwister(0)
        one = _MersenneTwister(1)
        assert [zero.next_byte() for _ in range(64)] == [one.next_byte() for _ in range(64)]

    def test_decrypt_block_rejects_short(self):
        with pytest.raises(ValueError, match="too short"):
            QSpiceCipher.decrypt_block(",,")

    def test_decrypt_block_rejects_bad_zlib(self):
        # 4-byte seed + garbage that is not a zlib stream.
        seed = 0x11111111
        garbage = QSpiceCipher.xor_decrypt(b"not zlib data here", seed)
        data = seed.to_bytes(4, "little") + garbage
        glyphs = "".join(ALPHABET[b >> 4] + ALPHABET[b & 0xF] for b in data)
        with pytest.raises(ValueError, match="zlib"):
            QSpiceCipher.decrypt_block(glyphs)

    def test_detokenize_decodes_cp1252_and_normalizes_micro(self):
        # High-bit bytes are Windows-1252; the micro sign (0xB5) becomes "u".
        assert (
            QSpiceCipher.detokenize(b"\xe31 a \xa5 multgmamp gm=650\xb5")
            == "ã1 a ¥ multgmamp gm=650u"
        )
        assert QSpiceCipher.detokenize(b"\xf8\xb4x1 \xabin\xbb \xabcom\xbb") == "ø´x1 «in» «com»"

    def test_detokenize_passes_ascii_through(self):
        assert QSpiceCipher.detokenize(b"R1 1 2 1k\n") == "R1 1 2 1k\n"

    def test_known_seed_recovered(self):
        # The basic fixture was generated with seed 0x1234ABCD; confirm the
        # decoder reads it back from the header bytes.
        text = (DATA_DIR / "basic.lib").read_text()
        block = "".join(line for line in text.splitlines() if line and not line.startswith("."))
        data = QSpiceCipher.decode(block)
        assert int.from_bytes(data[:4], "little") == 0x1234ABCD
        assert zlib.decompress(QSpiceCipher.xor_decrypt(data[4:], 0x1234ABCD)) == b"R1 1 2 1k\n"


# ---------------------------------------------------------------------------
# Robustness: malformed / truncated blocks degrade gracefully
# ---------------------------------------------------------------------------


class TestQSpiceRobustness:
    """A bad block must warn and pass through, not abort the whole file."""

    @staticmethod
    def _run(text: str) -> tuple[str, int]:
        parser = QSpiceFileParser(io.StringIO(text))
        chunks = list(parser.decrypt_stream())
        return b"".join(chunks).decode("utf-8"), parser.block_count

    def test_unterminated_block_decrypted_with_warning(self):
        # A block left open at EOF (no .unprot) is still decrypted best-effort,
        # but a warning flags the truncation.
        text = (DATA_DIR / "basic.lib").read_text()
        head = text[: text.lower().index(".unprot")]
        with pytest.warns(UserWarning, match="not terminated"):
            result, count = self._run(head)
        assert "R1 1 2 1k" in result
        assert count == 1

    def test_malformed_block_passed_through_with_warning(self):
        # A payload that decodes but is not a valid zlib stream must not abort
        # the file: the block is emitted verbatim and surrounding lines survive.
        src = ".subckt X 1 2\n.prot\nvvvvvvvvvvvvvvvv\n.unprot\n.ends X\n"
        with pytest.warns(UserWarning, match="could not be decrypted"):
            result, count = self._run(src)
        assert count == 0
        assert "vvvvvvvvvvvvvvvv" in result  # original payload preserved
        assert ".prot" in result
        assert ".ends X" in result

    def test_empty_block_passed_through_with_warning(self):
        # An empty .prot/.unprot pair is too short to hold a seed; warn and pass
        # the block through rather than raising out of the stream.
        src = ".subckt X 1 2\n.prot\n.unprot\n.ends X\n"
        with pytest.warns(UserWarning, match="could not be decrypted"):
            result, count = self._run(src)
        assert count == 0
        assert ".subckt X 1 2" in result
        assert ".ends X" in result

    def test_one_bad_block_does_not_lose_good_block(self):
        # A malformed block followed by a valid one: the good block is still
        # recovered and counted.
        good = (DATA_DIR / "basic.lib").read_text()
        bad = ".subckt BAD 9 9\n.prot\nvvvvvvvvvvvvvvvv\n.unprot\n.ends BAD\n"
        with pytest.warns(UserWarning, match="could not be decrypted"):
            result, count = self._run(bad + good)
        assert "R1 1 2 1k" in result  # the good block decrypted
        assert "vvvvvvvvvvvvvvvv" in result  # the bad block passed through
        assert count == 1

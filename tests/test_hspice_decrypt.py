# SPDX-FileCopyrightText: © 2026 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""Integration tests for hspice decryption.

Test data was done using the ics55 pdk.  The RANDKEY steps are checked
against :mod:`tests.hspice_reference`, a line-by-line port of the decompiled
functions.
"""

from __future__ import annotations

import io
import random
from pathlib import Path

import pytest

from spice_crypt import decrypt_stream
from spice_crypt.hspice.binary_file import (
    BinaryFileParser,
    access_level,
    derive_sha_key,
    header_value,
    init_key,
    make_sha_rand_key,
    read_access_table,
    tdes_cbc_decrypt,
)
from tests import hspice_reference as ref

DATA_DIR = Path(__file__).parent / "data"
SAMPLE = DATA_DIR / "hspice" / "nhvt.mdl"

# Offset where ReadCipherText starts in the sample: 14 signature + 8 header
# value + 32 checksum + 24 key block + 8 marker + 32 access table.
SAMPLE_TEXT_OFFSET = 118

# sha_rand_key[i] = local_b8[SHA_RAND_KEY_SOURCES[i]], taken from the
# assignments at the end of the decompiled TDESdecipher::InitHead.
SHA_RAND_KEY_SOURCES = [
    *range(32),
    *[24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23],
    *[8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7],
    *[0, 3, 6, 9, 12, 15, 18, 21],
]


@pytest.fixture(scope="module")
def sample_output():
    """Decrypt the sample once: ``(plaintext, verification)``."""
    with open(SAMPLE, "rb") as f:
        gen = BinaryFileParser(f).decrypt_stream()
        chunks = []
        try:
            while True:
                chunks.append(next(gen))
        except StopIteration as stop:
            return b"".join(chunks), stop.value


class TestHspiceDecryption:
    """Verify decryption of Hspice-encrypted files."""

    def test_parse_hspice(self):
        content, (block_count, _) = decrypt_stream(str(SAMPLE))
        assert content.startswith("*\r\n*  release version    : 1.1\r\n")
        assert "synopsys hspice version N-2017.12-SP2" in content
        assert "1.2v core hvt nmos model" in content
        assert ".model" in content.lower()
        assert block_count == (SAMPLE.stat().st_size - SAMPLE_TEXT_OFFSET) // 8

    def test_plaintext_is_text(self, sample_output):
        """No NUL padding or terminator; everything is printable text."""
        plaintext, _ = sample_output
        assert all(32 <= c < 127 or c in (9, 10, 13) for c in plaintext)
        assert plaintext.endswith(b"+ luc1 = 1.739135E-16\r\n")


class TestHspiceFileParser:
    """Test the HspiceFileParser class directly."""

    def test_detect(self):
        with open(SAMPLE, "rb") as f:
            parser = BinaryFileParser(f)
            assert parser.check_signature(f.read(14))
            f.seek(0)
            assert isinstance(parser, BinaryFileParser)

    def test_parser_stream_covers_whole_body(self, sample_output):
        """Every full 8-byte block after the header is decrypted.

        Only the zero padding of the last block (less than a block) is dropped.
        """
        plaintext, (block_count, _) = sample_output
        body = SAMPLE.stat().st_size - SAMPLE_TEXT_OFFSET
        assert block_count == body // 8
        assert block_count * 8 - 8 < len(plaintext) < block_count * 8

    def test_padding_stripped_across_chunk_boundary(self, monkeypatch):
        """The final block's padding is stripped even when it starts a new chunk."""
        import spice_crypt.hspice.binary_file as binary_file

        monkeypatch.setattr(binary_file, "_CHUNK_SIZE", 8)
        with open(SAMPLE, "rb") as f:
            chunks = list(BinaryFileParser(f).decrypt_stream())
        assert all(len(c) == 8 for c in chunks[:-1])
        assert not b"".join(chunks).endswith(b"\x00")

    def test_rejects_bad_checksum(self):
        data = bytearray(SAMPLE.read_bytes())
        data[-1] ^= 0xFF
        with pytest.raises(ValueError, match="checksum"):
            list(BinaryFileParser(io.BytesIO(bytes(data))).decrypt_stream())


class TestInitHead:
    def test_header_value_is_decimal_digits(self):
        assert header_value(SAMPLE.read_bytes()[14:22]) == 20171200
        assert header_value(bytes([1, 2, 3, 4, 5, 6, 7, 8])) == 87654321

    def test_header_value_wraps_like_signed_int(self):
        value = header_value(b"\xff" * 8)
        assert value == ((255 * 11111111) & 0xFFFFFFFF) - (1 << 32)

    def test_sha_rand_key_layout(self):
        sha_key = bytes(range(100, 132))
        assert make_sha_rand_key(sha_key) == bytes(sha_key[i] for i in SHA_RAND_KEY_SOURCES)

    def test_derive_sha_key_hashes_low_nibbles(self):
        """sha256() feeds the low nibble of each byte of 0x100 - (value & 0xff) words."""
        import hashlib

        from spice_crypt.hspice.isaac import RANDSIZ, Isaac

        value = 20171200
        low = value & 0xFF
        generator = Isaac([low] * RANDSIZ)
        for _ in range(low % 10 + 1):
            generator.isaac()
        words = generator.randrsl[: 0x100 - low]
        expected = hashlib.sha256(bytes(b & 0xF for w in words for b in w.to_bytes(4, "little")))
        assert derive_sha_key(value) == expected.digest()

    def test_sha_rand_key_length_checked(self):
        with pytest.raises(ValueError):
            make_sha_rand_key(b"\x00" * 31)


class TestTripleDES:
    def test_init_key_matches_decompiled(self):
        rng = random.Random(10)
        for _ in range(20):
            sha_rand_key, block = rng.randbytes(72), rng.randbytes(24)
            assert init_key(sha_rand_key, io.BytesIO(block)) == ref.init_key(sha_rand_key, block)

    def test_cbc_matches_decompiled_and_drops_partial_block(self):
        rng = random.Random(11)
        keys = [rng.randbytes(8) for _ in range(3)]
        iv, data = rng.randbytes(8), rng.randbytes(8 * 6 + 5)
        got = list(tdes_cbc_decrypt(io.BytesIO(data), *keys, iv))
        assert len(got) == 6
        assert got == ref.tdes_cbc(data, *keys, iv)

    def test_init_key_needs_24_bytes(self):
        with pytest.raises(ValueError):
            init_key(b"\x00" * 72, io.BytesIO(b"\x00" * 23))


class TestReadCipherHead:
    def test_access_level_matches_decompiled(self):
        rng = random.Random(12)
        for _ in range(5000):
            words = [rng.choice([0, 0x88888888, rng.getrandbits(32)]) for _ in range(8)]
            mask = rng.choice(
                [0, 1, rng.getrandbits(8), rng.getrandbits(64), 1 << rng.randrange(64)]
            )
            ok, level = ref.access_level(words, mask)
            got = access_level(words, mask)
            assert (got is not None) == ok
            if ok:
                assert got == level

    def test_flag_one_reads_eight_words(self):
        stream = io.BytesIO(b"\x00" * 7 + b"\x01" + bytes(range(32)) + b"rest")
        words = read_access_table(stream)
        assert words[0] == 0x03020100 and words[7] == 0x1F1E1D1C
        assert stream.read() == b"rest"

    def test_other_flag_reads_one_word(self):
        stream = io.BytesIO(b"\x00" * 7 + b"\x05" + b"\x88" * 4 + b"rest")
        assert read_access_table(stream) == [0x88888888] + [0] * 7
        assert stream.read() == b"rest"

    def test_missing_marker_rewinds(self):
        stream = io.BytesIO(b"\x00\x00\x00\x01\x00\x00\x00\x00rest")
        assert read_access_table(stream) is None
        assert stream.tell() == 0

    def test_sample_access_table(self):
        stream = io.BytesIO(SAMPLE.read_bytes())
        stream.seek(78)
        words = read_access_table(stream)
        assert words == [0x88888888] * 8
        assert stream.tell() == SAMPLE_TEXT_OFFSET
        assert access_level(words, 1) == 0

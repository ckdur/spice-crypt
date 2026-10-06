# SPDX-FileCopyrightText: © 2026 Ckristian Duran. <ckdur.iso@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""
Decryption support for hspice® Binary File format.

This module handles encrypted model files that use the Binary File
format. For now, this module only handles the

The file structure is (for randkey):

    Offset  Size  Field
    ------  ----  -----
    0       13    Prot spec: ``.prot RANDKEY\n``
    13       ...  ???

Decryption of each body byte at index *N* (0-based from offset 28):

where *base* and *step* are derived from the header key fields via
a lookup table, and *sbox* is a fixed 2593-byte substitution table.
"""

import hashlib
import itertools
from collections.abc import Generator

from .des import HspiceDES
from .isaac import RANDSIZ, Isaac

# Decrypted output is yielded in chunks of about this many bytes.
_CHUNK_SIZE = 64 * 1024

SIGNATURES = [b".PROT RANDKEY\n", b".PROT randkey\n"]


def header_value(data: bytes) -> int:
    """Decode the 8 bytes after the signature as InitHead does.

    Each byte is a decimal digit, least significant first::

        value = b0 + 10*b1 + 100*b2 + ... + 10**7 * b7

    (``00 00 02 01 07 01 00 02`` gives 20171200, which looks like a date.)
    The binary computes this in 32-bit arithmetic and compares it as a
    signed ``int``.
    """
    if len(data) != 8:
        raise ValueError("header value must be 8 bytes")
    value = sum(byte * 10**i for i, byte in enumerate(data)) & 0xFFFFFFFF
    return value - (1 << 32) if value & 0x80000000 else value


def derive_sha_key(value: int) -> bytes:
    """The ISAAC + SHA-256 part of InitHead that produces ``local_b8``.

    1. Every word of ``randrsl`` is set to ``value & 0xff``.
    2. ``randinit(1)`` (which runs ``isaac()`` once).
    3. ``isaac()`` runs another ``(value & 0xff) % 10 + 1`` times.
    4. ``sha256(local_b8, 0x100 - (value & 0xff))``, which hashes that many
       ``randrsl`` words, feeding only the low 4 bits of each of a word's
       four bytes (least significant byte first).
    """
    low = value & 0xFF
    generator = Isaac([low] * RANDSIZ)
    for _ in range(low % 10 + 1):
        generator.isaac()
    nibbles = bytes(
        (word >> shift) & 0xF
        for word in generator.randrsl[: 0x100 - low]
        for shift in (0, 8, 16, 24)
    )
    return hashlib.sha256(nibbles).digest()


def make_sha_rand_key(sha_key: bytes) -> bytes:
    """Expand the 32-byte SHA-256 *sha_key* into the 72-byte ``sha_rand_key``.

    Layout (indices into *sha_key*):

        0x00-0x1f  sha_key[0x00:0x20]   (verbatim)
        0x20-0x27  sha_key[0x18:0x20]   (8-byte words in reverse order)
        0x28-0x2f  sha_key[0x10:0x18]
        0x30-0x37  sha_key[0x08:0x10]
        0x38-0x3f  sha_key[0x00:0x08]
        0x40-0x47  sha_key[0x00:0x18:3] (every third byte)
    """
    if len(sha_key) != 0x20:
        raise ValueError(f"sha_key must be 32 bytes, got {len(sha_key)}")
    return (
        sha_key
        + sha_key[0x18:0x20]
        + sha_key[0x10:0x18]
        + sha_key[0x08:0x10]
        + sha_key[0x00:0x08]
        + sha_key[0x00:0x18:3]
    )


def tdes_cbc_decrypt(stream, key1: bytes, key2: bytes, key3: bytes, iv: bytes):
    """Decrypt 8-byte blocks from *stream* with HSPICE's 3DES-EDE in CBC mode.

    This is the loop shared by ``Randkeydecipher::InitKey`` and
    ``TDESdecipher::ReadCipherText``.  For each ciphertext block C::

        P = desdecode(key1, descode(key2, desdecode(key3, C))) XOR IV
        IV = C

    Yields plaintext blocks until *stream* runs out.  Like the binary's
    ``istream::read`` loop, a trailing partial block is dropped.
    """
    # One cipher per key so each keeps its own cached key schedule.
    des1, des2, des3 = HspiceDES(), HspiceDES(), HspiceDES()
    while True:
        ciphertext = stream.read(8)
        if len(ciphertext) != 8:
            return
        block = des3.decrypt_block(ciphertext, key3)
        block = des2.encrypt_block(block, key2)
        block = des1.decrypt_block(block, key1)
        yield bytes(a ^ b for a, b in zip(block, iv, strict=True))  # des::mod2add
        iv = ciphertext


def init_key(sha_rand_key: bytes, stream) -> list[bytes]:
    """Port of ``Randkeydecipher::InitKey(std::istream&)``.

    Decrypts the next 24 bytes of *stream* with 3DES-EDE in CBC mode to
    recover the three 8-byte ``subkey`` values.  Keys and IV come from the
    72-byte *sha_rand_key* (see :func:`make_sha_rand_key`):

        K1 = sha_rand_key[0x40:0x48]   (select2_dest[0x00:0x10])
        K2 = sha_rand_key[0x38:0x40]   (select2_dest[0x10:0x20])
        K3 = sha_rand_key[0x30:0x38]   (select2_dest[0x20:0x30])
        IV = sha_rand_key[0x28:0x30]   (local_48)

    See :func:`tdes_cbc_decrypt` for the block chaining.

    Args:
        sha_rand_key: The 72-byte key from :func:`make_sha_rand_key`.
        stream: Binary stream positioned at the encrypted key block.

    Returns:
        ``[subkey0, subkey1, subkey2]``, three 8-byte keys.
    """
    if len(sha_rand_key) != 72:
        raise ValueError(f"sha_rand_key must be 72 bytes, got {len(sha_rand_key)}")

    blocks = tdes_cbc_decrypt(
        stream,
        key1=sha_rand_key[0x40:0x48],
        key2=sha_rand_key[0x38:0x40],
        key3=sha_rand_key[0x30:0x38],
        iv=sha_rand_key[0x28:0x30],
    )
    subkeys = list(itertools.islice(blocks, 3))
    if len(subkeys) != 3:
        raise ValueError("Unexpected end of file while reading the key block")
    return subkeys


def read_access_table(stream) -> list[int] | None:
    """Port of the part of ``Randkeydecipher::ReadCipherHead`` after InitKey.

    Reads an 8-byte marker: the first 7 bytes must be zero (the binary
    compares only 7 bytes) and byte 7 is a flag.  If the flag is ``1``, eight
    little-endian ``uint32`` words follow, otherwise one.  Together they hold
    up to 64 four-bit access nibbles (word ``n // 8``, bits ``4 * (n % 8)``).

    Returns:
        The words (unread slots are 0, like the zeroed ``local_a8``), or
        ``None`` if the marker is not present.  In that case the stream is
        rewound to where the marker would have started.
    """
    pos = stream.tell()
    marker = stream.read(8)
    if len(marker) != 8 or marker[:7] != b"\x00" * 7:
        stream.seek(pos)
        return None

    count = 8 if marker[7] == 1 else 1
    words = [0] * 8
    for i in range(count):
        data = stream.read(4)
        if len(data) != 4:
            raise ValueError("Unexpected end of file while reading the access table")
        words[i] = int.from_bytes(data, "little")
    return words


def access_level(words: list[int], mask: int) -> int | None:
    """Evaluate the access table the way ``ReadCipherHead`` does.

    Each set bit ``n`` of *mask* (``this->from_param1_assign_1``, presumably
    the feature mask passed to ``Init``) selects nibble ``n``.  The selected
    nibbles are ANDed, starting from ``0xf``.

    Returns:
        The level written to ``*param_4`` (nibble value minus 8), or ``None``
        when ReadCipherHead returns ``false`` (access denied).
    """
    if mask == 0:
        return None
    level = 0xF
    bit = 0
    while mask >> bit:
        if (mask >> bit) & 1:
            level &= words[bit // 8] >> (4 * (bit % 8))
        bit += 1
    if level == 0:
        # A zero result is still accepted (level 0) when only bit 0 is set.
        return 0 if mask == 1 else None
    return level - 8


class BinaryFileParser:
    """Parser for hspice Binary File format encrypted files.

    This module only supports the RANDKEY format:

    1. Calls Init (0xcf8ce0),
      1.1 SHA256 hashes the signature "RANDKEY", and also a 0xA at the end
    2. Calls ParseData (0xcfccb0), which is just:
      2.1 Calls ReadCipherHead
      2.2 Calls ReadCipherText
    """

    def __init__(self, file_obj, access_mask=1):
        """
        Initialize the parser with a binary-mode file object.

        Args:
            file_obj: File-like object opened in binary mode.
            access_mask: Value used for ``this->from_param1_assign_1`` when
                evaluating the access table.  Its real origin is unknown;
                ``1`` (nibble 0 only) is always accepted.
        """
        self.file_obj = file_obj
        self.access_mask = access_mask

    @staticmethod
    def check_signature(data):
        """Return ``True`` if *data* starts with the Binary File signature."""
        return len(data) >= 14 and data[:14] in SIGNATURES

    def decrypt_stream(self) -> Generator[bytes, None, tuple[int, int]]:
        """
        Stream-decrypt the file, yielding decrypted chunks.

        Returns:
            Generator that yields decrypted byte chunks.
            The return value (via ``StopIteration``) is a
            ``(block_count, 0)`` verification tuple, where *block_count* is
            the number of 8-byte blocks decrypted by ReadCipherText.
        """

        # InitHead
        header = self.file_obj.read(14)
        if len(header) < 14:
            raise ValueError("File too short for Binary File header")
        if header[:14] not in SIGNATURES:
            raise ValueError("Invalid Binary File signature")

        digest = hashlib.sha256()
        digest.update(header)
        # digest.update(b"\x0A")

        raw_value = self.file_obj.read(8)
        if len(raw_value) != 8:
            raise ValueError("File too short for Binary File header")
        digest.update(raw_value)
        value = header_value(raw_value)
        # The binary also throws (HEDRStatus 4) when value is greater than the
        # caller's param_2; that limit is not known here.

        if value > 0x131CCC3:
            # A SHA-256 over everything else (header included) follows.
            checksum = self.file_obj.read(0x20)
            pos = self.file_obj.tell()
            digest.update(self.file_obj.read())
            self.file_obj.seek(pos)
            if digest.digest() != checksum:
                raise ValueError("SHA-256 checksum mismatch")

        sha_rand_key = make_sha_rand_key(derive_sha_key(value))

        # InitKey
        subkeys = init_key(sha_rand_key, self.file_obj)

        # ReadCipherHead (the rest): access table
        words = read_access_table(self.file_obj)
        if words is None:
            # The binary then fails unless an earlier header set this->valid,
            # returning (from_param1_assign_1 == 1).
            raise ValueError("Missing access-table marker after the key block")
        level = access_level(words, self.access_mask)
        if level is None:
            raise ValueError(
                f"Access denied by the file's access table (mask {self.access_mask:#x})"
            )

        # ReadCipherText: 3DES-CBC over the rest of the file, keyed by the
        # subkeys recovered in InitKey (select2_dest[0x00/0x10/0x20] are
        # rebuilt from subkey[0/1/2]).
        blocks = tdes_cbc_decrypt(
            self.file_obj,
            key1=subkeys[0],
            key2=subkeys[1],
            key3=subkeys[2],
            iv=sha_rand_key[0x20:0x28],
        )
        chunk = bytearray()
        block_count = 0
        for block in blocks:
            # Flush before appending so the final block is always in the last
            # chunk, where its padding is stripped below.
            if len(chunk) >= _CHUNK_SIZE:
                yield bytes(chunk)
                chunk.clear()
            chunk += block
            block_count += 1

        # The last block is zero-padded, and ReadCipherEnd (Decipher::ReadCipherEnd)
        # only appends a NUL terminator: HSPICE treats the output as a C string.
        # Ending the text at that padding gives the same text without stray NULs.
        chunk = chunk.rstrip(b"\x00")
        if chunk:
            yield bytes(chunk)
        return (block_count, 0)

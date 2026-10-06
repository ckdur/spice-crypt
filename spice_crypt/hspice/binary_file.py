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

import binascii
import struct
import hashlib
from collections.abc import Generator

from spice_crypt._constants import MASK32

SIGNATURES = [b".PROT RANDKEY\n", b".PROT randkey\n"]

class BinaryFileParser:
    """Parser for hspice Binary File format encrypted files.

    This module only supports the RANDKEY format:

    1. Calls Init (0xcf8ce0), 
      1.1 SHA256 hashes the signature "RANDKEY", and also a 0xA at the end
    2. Calls ParseData (0xcfccb0), which is just:
      2.1 Calls ReadCipherHead
      2.2 Calls ReadCipherText
    """

    def __init__(self, file_obj):
        """
        Initialize the parser with a binary-mode file object.

        Args:
            file_obj: File-like object opened in binary mode.
        """
        self.file_obj = file_obj

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
            ``(crc32, rotate_hash)`` verification tuple.
        """
        header = self.file_obj.read(14)
        if len(header) < 14:
            raise ValueError("File too short for Binary File header")
        if header[:14] not in SIGNATURES:
            raise ValueError("Invalid Binary File signature")
        
        digest = hashlib.sha256()
        digest.update(header)
        #digest.update(b"\x0A")

        # This seems to be a checksum
        b = self.file_obj.read(8)
        digest.update(b)
        calc = int(b[7]) + int(b[7]) * 4
        calc = (calc + int(b[6])) * 2
        calc = (calc + int(b[5])) * 10
        calc = (calc + int(b[4])) * 10
        calc = (calc + int(b[3])) * 10
        calc = (calc + int(b[2])) * 10
        calc = (calc + int(b[1])) * 10
        calc = (calc + int(b[0])) * 10

        assert 0x13329fc < calc
        assert 0x131ccc3 < calc

        sum = self.file_obj.read(0x20)
        pos = self.file_obj.tell()
        all = self.file_obj.read()
        self.file_obj.seek(pos)
        digest.update(all)
        dig = digest.digest()
        assert dig == sum, "SHA256 mismatch: {} != {}".format(binascii.hexlify(dig), binascii.hexlify(sum))

        # TODO: Not implemented yet.
        yield None
        return (None, None)

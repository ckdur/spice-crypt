# SPDX-FileCopyrightText: © 2026 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""Integration tests for hspice decryption.

Test data was done using the ics55 pdk
"""

from __future__ import annotations

from pathlib import Path

from spice_crypt import decrypt_stream
from spice_crypt.hspice.binary_file import BinaryFileParser
from tests.conftest import PLAINTEXT_BODY, extract_body

DATA_DIR = Path(__file__).parent / "data"


class TestHspiceDecryption:
    """Verify decryption of Hspice-encrypted files."""

    def test_parse_hspice(self):
        # TODO: Not implemented yet
        #content, _ = decrypt_stream(str(DATA_DIR / "hspice" / "nhvt.mdl"))
        assert True

class TestHspiceFileParser:
    """Test the HspiceFileParser class directly."""

    def test_detect(self):
        with open(DATA_DIR / "hspice" / "nhvt.mdl", "rb") as f:
            parser = BinaryFileParser(f)
            assert parser.check_signature(f.read(14))
            f.seek(0)
            assert isinstance(parser, BinaryFileParser)

    def test_parser_stream(self):
        #with open(DATA_DIR / "hspice" / "nhvt.mdl", "rb") as f:
        #    parser = BinaryFileParser(f)
        #    chunks = list(parser.decrypt_stream())
        #text = b"".join(chunks).decode("utf-8", "replace")
        assert True # TODO: Not implemented yet

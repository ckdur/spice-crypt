# SPDX-FileCopyrightText: © 2026 Ckristian Duran. <ckdur.iso@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""Tests for the HSPICE DES variant (:mod:`spice_crypt.hspice.des`).

:class:`HspiceDES` is checked against :mod:`tests.hspice_reference`, a
line-by-line port of the decompiled ``des::descode`` / ``des::desdecode``.
"""

from __future__ import annotations

import random

import pytest

from spice_crypt.hspice.des import HspiceDES
from tests import hspice_reference as ref

N_CASES = 200


def _cases(seed):
    rng = random.Random(seed)
    return [(rng.randbytes(8), rng.randbytes(8)) for _ in range(N_CASES)]


class TestHspiceDESMatchesDecompiled:
    """HspiceDES gives the same results as the decompiled functions."""

    def test_round_keys(self):
        des = HspiceDES()
        for key, _ in _cases(1):
            des.generate_key_schedule(int.from_bytes(key, "little"))
            expected = [int.from_bytes(k, "little") for k in ref.key_schedule(key)]
            assert des.subkeys == expected

    def test_encrypt_matches_descode(self):
        des = HspiceDES()
        for key, block in _cases(2):
            assert des.encrypt_block(block, key) == ref.descode(block, ref.key_schedule(key))

    def test_decrypt_matches_desdecode(self):
        des = HspiceDES()
        for key, block in _cases(3):
            assert des.decrypt_block(block, key) == ref.desdecode(block, ref.key_schedule(key))


class TestHspiceDESProperties:
    def test_round_trip(self):
        des = HspiceDES()
        for key, block in _cases(4):
            assert des.decrypt_block(des.encrypt_block(block, key), key) == block

    def test_decrypt_is_not_reversed_key_encrypt(self):
        """HSPICE omits DES's final half swap, so decrypt needs its own IP/FP."""
        key, block = _cases(5)[0]
        round_keys = ref.key_schedule(key)
        assert ref.descode(block, round_keys[::-1]) != ref.desdecode(block, round_keys)

    def test_wide_sbox_entry_spills_into_neighbour(self):
        """S-box 7 holds 0x9c; select4 ORs the whole byte into output byte 3."""
        assert HspiceDES.HSPICE_SBOXES[7][37] == 0x9C
        # row 2 (b0=0, b5=1), column 5 (b1..b4 = 1,0,1,0) -> six = 0b101010
        assert HspiceDES._HSPICE_SBOX_LUT[7][0b101010] == 0x9C << 24

    @pytest.mark.parametrize("block, key", [(b"\x00" * 7, b"\x00" * 8), (b"\x00" * 8, b"")])
    def test_rejects_wrong_sizes(self, block, key):
        with pytest.raises(ValueError):
            HspiceDES().decrypt_block(block, key)

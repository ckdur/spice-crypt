# SPDX-FileCopyrightText: © 2026 Ckristian Duran. <ckdur.iso@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""Tests for the pure-Python ISAAC generator (:mod:`spice_crypt.hspice.isaac`).

Expected words were produced by Bob Jenkins' reference ``rand.c`` compiled
with ``RANDSIZL = 8``: every ``randrsl`` word set to *seed* (or no seed),
``randinit``, then *extra* further ``isaac()`` calls.
"""

from __future__ import annotations

import pytest

from spice_crypt.hspice.isaac import _GOLDEN_RATIO, RANDSIZ, Isaac, _mix

# (seed word or None, extra isaac() calls, first 4 words, last word)
REFERENCE_VECTORS = [
    (0, 1, [0xF650E4C8, 0xE448E96D, 0x98DB2FB4, 0xF5FAD54F], 0x7A68710F),
    (192, 3, [0xAB1C6D6E, 0x1FED4134, 0xED149B05, 0x6F4AFE6A], 0xF8BAA81E),
    (None, 0, [0x9FC09148, 0xF989E740, 0x0898E634, 0x6E4D10EF], 0x71D71FD2),
    (7, 9, [0xA60872B8, 0xBEC5918A, 0xEDFC393D, 0x2DB76FFB], 0x8F6CC4ED),
]


@pytest.mark.parametrize("seed, extra, first, last", REFERENCE_VECTORS)
def test_matches_reference_rand_c(seed, extra, first, last):
    generator = Isaac(None if seed is None else [seed] * RANDSIZ)
    for _ in range(extra):
        generator.isaac()
    assert generator.randrsl[:4] == first
    assert generator.randrsl[-1] == last


def test_official_test_vector():
    """First words of ISAAC's published ``randvect.txt`` (zero seed)."""
    generator = Isaac([0] * RANDSIZ)
    generator.isaac()
    assert generator.randrsl[:4] == [0xF650E4C8, 0xE448E96D, 0x98DB2FB4, 0xF5FAD54F]


def test_scrambled_constants_match_hspice_randinit():
    """HSPICE's randinit starts from these pre-mixed golden-ratio constants."""
    state = (_GOLDEN_RATIO,) * 8
    for _ in range(4):
        state = _mix(*state)
    assert state == (
        0x1367DF5A,
        0x95D90059,
        0xC3163E4B,
        0x0F421AD8,
        0xD92A4A78,
        0xA51A3C49,
        0xC4EFEA1B,
        0x30609119,
    )


def test_randrsl_bytes_little_endian():
    generator = Isaac([0] * RANDSIZ)
    raw = generator.randrsl_bytes()
    assert len(raw) == 4 * RANDSIZ
    assert int.from_bytes(raw[:4], "little") == generator.randrsl[0]


def test_seed_length_checked():
    with pytest.raises(ValueError):
        Isaac([0] * 8)

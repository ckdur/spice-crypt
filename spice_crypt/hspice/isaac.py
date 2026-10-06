# SPDX-FileCopyrightText: © 2026 Ckristian Duran. <ckdur.iso@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""
ISAAC random number generator, as used by HSPICE® ``Randkeydecipher``.

A pure-Python port of Bob Jenkins' ISAAC (public domain) with the
reference 256-word state (``RANDSIZL = 8``), matching the ``randinit`` and
``isaac`` functions in the HSPICE binary.  HSPICE keeps the generator in
globals (``randrsl``, ``randmem``, ``rand_a``, ``rand_b``, ``rand_c``);
here they are the attributes of an :class:`Isaac` instance.
"""

from collections.abc import Sequence

from spice_crypt._constants import MASK32

RANDSIZ = 256  # 1 << RANDSIZL

_GOLDEN_RATIO = 0x9E3779B9


def _mix(a, b, c, d, e, f, g, h):
    """Bob Jenkins' ``mix`` macro."""
    a ^= (b << 11) & MASK32
    d = (d + a) & MASK32
    b = (b + c) & MASK32
    b ^= c >> 2
    e = (e + b) & MASK32
    c = (c + d) & MASK32
    c ^= (d << 8) & MASK32
    f = (f + c) & MASK32
    d = (d + e) & MASK32
    d ^= e >> 16
    g = (g + d) & MASK32
    e = (e + f) & MASK32
    e ^= (f << 10) & MASK32
    h = (h + e) & MASK32
    f = (f + g) & MASK32
    f ^= g >> 4
    a = (a + f) & MASK32
    g = (g + h) & MASK32
    g ^= (h << 8) & MASK32
    b = (b + g) & MASK32
    h = (h + a) & MASK32
    h ^= a >> 9
    c = (c + h) & MASK32
    a = (a + b) & MASK32
    return a, b, c, d, e, f, g, h


class Isaac:
    """ISAAC generator state (``randrsl``, ``randmem``, ``rand_a/b/c``)."""

    def __init__(self, seed: Sequence[int] | None = None):
        """Run ``randinit``.

        Args:
            seed: 256 32-bit words to place in ``randrsl`` before
                ``randinit(1)``.  If ``None``, ``randinit(0)`` is used
                (no seed).
        """
        if seed is not None and len(seed) != RANDSIZ:
            raise ValueError(f"seed must have {RANDSIZ} words, got {len(seed)}")
        self.randrsl = [w & MASK32 for w in seed] if seed is not None else [0] * RANDSIZ
        self.randmem = [0] * RANDSIZ
        self.a = self.b = self.c = 0
        self._randinit(seed is not None)

    def _randinit(self, use_seed):
        mem, rsl = self.randmem, self.randrsl
        state = (_GOLDEN_RATIO,) * 8
        for _ in range(4):  # scramble it
            state = _mix(*state)

        # First pass: fold in the seed (if any).  Second pass (seeded only):
        # fold in the first pass so every seed word affects all of randmem.
        for source in (rsl, mem) if use_seed else (None,):
            for i in range(0, RANDSIZ, 8):
                if source is not None:
                    state = tuple((s + source[i + k]) & MASK32 for k, s in enumerate(state))
                state = _mix(*state)
                mem[i : i + 8] = state

        self.isaac()  # fill in the first set of results

    def isaac(self):
        """Generate the next 256 results into :attr:`randrsl`."""
        mem, rsl = self.randmem, self.randrsl
        self.c = (self.c + 1) & MASK32
        a = self.a
        b = (self.b + self.c) & MASK32
        for i in range(RANDSIZ):
            x = mem[i]
            step = i & 3
            if step == 0:
                a ^= (a << 13) & MASK32
            elif step == 1:
                a ^= a >> 6
            elif step == 2:
                a ^= (a << 2) & MASK32
            else:
                a ^= a >> 16
            a = (a + mem[(i + 128) & 0xFF]) & MASK32
            y = (mem[(x >> 2) & 0xFF] + a + b) & MASK32
            mem[i] = y
            b = (mem[(y >> 10) & 0xFF] + x) & MASK32
            rsl[i] = b
        self.a, self.b = a, b

    def randrsl_bytes(self) -> bytes:
        """``randrsl`` as raw bytes (little-endian words, as in memory on x86)."""
        return b"".join(w.to_bytes(4, "little") for w in self.randrsl)

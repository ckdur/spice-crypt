# SPDX-FileCopyrightText: © 2026 Ckristian Duran. <ckdur.iso@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""Line-by-line reference port of HSPICE's decompiled DES and RANDKEY code.

This deliberately mirrors the decompiled functions (byte arrays, the
``cyc_move`` carry chain, ``select4``'s byte packing, the reversed 4-byte
half arrays in ``descode``/``desdecode``) instead of reusing
:class:`spice_crypt._des_base.DESBase`.  The library's :class:`HspiceDES`
folds those details into DESBase tables; comparing the two catches mistakes
in that folding.

Both sides use the same raw tables (taken from :class:`HspiceDES`), so these
tests check the algorithm, not the table values themselves.
"""

from __future__ import annotations

from spice_crypt.hspice.des import HspiceDES

SEL_TABLE1 = bytes(HspiceDES.DES_PC1_TABLE)
SEL_TABLE2 = bytes(HspiceDES.DES_PC2_TABLE)
SEL_TABLE3 = bytes(HspiceDES.HSPICE_SEL_TABLE3)
SEL_TABLE4 = [v for box in HspiceDES.HSPICE_SBOXES for v in box]
INIT_TRAN = bytes(HspiceDES.HSPICE_INIT_TRAN)
UNINIT_TRAN = bytes(HspiceDES.HSPICE_UNINIT_TRAN)
CYC = HspiceDES.ROTATION_TABLE


def select(inp: bytes, table: bytes) -> bytearray:
    """``des::select1/2/3``, ``init_transform``, ``uninit_transform``."""
    out = bytearray(len(table) // 8)
    for i, idx in enumerate(table):
        out[i >> 3] |= ((inp[idx >> 3] >> (idx & 7)) & 1) << (i & 7)
    return out


def cyc_move(p: bytearray, n: int) -> None:
    """``des::cyc_move``."""
    u1, u2, u4, u6 = p[0], p[1], p[2], p[3]
    if n <= 0:
        return
    for _ in range(n):
        u3 = u2 & 1
        u2 = ((u1 & 1) << 7 | u2 >> 1) & 0xFF
        u7 = u6 >> 1 | (u4 & 1) << 3
        u4 = (u3 << 7 | u4 >> 1) & 0xFF
        u1 = ((u6 & 1) << 7 | u1 >> 1) & 0xFF
        u6 = u7
    p[0], p[1], p[2], p[3] = u1, u2, u4, u6


def change1(p: bytearray, n: int) -> None:
    """``des::change1``."""
    c = bytearray([p[0], p[1], p[2], (p[3] & 0xF0) >> 4])
    d = bytearray(
        [
            (p[3] << 4 | p[4] >> 4) & 0xFF,
            (p[4] << 4 | p[5] >> 4) & 0xFF,
            (p[5] << 4 | p[6] >> 4) & 0xFF,
            p[6] & 0xF,
        ]
    )
    cyc_move(c, n)
    cyc_move(d, n)
    p[0], p[1], p[2] = c[0], c[1], c[2]
    p[3] = (d[0] >> 4 | c[3] << 4) & 0xFF
    p[4] = (d[0] << 4 | d[1] >> 4) & 0xFF
    p[5] = (d[1] << 4 | d[2] >> 4) & 0xFF
    p[6] = (d[2] << 4 | (d[3] & 0xF)) & 0xFF


def select3(inp: bytes) -> bytearray:
    """``des::select3``: byte index from sel_table3, bit index from sel_table1."""
    out = bytearray(6)
    for i in range(48):
        bit = (inp[SEL_TABLE3[i] >> 3] >> (SEL_TABLE1[i] & 7)) & 1
        out[i >> 3] |= bit << (i & 7)
    return out


def key_schedule(key: bytes) -> list[bytearray]:
    """One ``select1`` + 16 x (``change1``, ``select2``) block from InitKey."""
    cd = select(key, SEL_TABLE1)
    round_keys = []
    for r in range(16):
        change1(cd, CYC[r])
        round_keys.append(select(cd, SEL_TABLE2))
    return round_keys


def select4(inp: bytes) -> bytearray:
    """``des::select4``: eight 6-bit chunks, bit 0 first, packed by nibble."""
    bits = [(inp[j >> 3] >> (j & 7)) & 1 for j in range(48)]
    out = bytearray(4)
    for i in range(8):
        u = sum(bits[6 * i + k] << k for k in range(6))
        row = ((u & 0x20) >> 4) | (u & 1)
        value = SEL_TABLE4[(row + i * 4) * 16 + ((u >> 1) & 0xF)]
        out[i >> 1] |= ((value << 4) & 0xFF) if i % 2 == 0 else value
    return out


def _f(half: list[int], round_key: bytes) -> bytearray:
    expanded = select3(bytes(half))
    return select4(bytes(a ^ b for a, b in zip(expanded, round_key, strict=True)))


def descode(block: bytes, round_keys: list[bytearray]) -> bytes:
    """``des::descode``: encrypt one block."""
    key = select(block, INIT_TRAN)
    left = [key[3], key[2], key[1], key[0]]  # local_48
    right = [key[7], key[6], key[5], key[4]]  # local_58, first select3 input
    for r in range(16):
        new = [a ^ b for a, b in zip(_f(right, round_keys[r]), left, strict=True)]
        left, right = right, new
    return bytes(select(bytes(left[::-1] + right[::-1]), UNINIT_TRAN))


def desdecode(block: bytes, round_keys: list[bytearray]) -> bytes:
    """``des::desdecode``: decrypt one block (round keys 15 .. 0)."""
    key = select(block, INIT_TRAN)
    x = [key[3], key[2], key[1], key[0]]  # local_48, first select3 input
    y = [key[7], key[6], key[5], key[4]]  # local_58
    for r in range(15, -1, -1):
        new = [a ^ b for a, b in zip(_f(x, round_keys[r]), y, strict=True)]
        y, x = x, new
    return bytes(select(bytes(x[::-1] + y[::-1]), UNINIT_TRAN))


def tdes_cbc(data: bytes, key1: bytes, key2: bytes, key3: bytes, iv: bytes) -> list[bytes]:
    """The InitKey / ReadCipherText loop: desdecode(K3), descode(K2), desdecode(K1), XOR."""
    ks1, ks2, ks3 = key_schedule(key1), key_schedule(key2), key_schedule(key3)
    out = []
    for i in range(0, len(data) - 7, 8):
        ciphertext = data[i : i + 8]
        block = desdecode(descode(desdecode(ciphertext, ks3), ks2), ks1)
        out.append(bytes(a ^ b for a, b in zip(block, iv, strict=True)))
        iv = ciphertext
    return out


def init_key(sha_rand_key: bytes, data: bytes) -> list[bytes]:
    """``Randkeydecipher::InitKey`` over the 24-byte key block *data*."""
    return tdes_cbc(
        data[:24],
        sha_rand_key[0x40:0x48],
        sha_rand_key[0x38:0x40],
        sha_rand_key[0x30:0x38],
        sha_rand_key[0x28:0x30],
    )


def access_level(words: list[int], mask: int) -> tuple[bool, int]:
    """Tail of ``ReadCipherHead``: returns ``(return value, *param_4)``."""
    if mask == 0:
        return False, 0
    level, nibble, word, rest = 0xF, 0, 0, mask
    while True:
        if rest & 1:
            level &= words[word] >> ((nibble * 4) & 0x1F)
        nibble += 1
        rest >>= 1
        if nibble > 7:
            word += 1
            nibble = 0
        if rest == 0:
            break
    if level != 0 or mask != 1:
        if level == 0:
            return False, 0
        return True, level - 8
    return True, 0

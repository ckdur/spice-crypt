# SPDX-FileCopyrightText: © 2026 Ckristian Duran. <ckdur.iso@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

r"""
HSPICE® DES variant implementation.

This module implements the DES block cipher used by HSPICE's
``Randkeydecipher`` (``des::descode`` / ``des::desdecode``).  It keeps the
standard DES skeleton (IP/FP, PC-1/PC-2 key schedule with the standard
rotation counts, Expansion, eight S-boxes, 16 Feistel rounds) but deviates
from FIPS 46-3 in several ways:

* **No P-box.**  The S-box output is XORed into the other half directly.
* **S-box addressing.**  For each 6-bit chunk (bit 0 first), the row is
  ``bit0 | bit5 << 1`` and the column is bits 1-4 with bit 1 as the LSB
  (the reverse of standard DES).  The result is not bit-reversed, and
  even-numbered S-boxes fill the *high* nibble of each output byte.  One
  S-box entry is 0x9c rather than a 4-bit value; its extra bits are ORed into
  the neighbouring nibble, as in the binary.
* **Expansion.**  ``des::select3`` picks the byte from its own table but
  the bit position from the PC-1 table, giving a very non-standard
  expansion.
* **Key rotation.**  ``des::change1`` treats the 7-byte C/D buffer as a
  big-endian 56-bit value and rotates each 28-bit half *right*, while all
  the permutation tables address bits LSB-first within each byte.
* **Byte-reversed halves, no final swap.**  Each 32-bit half is handled as
  a big-endian word, and the final half swap of standard DES is omitted.
  Because of that, decryption is not just encryption with the round keys
  reversed: \`\`desdecode\`\` starts its rounds on the other half, so it needs
  its own IP/FP folding.

Bit numbering follows the binary's ``des::select*`` functions: table entry
``idx`` is bit ``idx & 7`` (LSB = 0) of byte ``idx >> 3``.  That is the
same convention :class:`~spice_crypt._des_base.DESBase` uses for a
little-endian integer, so blocks and keys are converted with
``int.from_bytes(..., "little")``.  The deviations above are folded into
the tables below, and the S-box step overrides :meth:`feistel_function`, so
the shared :class:`DESBase` engine can be reused.
"""

from spice_crypt._constants import MASK32, MASK64
from spice_crypt._des_base import DESBase, _apply_permutation, _build_permutation_lut

_MASK28 = 0xFFFFFFF

# Just for preservation purposes
# originally extracted from the binary

sel_table1 = bytes.fromhex(
    "383028201810080039312921191109013a322a221a120a023b332b233e362f26"
    "1e160e063d352d251d150d053b342c241c140c041b130b03"
)

sel_table2 = bytes.fromhex(
    "0d100a170004021b0e05140916120b0319070f061a130c0128331e242e361d27"
    "322c202f2b30263721342d2931231c1f"
)

sel_table3 = bytes.fromhex(
    "1f00010203040304050607080708090a0b0c0b0c0d0e0f100f100f1011121314"
    "1314151617181718191a1b1c1b1c1d1e"
)

sel_table4 = bytes.fromhex(
    "0e040d01020f0b08030a060c05090007000f07040e020d010a060c0b09050308"
    "04010e080d06020b0f0c0907030a05000f0c080204090107050b030e0a00060d"
    "0f01080e060b03040907020d0c00050a030d04070f02080e0c00010a06090b05"
    "000e070b0a040d0105080c060903020f0d080a01030f04020b06070c00050e09"
    "070d0e030006090a010208050b0c040f0d080b05060f00030407020c010a0e09"
    "0a0609000c0b070d0f01030e05020804030f00060a010d080904050b0c07020e"
    "020c0401070a0b060805030f0d000e090e0b020c04070d0105000f0a03090806"
    "0402010b0a0d07080f090c050603000e0b080c07010e020d060f00090a040503"
    "0a00090e06030f05010d0c070b0402080d0700090304060a0208050e0c0b0f01"
    "0d060409080f03000b01020c050a0e07010a0d0006090807040f0e030b05020c"
    "0c010a0f09020608000d03040e07050b0a0f0402070c090506010d0e000b0308"
    "090e0f0502080c030700040a010d0b060403020c09050f0a0b0e01070600080d"
    "040b020e0f00080d030c0908050a06010d000b070409010a0e03050c020f0806"
    "01040b0d0c03070e0a0f060800050902060b0d0801040a070905000f0e02030c"
    "0d020804060f0b010a09030e05000c07010f0d080a0307040c05060b000e0902"
    "070b0401099c0e0200060a0d0f03050802010e07040a080d0f0c09000305060b"
)

init_tran = bytes.fromhex(
    "39312921191109013b332b231b130b033d352d251d150d053f372f271f170f07"
    "38302820181008003a322a221a120a023c342c241c140c043e362e261e160e06"
)

uninit_tran = bytes.fromhex(
    "27072f0f37173f1f26062e0e36163e1e25052d0d35153d1d24042c0c34143c1c"
    "23032b0b33133b1b22022a0a32123a1a21012909311139192000280830103818"
)

cyc = [
    0x01,
    0x01,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x01,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x01,
]


def _bswap_halves(bit):
    """Map a bit index to its position after reversing the bytes of each 32-bit half."""
    half, rest = divmod(bit, 32)
    return half * 32 + (3 - (rest >> 3)) * 8 + (rest & 7)


def _bswap_swap_halves(bit):
    """Like :func:`_bswap_halves`, but also exchange the two 32-bit halves."""
    return _bswap_halves(bit) ^ 32


# DESBase always starts its rounds on the upper half of the IP output and ends
# with ``(last_left << 32) | last_right``.  HSPICE keeps its halves as
# big-endian words and does no final swap:
#
#   descode:   f() starts on bytes 4-7; output bytes 0-3 = L16, 4-7 = R16.
#   desdecode: f() starts on bytes 0-3; output bytes 0-3 = last computed half.
#
# These helpers fold both differences into the IP and FP tables.


def _select3_table(sel_table3, sel_table1):
    """Effective expansion table of ``des::select3``.

    Output bit *i* reads byte ``sel_table3[i] >> 3`` but bit
    ``sel_table1[i] & 7`` of the input (only the first 48 PC-1 entries are used).
    """
    return [
        (byte_src & ~7) | (bit_src & 7)
        for byte_src, bit_src in zip(sel_table3, sel_table1[: len(sel_table3)], strict=True)
    ]


def _fold_initial_perm(init_tran, bit_map):
    """IP whose output is rearranged by *bit_map* (DESBase bit -> HSPICE bit)."""
    return [init_tran[bit_map(bit)] for bit in range(64)]


def _fold_final_perm(uninit_tran, bit_map):
    """FP that first maps DESBase's final layout back with *bit_map*."""
    return [bit_map(src) for src in uninit_tran]


def _build_sbox_lut(sboxes):
    """Precompute each S-box's contribution to the 32-bit ``select4`` output.

    ``des::select4`` reads eight 6-bit chunks (bit 0 first) and looks up
    ``sbox[i][row * 16 + col]`` with ``row = b0 | b5 << 1`` and
    ``col = b1 | b2 << 1 | b3 << 2 | b4 << 3``.  The value is ORed into output
    byte ``i >> 1``: shifted left by 4 (truncated to 8 bits) for even *i*,
    unshifted for odd *i*.  Values wider than 4 bits (S-box 7 holds 0x9c)
    therefore spill into the neighbouring S-box's nibble, exactly as in the
    binary.

    Returns ``lut[i][six]``: the bits S-box *i* sets in the little-endian
    32-bit output for 6-bit input *six*.
    """
    lut = []
    for i, box in enumerate(sboxes):
        shift = 8 * (i >> 1)
        table = [0] * 64
        for six in range(64):
            row = (six & 1) | ((six >> 5) & 1) << 1
            col = (six >> 1) & 0xF
            value = box[row * 16 + col]
            byte = (value << 4) & 0xFF if i % 2 == 0 else value & 0xFF
            table[six] = byte << shift
        lut.append(table)
    return lut


class HspiceDES(DESBase):
    """HSPICE DES variant (``des::descode`` / ``des::desdecode``).

    Use :meth:`encrypt_block` / :meth:`decrypt_block` with 8-byte ``bytes``;
    :meth:`DESBase.crypt` works on the equivalent little-endian integers.
    """

    # --- Behavioral flags ---
    _SWAP_INPUT = False
    _SWAP_KEY = False
    _ROTATE_RIGHT = True  # See _rotate_halves_right below
    _OUTPUT_MASK = MASK64

    # fmt: off

    # S-boxes (sel_table4), in the binary's own [row * 16 + col] layout.
    # The binary stores them in the order S1, S2, S4, S5, S3, S6, S7, S8 of
    # FIPS 46-3.  S-box 7, row 2, column 5 is 0x9c in the binary (confirmed),
    # not the FIPS value 12; see _build_sbox_lut for how it is applied.
    HSPICE_SBOXES = [
        # S-box 0
        [
            14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
             0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
             4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
            15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
        ],
        # S-box 1
        [
            15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
             3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
             0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
            13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
        ],
        # S-box 2
        [
             7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
            13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
            10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
             3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
        ],
        # S-box 3
        [
             2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
            14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
             4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
            11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
        ],
        # S-box 4
        [
            10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
            13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
            13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
             1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
        ],
        # S-box 5
        [
            12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
            10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
             9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
             4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
        ],
        # S-box 6
        [
             4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  8,  5, 10,  6,  1,
            13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
             1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
             6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
        ],
        # S-box 7
        [
            13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
             1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
             7, 11,  4,  1,  9, 0x9c, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
             2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11,
        ],
    ]

    # Permuted Choice 1 (sel_table1).  Differs from FIPS 46-3 at entries 30
    # (47 vs 46) and 44 (59 vs 60); confirmed by decrypting real files.
    DES_PC1_TABLE = [
        56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
         9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
        62, 54, 47, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
        13,  5, 59, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3,
    ]

    # Permuted Choice 2 (sel_table2) — identical to FIPS 46-3.
    DES_PC2_TABLE = [
        13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
        22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
        40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
        43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31,
    ]

    # sel_table3, as stored in the binary.  des::select3 takes the *byte*
    # index from sel_table3 and the *bit* index from sel_table1 (PC-1), so
    # the effective expansion (DES_EXPANSION_TABLE below) mixes the two.
    # This is confirmed by decrypting real files.
    HSPICE_SEL_TABLE3 = [
        31,  0,  1,  2,  3,  4,  3,  4,  5,  6,  7,  8,
         7,  8,  9, 10, 11, 12, 11, 12, 13, 14, 15, 16,
        15, 16, 15, 16, 17, 18, 19, 20, 19, 20, 21, 22,
        23, 24, 23, 24, 25, 26, 27, 28, 27, 28, 29, 30,
    ]

    # Rotation counts (des::cyc) — identical to FIPS 46-3.
    ROTATION_TABLE = [1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1]

    # init_tran (FIPS 46-3 IP) and uninit_tran (FIPS 46-3 FP), as stored in
    # the binary.  The byte-reversed halves are folded in below.
    HSPICE_INIT_TRAN = [
        57, 49, 41, 33, 25, 17,  9,  1,
        59, 51, 43, 35, 27, 19, 11,  3,
        61, 53, 45, 37, 29, 21, 13,  5,
        63, 55, 47, 39, 31, 23, 15,  7,
        56, 48, 40, 32, 24, 16,  8,  0,
        58, 50, 42, 34, 26, 18, 10,  2,
        60, 52, 44, 36, 28, 20, 12,  4,
        62, 54, 46, 38, 30, 22, 14,  6,
    ]

    HSPICE_UNINIT_TRAN = [
        39,  7, 47, 15, 55, 23, 63, 31,
        38,  6, 46, 14, 54, 22, 62, 30,
        37,  5, 45, 13, 53, 21, 61, 29,
        36,  4, 44, 12, 52, 20, 60, 28,
        35,  3, 43, 11, 51, 19, 59, 27,
        34,  2, 42, 10, 50, 18, 58, 26,
        33,  1, 41,  9, 49, 17, 57, 25,
        32,  0, 40,  8, 48, 16, 56, 24,
    ]

    # fmt: on

    # --- Tables derived for the DESBase engine ---------------------------

    DES_EXPANSION_TABLE = _select3_table(HSPICE_SEL_TABLE3, DES_PC1_TABLE)

    # The S-box step is done by feistel_function below (see _build_sbox_lut),
    # and HSPICE has no P-box, so DESBase's S-box/P-box tables are unused.
    DES_SBOXES = None
    DES_PBOX_TABLE = None
    _HSPICE_SBOX_LUT = _build_sbox_lut(HSPICE_SBOXES)

    # Encryption (des::descode) uses the DESBase IP/FP slots ...
    DES_INITIAL_PERM = _fold_initial_perm(HSPICE_INIT_TRAN, _bswap_halves)
    DES_FINAL_PERM = _fold_final_perm(HSPICE_UNINIT_TRAN, _bswap_swap_halves)

    # ... and decryption (des::desdecode) needs its own pair.
    _DECRYPT_INITIAL_PERM_LUT = _build_permutation_lut(
        _fold_initial_perm(HSPICE_INIT_TRAN, _bswap_swap_halves)
    )
    _DECRYPT_FINAL_PERM_LUT = _build_permutation_lut(
        _fold_final_perm(HSPICE_UNINIT_TRAN, _bswap_halves)
    )

    @staticmethod
    def _rotate_halves_right(value, count):
        """``des::change1``: rotate both 28-bit halves right, big-endian.

        The 7-byte buffer (little-endian *value*) is read as a big-endian
        56-bit integer; C is its upper 28 bits and D the lower 28.
        """
        big = int.from_bytes(value.to_bytes(7, "little"), "big")
        c, d = big >> 28, big & _MASK28
        c = ((c >> count) | (c << (28 - count))) & _MASK28
        d = ((d >> count) | (d << (28 - count))) & _MASK28
        return int.from_bytes(((c << 28) | d).to_bytes(7, "big"), "little")

    def crypt(self, input_block, key, decrypt_mode=False):
        """Encrypt or decrypt a 64-bit block (little-endian integer).

        Same as :meth:`DESBase.crypt`, except that decryption uses its own
        IP/FP tables (see module docstring).
        """
        if self.initialized_key != key:
            self.generate_key_schedule(key)
            self.initialized_key = key

        if decrypt_mode:
            ip_lut, fp_lut = self._DECRYPT_INITIAL_PERM_LUT, self._DECRYPT_FINAL_PERM_LUT
        else:
            ip_lut, fp_lut = self._INITIAL_PERM_LUT, self._FINAL_PERM_LUT

        permuted = _apply_permutation(input_block, ip_lut)
        left_half = permuted & MASK32
        right_half = (permuted >> 32) & MASK32

        subkeys = self.subkeys
        for round_num in range(16):
            key_idx = 15 - round_num if decrypt_mode else round_num
            f_result = self.feistel_function(right_half, subkeys[key_idx])
            left_half, right_half = right_half, f_result ^ left_half

        combined = (left_half << 32) | right_half
        return _apply_permutation(combined, fp_lut) & self._OUTPUT_MASK

    def feistel_function(self, right_half, round_key):
        """F function: Expansion, XOR with the round key, then ``des::select4``.

        There is no P-box.  See :func:`_build_sbox_lut` for the S-box step.
        """
        xor_val = _apply_permutation(right_half, self._EXPANSION_LUT) ^ round_key
        lut = self._HSPICE_SBOX_LUT
        result = 0
        for i in range(8):
            result |= lut[i][(xor_val >> (6 * i)) & 0x3F]
        return result

    # --- bytes interface --------------------------------------------------

    def encrypt_block(self, block: bytes, key: bytes) -> bytes:
        """``des::descode``: encrypt one 8-byte block with an 8-byte key."""
        return self._crypt_bytes(block, key, decrypt_mode=False)

    def decrypt_block(self, block: bytes, key: bytes) -> bytes:
        """``des::desdecode``: decrypt one 8-byte block with an 8-byte key."""
        return self._crypt_bytes(block, key, decrypt_mode=True)

    def _crypt_bytes(self, block, key, decrypt_mode):
        if len(block) != 8 or len(key) != 8:
            raise ValueError("block and key must both be 8 bytes")
        result = self.crypt(
            int.from_bytes(block, "little"),
            int.from_bytes(key, "little"),
            decrypt_mode=decrypt_mode,
        )
        return result.to_bytes(8, "little")

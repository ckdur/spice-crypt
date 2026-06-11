# QSPICE® Encryption Specification

**Version**: 1.0.0 ([changelog](#changelog))\
**Author**: Joe T. Sylve, Ph.D. \<joe.sylve@gmail.com\> \
**Repository**: https://github.com/jtsylve/spice-crypt

This document describes the encryption scheme that QSPICE uses to protect proprietary sub-circuit model files (the `.prot` / `.unprot` "protected" block found in `.qsch`, `.lib`, `.sub`, and similar text files).  The scheme combines a randomized base-16 text encoding, a dual stream cipher keyed by a per-file random seed stored in the clear, DEFLATE compression, and a Windows-1252 keyword tokenization of the netlist.  Each stage is documented so that the protected models can be read by alternative tools.

[SpiceCrypt](https://github.com/jtsylve/spice-crypt) is a reference implementation of this specification, available as a command-line tool and Python library under the GNU Affero General Public License v3.0 or later (AGPL-3.0-or-later).


## Purpose

Many third-party component vendors distribute SPICE simulation models exclusively as QSPICE-encrypted files.  This encryption locks the models to a single proprietary simulator, preventing their use in open-source and alternative tools such as NGSpice, Xyce, PySpice, and others.

This specification is published in service of two goals:

- **Interoperability**: Documenting the encryption scheme allows developers of alternative SPICE simulators to support lawfully obtained encrypted models.  The accompanying [SpiceCrypt](../README.md) reference implementation demonstrates working decryption based on this specification.
- **Encryption research**: The scheme relies on security through obscurity — the key material is derived entirely from a seed stored in the clear alongside the ciphertext, so it provides no meaningful cryptographic protection (see [Section 7](#7-security-assessment)).  Documenting these properties illustrates how proprietary encryption schemes deviate from established standards.

Both activities are specifically permitted by law:

- **United States**: [17 U.S.C. § 1201(f)](https://www.law.cornell.edu/uscode/text/17/1201) permits circumvention of technological protection measures for the purpose of achieving interoperability between independently created programs, and Section 1201(f)(2) explicitly allows distributing the tools developed for this purpose.  [§ 1201(g)](https://www.law.cornell.edu/uscode/text/17/1201) further permits circumvention conducted in good-faith encryption research and allows dissemination of the findings.
- **European Union**: [Article 6 of the Software Directive (2009/24/EC)](https://eur-lex.europa.eu/eli/dir/2009/24/oj) permits decompilation and reverse engineering when indispensable to achieve interoperability with independently created programs.  Article 6(3) provides that this right cannot be overridden by contract.


## License

Copyright © 2026 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>

This document is licensed under the [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/) (CC-BY-4.0).  You are free to share and adapt this material for any purpose, including commercial use, provided appropriate credit is given.

QSPICE is a registered trademark of Qorvo US, Inc.  This specification is an independent work of interoperability research and is not affiliated with, endorsed by, or sponsored by Qorvo.


## Notation

- Byte values are written in hexadecimal with a `0x` prefix; multi-byte integers are little-endian unless stated otherwise.
- `a ^ b` is bitwise XOR, `a >> n` / `a << n` are logical shifts, `a & b` is bitwise AND, and `a % n` is the non-negative remainder.
- All 32-bit arithmetic is performed modulo 2³² (`& 0xFFFFFFFF`).


## 1. Encrypted File Format

A QSPICE protected model is ordinary SPICE text in which one or more sub-circuit bodies are replaced by a *protected block*.  A protected block is delimited by a line containing only `.prot` and a line containing only `.unprot`:

```
* Vendor copyright header
*
.subckt EXAMPLE A B C
.prot
l2VOxFvU,vvx,mUYUVYmQnYQlnlUQlmFYvVYwJvlvn2w
...more encoded lines...
.unprot
.ends EXAMPLE
```

**Delimiters**: The `.prot` and `.unprot` markers are matched against the whitespace-trimmed line, case-insensitively.  Everything between them is the encoded payload.  Lines outside any protected block — including the enclosing `.subckt` / `.ends` and any comments — are plain text and are passed through unchanged.

**Payload**: Between the delimiters, the payload is a stream of glyphs from a fixed 64-character alphabet ([Section 2.1](#21-encoding-alphabet)), wrapped across lines (vendor files use a width of 76 characters).  Whitespace between glyphs is not significant and is ignored when decoding.

**Decryption replaces** each protected block with the recovered plaintext sub-circuit body, yielding a complete, usable netlist.  The `.prot` and `.unprot` markers are not part of the plaintext and are discarded.

The remaining sections describe how the glyph stream is turned back into plaintext.  The procedure has five stages, applied in order: decode ([Section 2](#2-text-decoding)), generate keystreams ([Section 3](#3-keystream-generation)), decrypt ([Section 4](#4-payload-decryption)), decompress ([Section 5.1](#51-decompression)), and detokenize ([Section 5.2](#52-keyword-tokenization)).


## 2. Text Decoding

### 2.1 Encoding Alphabet

The payload is a randomized base-16 encoding: every plaintext byte is written as two glyphs, one per 4-bit nibble (most-significant nibble first).  The 64-glyph alphabet is arranged as four rows of sixteen:

| Row | Glyphs (nibble value 0x0 … 0xF)               |
|-----|-----------------------------------------------|
| 0   | `,` `v` `U` `x` `F` `K` `n` `m` `J` `w` `V` `O` `l` `2` `Y` `Q` |
| 1   | `Z` `R` `r` `D` `P` `H` `8` `f` `0` `u` `e` `d` `I` `T` `N` `7` |
| 2   | `z` `q` `M` `c` `y` `i` `h` `3` `W` `p` `E` `o` `j` `A` `1` `k` |
| 3   | `5` `6` `L` `t` `s` `&` `b` `9` `X` `g` `G` `a` `S` `4` `C` `B` |

As a single string in alphabet order (index `0` … `63`):

```
,vUxFKnmJwVOl2YQZRrDPH8f0uedITN7zqMcyih3WpEojA1k56Lts&b9XgGaS4CB
```

A glyph encodes the nibble value equal to **its index modulo 16**.  Each nibble value therefore has four interchangeable glyphs (one per row); the encoder selects a row pseudo-randomly so that re-encoding the same model produces different-looking output.  The row selection carries no information and is ignored when decoding — only `index % 16` matters.

### 2.2 Nibbles to Bytes

To decode the payload:

1. Discard any character not in the alphabet (this removes line breaks and inter-glyph whitespace).
2. Take the remaining glyphs in pairs.  For a pair `(g_hi, g_lo)`, the decoded byte is `(nibble(g_hi) << 4) | nibble(g_lo)`, where `nibble(g)` is the glyph's index modulo 16.
3. If an odd glyph remains at the end, drop it.

Let the full decoded byte string be `D`.

### 2.3 Seed and Payload Split

The first four bytes of `D` are a 32-bit **seed**, stored little-endian and **not** encrypted:

```
seed = D[0] | (D[1] << 8) | (D[2] << 16) | (D[3] << 24)
```

The remaining bytes, `P = D[4:]`, are the encrypted payload.  The seed is the only key material; both keystreams in [Section 3](#3-keystream-generation) are derived from it.


## 3. Keystream Generation

The payload is decrypted by XOR with the byte-wise sum (XOR) of two independent keystreams, both seeded by `seed`.

### 3.1 Mersenne Twister Keystream

The first keystream is the low byte of each output word of an MT19937 generator (Matsumoto–Nishimura) that uses **non-standard seeding** but the **standard** recurrence and tempering.

**Parameters** (standard MT19937): `n = 624`, `m = 397`, `MATRIX_A = 0x9908B0DF`, `UPPER_MASK = 0x80000000`, `LOWER_MASK = 0x7FFFFFFF`.

**Seeding** (non-standard).  The 624-word state is a geometric sequence in the seed with multiplier `6069`:

```
state[0] = seed                                    (if seed is 0, use 1)
state[k] = (6069 * state[k-1]) & 0xFFFFFFFF        for k = 1 … 623
```

A `seed` of `0` is replaced by `1` for the purpose of seeding `state[0]` only; the table-walk keystream in [Section 3.2](#32-keystream-table-walk) continues to use the raw `seed` (so its first index is `0`).

The generator index is initialized to `n` so that a full regeneration ("twist") runs before the first word is produced.

**Regeneration** (standard twist):

```
for i in 0 … 623:
    y          = (state[i] & UPPER_MASK) | (state[(i+1) % 624] & LOWER_MASK)
    state[i]   = state[(i+397) % 624] ^ (y >> 1)
    if y & 1:
        state[i] ^= MATRIX_A
index = 0
```

**Tempering** (standard).  To produce the next word, take `y = state[index]`, advance `index`, regenerating first if `index >= 624`, then:

```
y ^= y >> 11
y ^= (y << 7)  & 0x9D2C5680
y ^= (y << 15) & 0xEFC60000
y ^= y >> 18
```

The MT keystream byte for payload position `i` (0-based) is the low 8 bits (`y & 0xFF`) of the `i`-th tempered word.

### 3.2 Keystream Table Walk

The second keystream indexes a fixed 9973-byte table `T` ([Appendix A](#appendix-a-keystream-table)).  The walk is a constant-stride sequence modulo the table length (9973 is prime):

```
stride  = (seed >> 3) & 0xFF
index_0 = seed % 9973
index_{i+1} = (index_i + stride) % 9973
```

The table keystream byte for payload position `i` is `T[index_i]`.  When `stride` is `0` (i.e. `(seed >> 3) & 0xFF == 0`), every position uses `T[seed % 9973]`.


## 4. Payload Decryption

Each encrypted payload byte is decrypted by XOR with both keystream bytes for that position:

```
plaintext_compressed[i] = P[i] ^ mt_byte(i) ^ T[index_i]
```

where `mt_byte(i)` is from [Section 3.1](#31-mersenne-twister-keystream) and `T[index_i]` is from [Section 3.2](#32-keystream-table-walk).  XOR is its own inverse, so the same operation encrypts and decrypts.


## 5. Decompression and Keyword Tokenization

### 5.1 Decompression

The decrypted payload is a single zlib stream (RFC 1950: a two-byte header, a raw DEFLATE body per RFC 1951, and a trailing Adler-32 checksum; vendor files begin with the bytes `0x78 0x9C`).  Inflating it yields the QSPICE netlist body — the lines that belong between the enclosing `.subckt` and `.ends`.

### 5.2 Keyword Tokenization

QSPICE stores its netlists in the Windows-1252 (CP1252) code page.  Standard ASCII SPICE content (component lines, `.model` statements, node names, numeric values) is recovered verbatim, but certain device prefixes and operators are carried as single bytes with the high bit set.  These bytes are ordinary CP1252 characters, so decoding the inflated body as Windows-1252 expands every token to the character QSPICE documents for it:

| Byte | Char | QSPICE meaning |
|------|------|----------------|
| `0xC3` | `Ã` | Gm-block device prefix (stored lowercased as `0xE3` `ã`) |
| `0xD8` | `Ø` | .DLL device prefix — C++/Verilog modules (stored lowercased as `0xF8` `ø`) |
| `0xA5` | `¥` | gate/flip-flop device prefix; also the reserved-/unconnected-pin marker |
| `0x80` | `€` | 12-bit DAC device prefix |
| `0xA3` | `£` | (de)multiplexer / gate-driver device prefix |
| `0xD7` | `×` | saturating-transformer device prefix |
| `0xAB` / `0xBB` | `«` / `»` | node-group delimiters (`Ø` and `×` devices) |
| `0xB4` | `´` | separator between a device-type prefix and the instance name |
| `0xB5` | `µ` | micro (1e-6) SI prefix on numeric values |

The device prefixes are written uppercase in QSPICE's documentation but appear lowercased in the payload, because QSPICE lower-cases the netlist before protecting it.  Of these characters, only the micro sign has a standard-SPICE ASCII equivalent: `u`.  Other SPICE tools (NGSpice, Xyce, PySpice, PSpice) expect `u` and will mis-parse `µ`, so a tool targeting interoperability should rewrite `µ` → `u`.  The remaining characters name QSPICE-only behavioral devices and bus syntax with no equivalent in other simulators; they are preserved as the characters QSPICE itself documents.


## 6. Decryption Procedure (Summary)

Given the text between `.prot` and `.unprot`:

1. **Decode** the glyphs to bytes `D` ([Section 2.2](#22-nibbles-to-bytes)).
2. **Split** `seed = D[0:4]` (little-endian) and `P = D[4:]` ([Section 2.3](#23-seed-and-payload-split)).
3. **Generate** the MT keystream ([Section 3.1](#31-mersenne-twister-keystream)) and table-walk keystream ([Section 3.2](#32-keystream-table-walk)) from `seed`.
4. **XOR** each byte of `P` with both keystream bytes to recover the compressed payload ([Section 4](#4-payload-decryption)).
5. **Inflate** the result as a zlib stream to recover the netlist body ([Section 5.1](#51-decompression)).
6. **Detokenize** the body by decoding it as Windows-1252, expanding the high-bit device-prefix and operator tokens (and, for interoperability, rewriting `µ` → `u`) ([Section 5.2](#52-keyword-tokenization)).
7. **Substitute** the plaintext body in place of the protected block, discarding the `.prot` / `.unprot` markers.


## 7. Security Assessment

The QSPICE protected-block scheme is obfuscation, not encryption, and provides no confidentiality against an informed party:

- **No secret key.**  All key material is the 32-bit `seed`, which is stored in the clear as the first four bytes of every protected block.  Anyone holding the file holds the key.
- **Public construction.**  The alphabet, the keystream table, the Mersenne Twister parameters, and the seeding rule are fixed constants of the scheme, identical across all files; only the seed varies.  Recovering them once (as in this document) is sufficient to decrypt every protected block ever produced.
- **Keystream reuse.**  The 32-bit seed space is small, and the table walk repeats with period at most 9973.  Two blocks sharing a seed share both keystreams, exposing the XOR of their plaintexts.
- **Integrity.**  The format carries no authentication or signature over the protected block; the only consistency check is that the inner payload must inflate as a valid zlib stream.

These properties are inherent to storing the key alongside the ciphertext and are documented here for interoperability and research purposes, consistent with the legal provisions cited under [Purpose](#purpose).


## Appendix A: Keystream Table

The 9973-byte keystream table `T` from [Section 3.2](#32-keystream-table-walk), encoded as base64.  Decoding yields exactly 9973 bytes; verify with:

```
SHA-256 = bfd3ff9339f28056922c167e92daffbb10668e993aef4ac7ff3dd6d662004df3
```

```
daSZRhncoigA7m5ui+t7gFs3g439XtsskDXi/TO+/kETAsxbXq+kS10fuQ/RXfB/3G5jMXFwrfvJby7VFDz0yulbS3zlCbYQ
5bofyI2+BVesMYKuqFICNzipKluj99LfQcpFQypp25H8M4mjI+FD/8rxvk1Bsoqe2DHj7YzZ2ZM2AVAdekaDk/8+E5Wsb3Da
2yScCfqQkH7cyVqG1rIrxR+nTT0BfAZL+I4j+JA7IASQ9liBix2KCkUEVHsvn45oCHHvm7a1ASNNmWN10BhBEaVVeW45f6W9
/ZWf/gtQNV4J/HwvHY40HBwA5dtl6NoFpFndQ01kdfIkzMF5biF/MTjUW/42PGqSSDlAs2DuFO/2xEM7AWB3tUTWkA+M1h8i
6yyC6NF8utqFIfMOurKbUN4LEVmaO1Zvtz/nw9VSAtgM19FmoPFvZLKPRRnywGyBteoVivo9u9b5jjzmZQ4dQZKRTISajEB1
355bDVn3Jm9U92b9l8krDwPVt7gBL8t5/FavXmliRsag4s8n5hfxUxuvjIxSe1rWLOVbSAR6obGhl/qDGTBYbIumLwxlbIoU
mcLgYxCkuqmQtBTnwIooYb9mbbM3CVcedkgJZRDiNWIpmX02hpPQ44eL0Yys4ienCd7I3PX5VedMWy74i+plH83ea9iyfeid
rFt63jt6AEQMceNDmnR6XvbcHs3HH+tuuEvdiF3UMqq8GLbfLDO5RJuHtBd77IcBVznoUGjcJV7ma1bIZjZCPCmP3wNIRVzt
aZ9UAqpC7m+mDChzJ7zUlCKVXoInJUKAmdWxdxraYlLR8/wCVpbpMuHHl3thROzs6y6JNSFyDe+TodFMvSLg7SbOwpwU8PF3
bBb1Kk88wj0lN6psu93mTU5fgMxtWRpb7oGC3G7TtGhXlwy0CZKdkEpsFIBKT6Bf1DwxZEOOwfqgRcn0Bt9VFkvKBqG7syF+
HHa/vchRXYlMk2hvFCc/QbldVUIhJpAe5TV3ZsutrO1YCtS55WlqTWHUQPGg9MYvpCQJwR1psPdNTxesp4Ij0JV9Pzq1QugT
yPlYMeSDCEEzpWmZPkNopdOXNLxXMPmajBvZFTG4ppA/oRtlYdh1K+z69V+XPJw0D4Idp2EF/KxR1UL476QB3jmFD2+LhiYP
oIopy7jUS9GrfO24Ho6+KYaHMv/L5K5TIrgbBdQYbpPqgSytRPk7sywfPOv6iUxn4BJTeh9XZdkTrTKHZ7vW0ApXy7XqJRXZ
6SZh/INGU79JVMNUcnO2l1GYj1xcNVyrffeV5aWnyfRyniobbDalFJkDiUuEaXpavuH+ZnoHlpGShrkDr7bINkNd8RmHsfCj
Ex81tpoy2SINXP1JCoieHC24b9o+9a5j8sGUI/cpSKH5+fLpLgffpEDgIW1q2Rdy0oqYS50MKJNoyxpGPuWLK9pZBR5POuyf
/6T9lVVcVLAFv1BGD/Sj8e2HnIZ7kT1qtmVoD/jIXVKk/0f3lVx+ztDA4xc5G6PFKS+wz2Ecb+PU3jcw1eAw4wS/mbLxuMp7
macoZeo3saU2q1Ve4qOaIxxTSoWWuff0P+f62watUdPLmyrrRuvxuCCNi/cAivDYtTKJ21qw4oehiFJEiGSFCUPhUOghPLpo
iH9JLiu+f/8m2Xc8hqA119oopRrrFe/VRW+U4X6jaMt+MI99YYMqRPif4STgLd4afWjny55eJH5GZsMmHs+gTu1WOr3yvWEy
1yEgsXouYWmKRM9m9apUgQmTqT+amL7QZMti04PYw4J7DXJeblsyMb6Nhvw8BOqNW/I0fk+zaqsvl2/D96hDraXtbfsEMUAd
LovkWwLCuEYqzAfGGCmF8fTh8XVCjOas9u4cNeqyb772E9j18i7CQMRcZzUERy4vztIHq9MlgvL5m980qwCbNjf4k6sk9GeG
Mgt7XM1QAjkad76oc+X4rimUdhTGCV3YLqhmhWrWveRVX2GywqxBtPprg/+GZn38P72CzVrwkyVFuldcKMDZ46QeXY09lClj
DU6bmlweP67F5RMj0dZrZB/Z+Rw95F2u908WSKJ/mj5D24pn0xhLgujIisFtajDSmGYlo+hk9qnQ1A8mZXMURT5limOTSL+b
sERsFMDHqT6rOWQKIuSe88xveWviB5H9qodPd82hknnVWbewvrWJN+4+SWmJixW6TcW3i3HJia2+jXsP11LfFPiqpea05CnY
8irJ5k37RWuKPunT10U/OVEMt7REdKhxafyGttlUQxvwMaPM7JSELYFZkMhsnHpaoJlsyz7z5oGt0MVHgZy/36bQc5i3dXIn
Q9LpAx7zydpLpCx0a9rWJHc0t5TQ5EelyDm8/G2r4+9MOED4VseF0LOA8qtJbpWaOUMC9JFc3s0SLC/tq17skk4mgzSu1k8j
2srfZ4r7wI+fPH0gknB2V9zel/ertJMF8TatZfKGRPejAT71o2MYCo8laqzrPxEvnOXPAbjl0+slakftxbg1Ana/qss5KbP/
X0v/uXz6b7XfA+LpBo60ABKGe7q5NkSRK88LXx5EB7E2TzCCNzIvtY0zERnz4QFqw9MMB6shsx2Aj1musJMsyhXVqWIjh5jh
W2dqCIgbA1+OyEcMz7OgJzJjBy8UaLHUnTN+XCoTNUBc5xRhcASnNhU0fxjZaNZ5o+JzRPCB7M9USRO3ZkjGSrC55XISzGEz
JsIJPlwflcQvCyUtZWsS6fzn+bZvl3Tw1Wd1IGF/yDTa1z4cYgm4F6RZxReFn9ZrWhAMfXfa1X7DhKroVOgoNAiL2i4x8+lp
+4YFKjslEb9jgVF1bsonkfuxFAx9Gp5wQblQPoVzQjDM4CvBFKva6Z8MHSd8QV6iWQM99ZpA/+9t4ot/C+iPB/GrUkw/pjfz
8AKTjGFQm3OxsF5JJ4BuNQgu/RDfMY4ZMaU8glvb1RTT0d689ksQo7J+H1i4bOSh1K9F4ha771tVae3C0Uy6GIRkb7OfA9is
/FAwBzvuEGTYfwW1RR4M1dmW2Ixd6zKMEtX2U1hqoEd++gxglTd0Ve9q3oljAEuzFXdant6xV1CW6DDvB86VxMuSWnqwG5LT
TgO6CK0pzjFmnCadEhTn64p4vbeGLoMc/KHMZY7W6t+j2WqJSlUPya5EaPKs0mbGX3AvVsO6Qd5TlxGUzhisc7v29YANCtQd
YzQER2viKcRYQCu3mVvQAjDWx4GGUmQSMrda/l62QZdozBx4hfzniXryihE2eCHuD7ElN3zihqpprCfO9QWBY8tLPsNAcCqP
wjFoBm1yn2iLTO8XO6uLkGO248ETVheGvGVx0mwDWEBz3op7Vl0UKUy0MaIGnEsf6lTFCVQeg1uX04X3iBBM9akr+GabLxe3
a0i3DAjEKJfvpj3RljsnXAPaN6JLgJDYuSWSMnJZ8XK0M+QL6vRk2RV+Ds8Hrxb+SWtDla+U9o2Jljc2MgYeGXYHCaImAgO2
dJK85VtWwJZB+v7rCRJDSzwT3u8vujHpvSRAQrJGBzs9opXYAuh/7i40fmHfKyNZtXk8XSuqBIolrZElMTj7lv0vs+DE0DBg
rIXjsHIh3cJQGkI5/XUaBbtNebdvr8YhXs6slUT29fcMRZ3GDENACNrGFMvt52YrxuyHpKsKS34abdDZV1jm9dPNDJRwNRHG
HeaODipuXonE2yU/tDINQn9b2AXWnzaSbsOPR+2pFUB61Ry/Fx9i4knSd0Vmh2F6OJQVsZLtf8BgApgOsWmsTB3cDLZrfP1O
zXLR6Pe3/E8o8WhKg229kVidSDr5EIoajjNu1VFmCWxitYlWZxANKe6zcZ90WjncgwE1KZdGUh/l+nE9IWkGvI2F++MXTBxU
36z6p/RDxPNi7JSP4oJfU0BANFcJWp+yfW0OgoFudG7/x5t5DDGP2M7zWYq8fZNN3ysjT7cbLeTTowVPCuofwd+VD6DMNnfu
nHrX6ahqmaZImwe/dnOlKA+XOQ2nndo49Hg5R3qseGflKvwD2WL/LZmR4+9Ixwz80cHW9Vy7BDPkih/kt1RwtNozxdSOmc7t
adUNqVad135ENcrmbtX7oGmqLdoQp8PTnqFapuF9ZtMJsC5EuI33z1KiilsxLzX5G40JZFzClpO7UmxXPPu8i6Xsp2P5uQAF
dQ/CAmfGPELJ3wvLeM3L7Zb9sRyR1wGIwBZ8gDsYDuAaMQJs2l7r+Yi15NlwnlbyPE/DaRATpvcZE0xWhnEDAlvbfnidgUsE
I4Sn8e89jAqLd3LhpS554WljJ9OdZZMc/MeGSCsT6VO598yKFTcnapl1Bf9ilt99xElAE/5lByo3f2l9Uks9QC7SOtQE8eJ1
i+MJeYuEm48YRmcszWZdv7bTWFYkEIUr4aGcSHEVEusZqU3PRMc6xF1AHsmJGUp3jHlcZiGa9OM63cDyKSLebNyq8grw286w
aHrONCcpm6QazhH8C8HTisHb7zUp1X/KgE8ReR6x/+8l39nazHHi3nQeYrToWDQaFKdRGf+xT6RFI3WvMSCSHdF61JF3YZHS
6DLEQI8Yd0JscqFtGHcgBMvKGfnsy96UeycYA0N1ITwsIDkzp8wqYDg44qxFvgztfvUyAvr5GHd+fIw2Tvfjw34fMcCuvcgz
fJopO/cHdztOxs8y2FA28BavyFwK4mKrLEMNl/cp5S8ZxUwMjJe3eHT+XzAIuF8XlH5WQib5MSmck51H3Ykn8+ALwctwRikU
jvPLN2HrVB5gs8Hy3LLUPrfL++z7alWe+smBTZBCWoeWhP7h7rGHchuxFLrMXUZcCIIk5mT2XAH1LJgrblccfhDQl+Uql939
evUWnV0HBCM0pyE/iKVHXXbaHcmdxfNrGYbSm250TaD8V/MokIE95681qB6gaSGPUn4ASvWS72QWPyZjesv5HkavuTSUt0Xa
3OHyVTlOxNVIs7KmHvf5A2UF47xCQhxif/d1wFXTdfRjthcg/FqNdHTbfYQNJ7TC0GFl+6c+uodb+S+r6NbgLobyIe93nUwx
1nEMLn2DH9qj/Oeb4JjJhagisqUGkdJRHucxJ61KiRW7SJmA4dgu4Kl9n9VH4DvNKMOsfE/dClXrsLxvl5Ii8K79O8U3DX2T
5mvQku95+OX1IKQzt4mwq21qtTKfJ9bVYm2H3DUgMAXdCQNfsTBdkSDz0+4klpnvpzuPevOGYjoYUQBZAEsNfp5+hMqC3ZuY
6KjBMSh0Cfh1YSBa0nIvRm7hqJExwbT6as6YC0/htbG+2BE5q5bvEKz8/pYQ5vIsuy3tLAI1D4BZrvRHRDdjkzlfSlQINSgc
JLkR2rUAbx2Jpo8k4r+Jg2NF99kbzBXXxO48TArEJkIJoT7HDvLQBTiRcF7i3p/rv8z/YMUcSnmKi2xkC3HPS0YTEoX4ORSB
OMB/4vesoC8ppcdjeDo0f0K2f4mSQDHR9uIBg7tJ8g5iRHUB/pFfvAJpKPKAw75bLpF52gLr8mGskililjC2hubreZRg5QND
4XFXwTwV+XbJrFTatfSQy3Y0HVfLGafsyFRunqwx9TtestgPcpkrPkMUkQ8ySWPQlaBbsduks2RvgP97H9HH9LvnBcyfUTyh
5B/IbuA19XtQjxwD+Djl4n0cfO+C14LqzkNEuboXT8RyallfXW0D2yVrNyJ6yX70SeuNnNpleiGnON0cYhpKrP/zPqqiaBgY
eN0xK5/jCkBG/uZli+CItM9uAHmjvXrioORO2wA1fjXwg1iOrjSOfroSF6sZyurjCoke5KXtSxFDw+4IXVRFcSD5/b0ahLgC
TMNkCWA4T/l6GD7OdZkLd47AqFFD3o6URx5sQeICipe/TlzBwBQNz3eUlrgEtCpFlfJqPVTNQOtcsqNJYZXicsHU2GbS1WKj
lI0p1GqzjhbfuiBnQBN/+75VedTkBZuKdWX81Txyf244dPzJgSeFpqApa2dehdxzRu4ZRIxxhUnianptBjI4ukziOcjVzpwT
YN5rJZ6p3IOF8q0PQ0xvn2hYxtYL7qlAVXFM/yNFFaeCWAXQVZWANByNiwYt2WMZvzm4GEvt0SSjPSaoci5BInh4JRLSVQ4B
zXWNoVimFP8BlIV9Ig4XawS0yy8JSAgVEj9kroL/g5diespKZtSaVQF/lyH/FU/hy+9ed6dF/NeFD6oDJEt1WQs8uuVSZx8D
VOxdPendT8fyz7Nbh7uBKAJl9sCMCQUQ88+3VSVlmNijAhI7wQYh6Dlj4+sgWJOPMwSCL36yvwdZxi8UFQRiEqsDqNmbAXft
I3KIAWmuK6D6muq8UqvkcLJBGk+SgnjfVGpRlGr9kXgVK7XFXESiOUTJSUXXQFzCgQ3bxQeQcB11M2tJRNjK5D3wHMbVq2I2
7pXczbBGTavvIpVGBJlxcQ2DFmNhBw8fuRXeSseWv4OpEHeDDdTRYOlZ9iXM9HkDkFQGDY0bYiwoKsrK+hWLvg1MGppWVORt
jHCH7SRS3mGDZQO1hB5Sp6D+2v7P4qlhN4/U4+k5tNkbrDhfYcY7YBktt2BoiBnsolaU/hs930L0c6/TqADWXRc/Own2lxSz
dxzOTPbU7odIVxjK7b3nfjgObm5TGKykAPU7041W4L1MWmD+EK/7wRqBK9dN8EsnOo2Qsxxurqxxqo6s30dYYYgixDo/yu0M
UEzYOM8O/qy7qnukk8nj5bMeBL2yvG1+5esBRYtiqErh+dQkWugkOiJXTunOAduxmT480XgDnlQVk6ZzNCZcO7HWnVI/Jl4W
gU6zz5gZA56finqdjBxIDD1f512ewLYbUY0D/DGB4TdxO0wVpit7LsXhnzNOgKGua2I1Fhc8R+avjfqcpSsuUAkl2hyPN4xa
63LcfWwXcGnWpS047oAwg7klTzF7VBYA9eDNpLNu4y102YLJmu3bpRp3hSg7Q6srOL8TXeJGRoNwof2D67Sbk7+3Kqop5SLu
eeNEmHeNj00us4o3dB643Gv3sw6CUTZfPTsIb18u8bF5ZwPVSHHLtMwwwl1WhNjK1x7L774twAEG8NqgQLPXlGGmmZjfnhN5
zEtTeNK/74WGtdo7STG4mEDDJtkPQKPVylGQbR+YsBNHpzHzD55H44+cUBqMWU77LGOyqI/uznzRifzyQUbCFjw+iQQLN5wU
hzv2C7dewUdZRt29dZ7sv2mntbG9BMhibFZ04NvhCpwwfOVpf2eVfkDnsP4g82hlQJQzPeaI1x/SOdXicnVnLzUyup8ePaH4
Al3JFaU8wNZ6DRzIoy2XK1N7aConh7Df6LvPj2SI7apT7q4Ywm9P5tIEI+M4qoOiZw8Y7PXbZb8NdERy8KSjNGvcaBC8wvT+
xx1njh31TOPwsjFqlxb0gYbwWYhReYqycAMrQpFVULJG4p57wnsAeVNlYfCgNuAZ9VsycOTpmfZ1XpaQMxR7uht8J18/+hMM
wShjqSDh79U3LRW02NTZ9w9/noR93MnO8cePhpxkb+02WXkW/6/vwjUEiDfaQhR1gJQkE5B5cx04PL9JkqP6ULPrcm572y9g
XYjgC7mBiqEZ3UjbMyTC9RxlXUZ+vir/7Byv9gsQpTYBYW5wqIc72smUi2Kn/3INrccOPx7tPmy3Lp3AxtlCb6HgH+E86A2T
9jGdloXUOnCYEH81nk8Og9PAM7Fhcds62CXEnZfs2lMm5FbO4Yo5Y0bMnGLC3aOWa5QtlZq9fSAV8aJmpB5nV+ySMY1l8J+Z
RZauR+FzFNXBZmKd5VIVkm6kTuT74OyCjB1rPt9AfpubY+c6L2oHWIovgbKvPlgWq9lL6dn+4oQZjDkce87PEdwG2rJr02Nm
ge7J6nZ6TGZiXinl7j4vqn1sdFnxi1/h4pYZZ0xTXv6W4aE/L8yAspQUU7pz/hZ9AHyfIYmhV3pP8cEdJ0B5u7r0alptr5Ws
7GCMkmPDAKwRmTr+3oPo3MQQPLkbjG2ad9JkUvgJxR59f+80Phd7Bja159NEEQeGf8RhqVFrveXm3lFIBBaIywHBVRqgW5I3
u0Oz8dO5dgaBEnUTNTpLn2bU46x3vuI90vKA77SXIat12pLU3GhBxcWNTyK4HUd3NPZxhDcftm46zAOOIIPfP6ERG3UlTBLZ
4/Yx0jjEEuQ+i18r/gn1BMCWYJ9eD3B/6r2G8GmRYfAvC7ihQnLOveKrgTyLMevGUs+q5lgk0w1cgcb4GvyWrmOlbnQY1xD8
Dt2sm6I/JMCLxKiYxAl5C9MVOkWpyGEZmsAc2+Y6RK3df1bWn0Iws8bwPc5IaT1wY0O6Yltyip96rqN/p7bGHzI3T0x5z+NL
D1Oqa6JrE/HPBlA8JnNM3hdJ8mxRfhPrimXgFatnVK84V0dsEsSCvcZKF0cKSnoWobbzlL754405ytylXCGWgx06xsNdLV8k
hIJz/Y3LU0yNJZ3wadB1+zb4JrkwE8WgaAEMXCN66YhGM7tw9xr5K/5R0qYnOfCAmBNlPCqHBmEeAdO6VpD/Di1yyQ03M8An
iwd8X1+FjUt34oXgCXorXIGU8AjPWR8cvZ0TpKSsYdLSFAVcKQJ1ArWO+8VE7LEWcNjdlq60dlP3g4cbfZOPiZfe1anUVWJA
OeTLB3jTVm7Q+89J3V8j9AMUyPjpoNdQ1tyhr77l/L34lCQczTkDQJ+LIW8jlaYhzCAgWXLqMCmPoLdaM657FoTX/qtUII4R
S19fep9R9jaeOcaDKVFro1FXQHhHsigOxB3sazyhOjbvDG7+VlqieKKqChKB3k8rwNAHRO+z78UavbJdlNC8cOoJC1/2l1DG
ZgSpxuv4ihOmwF1qg+90DBFYIx1FG1i4yTECEms7VeD9Ekl+ZpN7kXmrSp8kuizzjtLX4UKIlNxhzQlMoXrHw6x7w15TZZHr
Xn9b03tIWc9ZAwx44kZR6fZyXYDrxN1108GIbQjBM/g+AF+XeynZR85UzAbWdwFMOBdnu+U6q5fBUASTdUZShH+en/ohsAxz
RjZUXmE+gKU2EKkSEqa0OKIZheQU9861nZT/tucmr7HVNum7LUNlfBZ2N05mUD80lrkIfkJR8orL6FMHNBErxQCZjS0WxtU9
7OctC+UdhnhOXAgdxeEF5g5wbNHYb07A4DA0nop1Al+gMp3udxFOv4xL3Hpr26XvYrctpXEbiQcdr7hT6418tYrKIlmSkiMv
q6FBdHDgwjcYZbwQnCii6l9rsCMKOeIPUvNyrr9CzfEXLZmyVjO+yINOqpwL7jgh2OnyHkJmDwLsCz6LyGPlGhpFTNaCM3T4
5rPWqD+pLPmPNjHNKFUaDVfKokweE1bOT38VqAQopdr1tg6GZo/LVVzhJ6VohzOfw00LBxSt18SSoMtxocOJgl2c+Huc9/Ci
i66nVscpvPMRd1gTG+Kkf7hRAeO+Cg3QB5teSSkz8ygZORGquRFLkyhvhGWe/eAG8YVAiXShsrxrZ92Q+wEWvkNblFX1IrrA
8mtdJDbWO6ts3Msc5IFdoIFRMxUuycuG4EgX6BIncwBYWJOAUzXAM01MwsRoiVIMseni3nLAJ17Fos0Lmg/klnXSPzNGMkjG
QtHArDyvLTRVnd2dNv1/6jp39+QHM0+hykh6Bi1Puv9sDeGLbHHwYXgHJOoUDUl0nAwStRvpBtNbsp14Ka5eEligrUOPxNv9
l7JbVs+7G1mvrPBNXp+S4J2UqwGx+wl2THhSM7SGj1xA8BDXYm5Jx6+zt3NNxHLiqxMSvnvjz8tMAb/vVM7MWbCubYlP1khT
PRQCmJFRnVHHNK6FV8XmGx9Niv1kh3DO4o9roh7GmXGpa4MeCe2Iz752VjdaOW/uBKucAxFy0h71+8Z/kzzlfiIyHhA7HB2y
CavgsHI5y3Fd1tDTMrIslZM25xgzscp+ueB7LtDBLQEWPWsk3/gozRTda1ToA9LbLITyuPtQSJkyDK9r5awdq5AS8rHs17cW
M8ppfUKwJNJKiNKH1iBBe6j1EdVpB9gc3L2Lj/15wzsgA/Yz3VT+Gkmtx9n35XDxdxuFvIfNU4PmPw8sa8gJ/oQVSxt6c3eH
OHXQg4rl91wtJM3/finuemP5NcsEWb1eORYoMiHHJ0mIGCe5bmtUcZ8U/jSThTMjzQ6MKtEZK6bTj/qmWbaufkWHgWj62kQ4
ozOceHE1O3KQgBEFNE16aQjgXx+IZFJgJMBlHRk6dvZtrFbWx6jXnMA1Ayp+UqwNDtY5Web/VmFceYDiXGK8LRLvQNrdTEvm
RY3aRJl+nU0oZSgn3cMS8A67bhV8TuejUtXs/Yn61hoDYWjWxOfZd40cZHJZbA+K1jja7TZ388gJJd06unYFQ3JDCcINfOxJ
C2WGsUMVLqtVNBf3K2y+P3P2X6YSDZ4DI1pjzImxdqiOtKoFylCT+CAm2OhOGe/n6BYQj0g8/5yBj9hU5RCg4vGS98HaiaWZ
ZYwuDellOjy1fjCS+KH0RvfqvL5Mi/qtYJlTC+WDG93V9QxZwlsneRdvz5/aRAb2+xZNwMgy8CCNIbLyBvVsr94gpsbm8uWj
dae1EE4DyZTVXOoU48gsYM0Q5R+xx6bv/auAZTNMfo4Fo8a+2YWr4qpfdIHHIxcYyHrnAFZiHzat1hQtv6DTVWQGjbWBcBrp
RvSpLqHCt0vsm40M9rhY2P8zt6gDt/TLwrffphMwpg0aWvHyFNiMueU5v4N0H6KKfLJjw0fMiv+YhHqN3d5F1HVqdkDGLonV
gGrf8KTj64cKCiq/cA5M0ftje3ihKMiyTAvplANrWp3Vcl8d92esgTbin3fhqLgQ1MJ+se6kFFf+Hpz4uEtnavXQyvftwDRS
k9iKGWBKvfgUTgJYjota6g6569/SgGFxog2208iFxdbsGIA+I6BSziv3yzq+dC0rByz9yfVobz9UPXwQkMyH/sXpAWgdV4Jc
+qxxYJQQyLbhXxkGuNa8julVj9pMgyMRcoMNMBQpsfldEQl+abo9C696imZTi/BThQA3lYRcW+BCZ31AcgBfsc5huxpETwvh
QIP1Wg598Zr+VWNSHgpWweW9UfQoF7sqSi+sovQP3YwcZSz2L1t3ysTnZD5ZAHFXkP2omuMfV2m9gUo7HaokSBw5G0Sctg0j
wjZSJPhZ9SR4GBJrRJ2ACHg5aPGdPd3qUsuawTnGlzq4w6sOg+lBB4iJLm234J0Qs5j0LV5rvumVr6vurFy74buZy1raru85
6zSMzICTR69WuMjWbqWBmQGG8ZZq+2ZgBDndA6G3Wb3pfE1XNzdz7/9YmoGWdW9DU3bl8A6buabkhg7usFEdZwgU9IlAk6+I
+tljDuvdUi98hqECzdgIbXZ95ZOJXu6i7rGNmeZE/t1DiRvs0GE5OO7xEalOnWwierxTqsGXzwyjpLGUZNH3tWRH0Uls6O2J
N4LXLmrcVyTuvKcq2eNWwH5HExE9XMeA/gK/XaDmEaua63oegoK+c5GfgdrIL4GRhxzrr+n5sqUBbDelWf91k9l6U+qjrSRj
nuupfxGi8eAFMB0cZcxWV6cBCALF1io5rc18CNAWEfrqLhXbfmz6gYH4N+FQlqc19RLT01ykpdPFJgS1tlI1vrHqG+ASPMZF
MAkW/hoqUSgLVJdtV7BxGA3bEqjfuXTKhxz5j8rsAjCN/4erVq7ZfWQb5V/H97jh1Z3+yYSxe7UinvxwXDQ/73vw4pZLY3Qp
kDf9q5Tl++E/qG9pI4UKjf+yphaF5VW9CTZSZpvasi0U8TH6OhrHl3DoG0anz+Vb6vZI0V+KennrtoZV1LFZvmX493odR5vw
yNbhYoMoilJ4/eX42acZ+tQjYuVhAJdq29KzQL7+IKq5jCd4uxXaGL8uWKjIJMEg/gAL8ta9FsrnFXE4LH5/JGGcu9YiEid5
K1zZbbsPi1V5TUrmCGZhl6NpYRQJ9oVAhvqUCuj5lU0Mr5/UWIwxkbNjIJS7IDp8HOadlmXAlEqqt7WCLELHFHwqb7LXkBnn
ryw6Ujj68b99aFrIUSd5rYOVAABEJGzos9KQL5Mu9WWw6irWh9koZsZ2COdi4VrDl1eSmtHIHt9N/ofdsI8SbEbMkWz40O7x
KVQNvoQNF1WtXpCn3JpW3Qe47I8KwzQKYf8Fuc4CaYEsYZdNA2c28HArmbSquyDkYohyvrCyDOl2J7Y9lWTGd4hQ2Kh9Y13m
b3tt5yAjlutjLzPjgiZVus2YSx8MdUpsE1y5hcP3sFhRK0D5KMwXI6DncEyD+BEgP393MYKcsJZ0jaG8rrw8iOr7WHehBDkm
tEa1D+F9+hu+dOpWKJwMJpnj0c/7VHzUK4H85f/8CslDvBMqnP1kzYeRo3YH+oOgSC4oUa8/6mPeEGxl5LWe2fI0uMj8LoTp
cjTJiafJqY8S6ssr8ZtOfYHnFgnus9WGkoV2J6QmA+VhchOEfWrhbdd4wawiwPMpDqlwsIBI50mQpIGp/RHAs2alhe+NoZc3
IQdMisY2HvWL+NUuNYrV49en5coNFhSOQZnH8mh0l0T/yI1OC/g09VXecQh3jjO8PbSeIoi0bAsjNqA0/QsRm8ejAAm0sa42
x0Ra/Ak+wh7kRe4W28m0axT3mcdQ5fxEroZFvHQhixiZWmX4jUfYy97JNn7wfv9pxxV3yYuDt1VWmyluPtmKApck0qVppJFH
poThzyI4tVSKD/jnbYFBw9uGgXh/bL6BfGHxgrcU/ZWFJUw6/lv7blBoNX4X38VnVlBDFukwj/YVIK0h38z3YEUnf9qlJjq0
9onCMSGS4aewfidd5mJW5+qOVzzyCiRnUM1PaP7WJCkLL0N+X2BHewwGfwSz0ffNpjHSXnFFaQum1dmH97+Y+2ug3g697CTf
dJ3VC0oDqtnpfUHzBYbppTuWibhnjJnv/7sbbfu3Rjt+3OIsUboxUYlyDjGU+uWTu9vjIfBkiUhw3IPtdtrrc/VqCmcy1mrK
hE8ScNolB6xKx3y5BBqz29Gv/CX3J15pq64+NMZITt2+tGCfOt/5MiJuR7cOk73Qt7UoxTxYTkrWO1Ik+jHCU2cOPeR5/6vp
funeuWHqLIwGvcEoA9hmOL/4egeyJE8+iWJSo8/eJbYf7x99Bo77K1IwH/pXYeLRAH+C+8Hd90RgtMbiy6e5VH6Hs7vttOE6
OrafqMHAMoZ9hCRW23Vm3Aozu8HQYkUNJm4leDraghykCwvRF09Q/F7+wxtZekletDQk543KjswRuohLesfv4msULn/TLUmt
ZOX1IdYIUyiIsnO1WYOMh1cYIhs9ZWuGQUvAt9HHpnf3X3a9gRVgg7PPliH5mLCbLxoFgZ25WT+KU5u4yOeWuwnXNV0mec4N
VxHG4eotYRLVIHbtbJDeA43fMOvUWA4CLoeUDUShAq/vyDcgwA==
```


## Changelog

- **1.0.0** — Initial specification of the QSPICE `.prot` / `.unprot` protected-block scheme: randomized base-16 encoding, seed-derived Mersenne Twister and table-walk keystreams, zlib decompression, and Windows-1252 keyword tokenization.

---
QSPICE is a registered trademark of Qorvo US, Inc.

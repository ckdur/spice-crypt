# SPDX-FileCopyrightText: © 2026 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

"""
Decryption support for QSPICE ``.prot`` protected model files.

This module provides :class:`QSpiceFileParser`, which streams a QSPICE library
or sub-circuit file and replaces each ``.prot`` … ``.unprot`` protected block
with its decrypted plaintext, passing all other lines through unchanged.  The
result is a fully usable, plaintext netlist.  See SPECIFICATIONS/qspice.md.
"""

import warnings
from collections.abc import Generator

from spice_crypt.qspice.cipher import QSpiceCipher

# Block delimiters (compared case-insensitively against the stripped line, as
# QSPICE itself does when scanning a netlist).  The spec defines exactly
# ``.prot`` / ``.unprot``; see SPECIFICATIONS/qspice.md Section 1.
_PROT_START = ".prot"
_PROT_END = ".unprot"


class QSpiceFileParser:
    """Parser for QSPICE ``.prot`` protected files with streaming support."""

    def __init__(self, file_obj):
        """
        Initialize the parser with a text-mode file object.

        Args:
            file_obj: File-like object (text mode) that supports iteration.

        Raises:
            TypeError: If *file_obj* is not iterable.
        """
        if not hasattr(file_obj, "__iter__"):
            raise TypeError("file_obj must be an iterable file-like object")
        self.file_obj = file_obj
        self.block_count = 0

    def decrypt_stream(self) -> Generator[bytes, None, tuple[int, int]]:
        """
        Stream decrypt the file, yielding plaintext chunks.

        Lines outside a protected block are emitted verbatim.  Each
        ``.prot`` … ``.unprot`` block is decoded, decrypted, inflated, and
        detokenized, and the resulting plaintext sub-circuit body is emitted in
        its place (the ``.prot`` and ``.unprot`` markers themselves are
        dropped).

        A block that cannot be decoded, decrypted, or inflated does not abort
        the stream: a warning is issued and the original block is emitted
        unchanged, so other blocks and all passthrough lines are still
        recovered (mirroring the resilience of the LTspice and PSpice parsers).

        Returns:
            Generator that yields decrypted/passthrough chunks as ``bytes``.
            The final value is ``(block_count, 0)`` — the number of protected
            blocks successfully decrypted.  QSPICE stores no integrity
            checksum, so the second element is always ``0``.
        """
        in_block = False
        encoded: list[str] = []
        raw_block: list[str] = []

        for line in self.file_obj:
            stripped = line.strip()
            lowered = stripped.lower()

            if not in_block:
                if lowered == _PROT_START:
                    in_block = True
                    encoded = []
                    raw_block = [line]
                else:
                    yield line.encode("utf-8", "replace")
                continue

            # Inside a protected block.  Retain the original lines so the block
            # can be re-emitted verbatim if decryption fails.
            raw_block.append(line)
            if lowered == _PROT_END:
                yield from self._emit_block("".join(encoded), raw_block)
                in_block = False
                encoded = []
                raw_block = []
            else:
                encoded.append(stripped)

        # A block left open at EOF means the file is truncated.  Warn, then make
        # a best-effort attempt to decrypt whatever payload was collected.
        if in_block:
            warnings.warn(
                "QSPICE .prot block was not terminated by .unprot before EOF; "
                "attempting best-effort decryption",
                stacklevel=2,
            )
            yield from self._emit_block("".join(encoded), raw_block)

        return (self.block_count, 0)

    def _emit_block(self, encoded: str, raw_block: list[str]) -> Generator[bytes, None, None]:
        """Yield one decrypted block, or warn and pass it through on failure.

        On success the decrypted plaintext is yielded and ``block_count`` is
        incremented.  If the block cannot be decoded, decrypted, or inflated, a
        warning is issued and the original block — ``.prot`` marker, encoded
        payload, and terminator — is emitted unchanged so the remainder of the
        file is still recovered rather than the whole stream aborting.
        """
        try:
            plaintext = self._decrypt_block(encoded)
        except ValueError as e:
            warnings.warn(
                f"QSPICE .prot block could not be decrypted ({e}); passing it through unchanged",
                stacklevel=2,
            )
            for raw in raw_block:
                yield raw.encode("utf-8", "replace")
            return
        self.block_count += 1
        yield plaintext

    @staticmethod
    def _decrypt_block(encoded: str) -> bytes:
        """Decrypt, inflate, and detokenize one block to UTF-8 bytes.

        The recovered text is QSPICE's Windows-1252 netlist body with its
        keyword tokens expanded (see :meth:`QSpiceCipher.detokenize`).  A
        trailing newline is appended so the block joins cleanly with the lines
        that follow, and it is emitted as UTF-8 to match the passthrough lines.
        """
        text = QSpiceCipher.detokenize(QSpiceCipher.decrypt_block(encoded))
        if text and not text.endswith("\n"):
            text += "\n"
        return text.encode("utf-8")


def _detect_qspice_format(file_obj, max_lines: int = 200) -> bool:
    """
    Auto-detect whether a seekable text file contains a QSPICE ``.prot`` block.

    Scans up to *max_lines* lines for a line equal to ``.prot`` (case-insensitive,
    whitespace-stripped), then restores the stream position.
    """
    pos = file_obj.tell()
    try:
        for i, line in enumerate(file_obj):
            if i >= max_lines:
                break
            if line.strip().lower() == _PROT_START:
                return True
    finally:
        file_obj.seek(pos)
    return False

# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

SpiceCrypt: a Python library + `spice-crypt` CLI that decrypts encrypted SPICE model files (LTspice, PSpice, QSPICE, and an in-progress HSPICE format). This is a fork of the original upstream project (no longer available), extended with HSPICE support; it does not use uv or maturin.

## Commands

```bash
# Dev environment: plain pip venv in .venv/ (the project does not use uv)
python3 -m venv .venv
.venv/bin/pip install --group dev        # pytest, ruff, pre-commit (PEP 735 group)
.venv/bin/pip install -e .               # builds the Rust extension
.venv/bin/python setup.py build_ext --inplace   # rebuild the extension after Rust edits

# Tests
.venv/bin/python -m pytest tests/ -v
.venv/bin/python -m pytest tests/test_pspice_decrypt.py::TestClass::test_name   # single test

# Lint / format (CI runs both; line length 100)
.venv/bin/ruff check .
.venv/bin/ruff format --check .
cargo fmt --check
cargo clippy -- -D warnings

# Everything pre-commit runs (ruff, rustfmt/clippy, version check, pytest, REUSE)
.venv/bin/pre-commit run --all-files     # local hooks call `python`, so activate .venv first

# CLI
.venv/bin/spice-crypt path/to/file.lib
.venv/bin/spice-crypt --recover-key path/to/mode4.lib   # PSpice Mode 4 brute force (needs Rust ext)
```

## Build system

The build backend is **setuptools + setuptools-rust** (not maturin). [setup.py](setup.py) defines the one compiled extension:

- `spice_crypt.pspice._aes_brute` — Rust/PyO3 (`Cargo.toml` points its `[lib]` at [spice_crypt/pspice/_aes_brute.rs](spice_crypt/pspice/_aes_brute.rs)). Optional at runtime; only needed for Mode 4 key recovery.

CI ([.github/workflows/ci.yml](.github/workflows/ci.yml)) builds with `pip install -e .`. The release workflow ([.github/workflows/publish.yml](.github/workflows/publish.yml)) builds wheels with cibuildwheel and the sdist with `python -m build`. [MANIFEST.in](MANIFEST.in) is required to ship the Rust source and `Cargo.toml` in the sdist (maturin used to do this implicitly); update it when adding extension sources.

## Architecture

**Dispatch** — [spice_crypt/decrypt.py](spice_crypt/decrypt.py) is the single entry point (`decrypt_stream`, `decrypt`). Format detection order matters:
1. Binary signature peek on the raw byte stream (first 20 bytes): LTspice `<Binary File>`, then HSPICE `.PROT RANDKEY`.
2. Otherwise the stream is wrapped as UTF-8 text, then: PSpice (`$CDNENCSTART` / `**$ENCRYPTED_LIB` within 50 lines) → QSPICE (`.prot`; reader is reconfigured to **cp1252**) → LTspice text/raw-hex fallback.

Every detector must restore the stream position after peeking.

**Parser contract** — each format has a `*FileParser` class whose `decrypt_stream()` is a generator that yields decrypted `bytes` chunks and *returns* a `(v1, v2)` verification tuple (delivered via `StopIteration.value`). `_run_decrypt_generator` drives it and writes to a file or string buffer. New formats should follow this contract and be wired into both `decrypt.py` and the exports in [spice_crypt/\_\_init\_\_.py](spice_crypt/__init__.py).

**Per-format packages** — `ltspice/`, `pspice/`, `qspice/`, `hspice/` each hold their own cipher primitives and parser. Shared crypto lives at the top level (`_des_base.py`, `_aes.py`, `_constants.py`). Pure Python, no runtime deps.

**Specifications** — [SPECIFICATIONS/](SPECIFICATIONS/) documents each reverse-engineered scheme in detail (key derivation, tables, file layout). Code comments reference section numbers in these files; read the relevant spec before changing a cipher, and update it when behaviour changes.

## HSPICE work in progress

`spice_crypt/hspice/` and `tests/test_hspice_decrypt.py` are new and incomplete: `BinaryFileParser` handles `.PROT RANDKEY` (SHA-256 of header, ISAAC-based keystream), its docstring still has placeholder text, and the integration test is a stub. Test data is [tests/data/hspice/nhvt.mdl](tests/data/hspice/nhvt.mdl) from the ICS55 PDK.

## Conventions

- Every file needs SPDX copyright and license tags (AGPL-3.0-or-later for code) in its header comment; REUSE compliance is checked in CI/pre-commit (`reuse lint`). Files that cannot carry a header are annotated in [REUSE.toml](REUSE.toml).
- QSPICE files intentionally contain Windows-1252 glyphs (`Ã Ø ¥ × « » ´ µ`); ruff's RUF001-003 are disabled for those files only.
- [scripts/check_version.py](scripts/check_version.py) enforces that the "Development Status" classifier matches the version in `pyproject.toml`.
- Test fixtures are generated (e.g. [scripts/gen_qspice_testdata.py](scripts/gen_qspice_testdata.py)); tests compare decrypted bodies against `PLAINTEXT_BODY` in [tests/conftest.py](tests/conftest.py).

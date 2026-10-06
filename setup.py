# SPDX-FileCopyrightText: © 2026 Ckristian Duran. <ckdur.iso@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

from setuptools import setup
from setuptools_rust import Binding, RustExtension

setup(
    rust_extensions=[
        RustExtension(
            "spice_crypt.pspice._aes_brute",
            path="Cargo.toml",
            binding=Binding.PyO3,
        ),
    ],
)

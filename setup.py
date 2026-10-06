# SPDX-FileCopyrightText: © 2026 Ckristian Duran. <ckdur.iso@gmail.com>
#
# SPDX-License-Identifier: AGPL-3.0-or-later

from setuptools import Extension, setup
from setuptools_rust import Binding, RustExtension

setup(
    ext_modules=[
        Extension(
            "spice_crypt.hspice._pyisaac",
            sources=[
                "spice_crypt/hspice/_pyisaac.c",
                "spice_crypt/hspice/rand.c",
            ],
            include_dirs=["spice_crypt/hspice"],
        ),
    ],
    rust_extensions=[
        RustExtension(
            "spice_crypt.pspice._aes_brute",
            path="Cargo.toml",
            binding=Binding.PyO3,
        ),
    ],
)

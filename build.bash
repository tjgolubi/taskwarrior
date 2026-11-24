#!/bin/bash
set -euo pipefail

export PATH="/usr/bin:$HOME/.cargo/bin:$PATH"
export TMPDIR="$PWD/build/test-tmp"

set -x

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PWD/build/local-install"
cmake --build build --target task_executable test_runner

cmake -P build/doc/cmake_install.cmake
mkdir -p "${TMPDIR}"
ctest --test-dir build --output-on-failure -j1 -R '[.]cpp$'
ctest --test-dir build --output-on-failure -j8 -E '[.]cpp$'

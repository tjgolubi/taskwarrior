#!/bin/bash
set -euo pipefail

export PATH="/usr/bin:$HOME/.cargo/bin:$PATH"
export TMPDIR="$PWD/build/test-tmp"
mkdir -p "${TMPDIR}"

if [[ -n "${TERMUX_VERSION:-}" ]]; then
  target_flag="--target=aarch64-linux-android24"
  export CFLAGS_aarch64_linux_android="$target_flag"
  export CXXFLAGS_aarch64_linux_android="$target_flag"
fi

set -x

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PWD/build/local-install"
cmake --build build --target task_executable test_runner

cmake -P build/doc/cmake_install.cmake
ctest --test-dir build --output-on-failure -j1 -R '[.]cpp$'
ctest --test-dir build --output-on-failure -j8 -E '[.]cpp$'

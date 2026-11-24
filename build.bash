#!/usr/bin/env bash
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

target_flag="--target=aarch64-linux-android24"
export CFLAGS_aarch64_linux_android="$target_flag"
export CXXFLAGS_aarch64_linux_android="$target_flag"

cmake -S . -B build-termux -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build-termux --parallel 2
cmake --build build-termux --target test_runner --parallel 2
./build-termux/src/task --version
ctest --test-dir build-termux --output-on-failure --parallel 2
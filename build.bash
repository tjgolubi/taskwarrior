#!/usr/bin/env bash
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

target_flag="--target=aarch64-linux-android24"
export CFLAGS_aarch64_linux_android="$target_flag"
export CXXFLAGS_aarch64_linux_android="$target_flag"

cmake -S . -B build-termux
cmake --build build-termux --parallel 2
./build-termux/src/task --version

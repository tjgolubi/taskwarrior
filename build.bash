#!/usr/bin/env bash
set -euo pipefail

mkdir -p -- build

set -x

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="build/local-install"

cmake --build build --target task_executable --parallel 2
cmake --build build --target test_runner     --parallel 2
cmake -P "build/doc/cmake_install.cmake"
ctest --test-dir build --output-on-failure

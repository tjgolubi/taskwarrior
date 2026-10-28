# TaskWarrior build and validation

[Document map](README.md) · [Workflow](Workflow.md) · [Integration design](CppDesign.md)

Updated October 28, 2026.

## Environment and dependency

Use MSYS2 tools and objects, not UCRT64 or MSVC. Inspected after Terry ran
`pacman -Syu` and rebooted: GCC 16.2.0, CMake 4.4.4, Ninja 1.13.2, Rust 1.99.1.
Previously recorded Cargo 1.99.1 and Python 3.12.15 were not freshly rechecked.
Both consumers use C++17, Release, and `FMT_COLOR_COMPACT_SGR=1` publicly through
their libshared targets, which link `fmt::fmt` publicly.

Fmt is now pinned at `5b3f49ff7599df5beb651c3ccc8a403d7a7386a1` in libshared's
`ext/fmt` submodule (TaskWarrior path src/libshared/ext/fmt), from tjgolubi/fmt's
color-dev. Both projects add it directly through CMake; no local install or
fmt_DIR is required. Compiler commands were checked to confirm this path.
The separate ../fmt checkout and old build/fmtcolor-install are not consumed.
Do not silently substitute ordinary upstream/system fmt: the integration uses
custom color APIs. Constructors assert on invalid values with assertions enabled
and clamp when disabled. The verified builds are Release.

## Directories and MSYS fixes

- `^/build/color-dev-baseline`: current TaskWarrior build and tests. The name
  reflects its initial upstream-only purpose; it now contains the integration.
- `^/build/libshared-installed-fresh`: current standalone libshared build. Despite
  the historical name, it now builds the ext/fmt submodule directly.
- `^/build/libshared-upstream-post-update`: historical upstream-only tests.
- `^/build/msys-gcc16`, fmtcolor-package, fmtcolor-install, and color-cxx17-check:
  older builds, probes, and logs; do not confuse them with current validation.
- `^/coord`: persistent worktree, never place it under disposable build/.

TaskWarrior commit `101261b26` provides HAVE_TIMEGM detection and cmake.h output,
selects patched ring 0.17.14 through Cargo's patch.crates-io section, and exposes
POSIX declarations privately to test_runner on MSYS. The full vendor/ring tree
is intentional build support, separate from FmtColor. The small Cygwin randomness
and Win64 ABI patch and provenance are documented in vendor/. Retain it unless
Terry authorizes a different dependency solution. Upgrading Rust alone did not
address the observed missing ring Cygwin implementation.

TaskWarrior's integration also defines `_POSIX_C_SOURCE=200809L` privately for
fmt on MSYS because its strict C++17 build otherwise hides declarations needed
by fmt/os.cc. Libshared's separate `<ctime>` include fix in test/test.h is commit
`3e94fdc5f`; without it upstream test executables failed to compile.

Unmodified upstream TaskWarrior failed on ring's SystemRandom trait support and
Datetime's timegm fallback (setenv/tzset/unsetenv undeclared). These have been
resolved in the current build. Ring lifetime-syntax warnings remain nonfatal.

## Configure and build

From TaskWarrior root in MSYS Bash (automation uses bash --noprofile --norc):

```bash
export PATH=/usr/bin:/home/tjgolubi/.cargo/bin:$PATH
# First inspect git status and recorded gitlinks. Only initialize/update when
# appropriate: this checks out the parent's pinned revisions.
git submodule update --init --recursive

cmake -S . -B build/color-dev-baseline -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PWD/build/color-dev-baseline/local-install" \
  -Ufmt_DIR
cmake --build build/color-dev-baseline \
  --target task_executable test_runner --parallel 8

cmake -S src/libshared -B build/libshared-installed-fresh -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -Ufmt_DIR
cmake --build build/libshared-installed-fresh --parallel 8
```

-Ufmt_DIR removes an obsolete installed-package cache entry when reusing a build.
No package installation is required. Do not run standalone libshared install to
its cached /usr/local prefix. Do not overwrite Terry's normal task executable.
The current test executable is build/color-dev-baseline/src/task.exe.
Configuration can regenerate cmake.h files in source trees.

## TaskWarrior tests and evidence

A fresh build needs themes/resources under its workspace-local install prefix.
Check that prefix before running the doc-only resource install:

```bash
cmake -P build/color-dev-baseline/doc/cmake_install.cmake
mkdir -p build/color-dev-baseline/test-tmp
export TMPDIR="$PWD/build/color-dev-baseline/test-tmp"
ctest --test-dir build/color-dev-baseline --output-on-failure -j1 -R '[.]cpp$'
ctest --test-dir build/color-dev-baseline --output-on-failure -j8 -E '[.]cpp$'
```

Use isolated test configuration/data. Never use Terry's real task database.
C++ tests share temporary paths and run serially; scripts may run concurrently.
ids.test.py commonly takes around 100 seconds.

Latest validation is TaskWarrior `3272705b95583d9fc5689d10d3f5487fdf6fde59`,
libshared `58ae81d0a90b93839c1157f0cac3e85f888154cb`, fmt
`5b3f49ff7599df5beb651c3ccc8a403d7a7386a1`, and Corrosion
`2522f7abad9eaa88b05d07fd492841c529ca2bf1`. The incremental build succeeded and
all 179 tests passed: 32 C++ in 0.99 seconds, 147 scripts in 107.41 seconds.
Git remained clean. The current TaskWarrior commit is now
`a129245774249abcb0ad26b89220ddd7f1bd5303`: only INSTALL differs from the tested
3272705b9. Terry amended the installation instructions and force-pushed. No new
build or tests were run after that documentation-only amendment. INSTALL now
documents recursive submodule initialization and direct use of ext/fmt.
Logs in build/color-dev-baseline:

- final-build.log
- final-cpp-tests.log
- final-script-tests.log

Earlier fmt-submodule-* logs cover the first nested-fmt build/test. Its initial
run failed because default.theme resources were absent; after the doc resource
install both suites passed. ext-rename-build.log verifies the subsequent rename.
The final-* results supersede these intermediate results. The previous 178-test
baseline used the older upstream base; current upstream adds a C++ entry.

Earlier manual color rendering and CmdNews/CmdColor output comparisons remain
historical evidence only. Do not claim fresh byte comparisons, clean builds,
or CI results from the current incremental build and tests.

## Standalone libshared tests and evidence

```bash
mkdir -p build/libshared-installed-fresh/test-tmp
export TMPDIR="$PWD/build/libshared-installed-fresh/test-tmp"
cmake --build build/libshared-installed-fresh --target test --parallel 8
```

The test target builds the otherwise excluded test executables and runs the full
suite. Latest run at `58ae81d0a`, using fmt `5b3f49ff` under ext/fmt, reports
**11,894 passed, seven failed, four skipped**. The library build was already up
to date after configuration; tests were rebuilt as necessary and rerun. Logs in
build/libshared-installed-fresh are truck-configure.log, truck-build.log, and
truck-tests.log (the filenames retain an intermediate branch name; checkout at
this run was color-dev). Git remained clean.

| Suite | Latest result |
| --- | --- |
| Color | 1097 passed, 0 failed |
| Shared utilities | 211 passed, 0 failed |
| Palette | 3 passed, 0 failed |
| Table | 3 passed, 0 failed |
| Composite | 5 passed, 1 known emoji-width failure |
| Filesystem | 125 passed, 0 failed, 3 skipped |
| UTF-8 | 86 passed, 6 known emoji-width failures |
| Date/time | 6640 passed, 0 failed |

Filesystem skips are guarded by __MSYS__ and limited to the observed file-owner
execute and directory other-read/other-execute checks. Their mount/ACL cause is
not established. The fourth skip is pre-existing elsewhere. The upstream-only
checkout with just the ctime fix reported 11,841 passed, ten failed, one skipped:
those same seven emoji cases plus the three permission cases still asserted.
Do not mistake that baseline for the integration's latest result.

Keep the seven emoji assertions: Terry agreed to leave their failures alone,
not disable them. UTF-8 returns width 1 where 2 is expected; Composite has a
related layout mismatch. Earlier upstream emoji changes are already included.
Previously observed next-weekday failures did not recur in the latest run; their
cause was not established. datetime.t and utf8.t still report obsolete planned
counts, separately from failed assertions. Do not adjust expectations merely to
make a suite green.

This documentation update reads existing evidence; it runs no new tests. Terry
reported everything pushed; no network publication check or integration CI run
is claimed. Preserve exact source/dependency revisions, skips, and distinctions
between historical failures and current verification.

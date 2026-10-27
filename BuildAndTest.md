# TaskWarrior build and validation

[Document map](README.md) · [Workflow](Workflow.md) · [Integration design](CppDesign.md) · [Documentation](Website.md)

Updated October 27, 2026.

## Environment and dependency

The verified environment is MSYS2, not UCRT64 or MSVC. Tool versions inspected
October 26: GCC 16.2.0, CMake 4.4.4, Ninja 1.13.2, Rust/Cargo 1.99.1, and
Python 3.12.15. Application and standalone libshared use C++17 and Release builds.
The current clean source revisions and separate dirty coordination tree are recorded
in README.md.

The installed dependency comes from the clean sibling fmt TaskWarrior branch at
`eb0444023168fbe79f68f2c032bf1ca67ebd47b8`. The workspace-local static installation
is `^/build/fmtcolor-install`; CMake explicitly selects its lib/cmake/fmt package.
The current installed revision already contains the noexcept assert/clamp
constructors; an earlier libshared run before refreshing that installation did not.
This is a local override, not portable acquisition or an automatically enforced
pin. Do not silently substitute another system fmt or mix MSYS and MSVC objects.

Both selected library targets publicly link fmt::fmt and define
`FMT_COLOR_COMPACT_SGR=1`, so consumers inherit a consistent compact-SGR mode.
TaskWarrior builds its selected libshared sources through src/CMakeLists.txt;
standalone libshared uses src/libshared/src/CMakeLists.txt. No FMT_HEADER_ONLY or
global CXX flag override is needed. Installed headers include color-ostream.h,
with styled stream insertion and narrow/wide ostream print/println support.
The installed color.h SHA-256 was verified against ../fmt on October 27.
Constructors assert on invalid values when assertions are enabled, and clamp
when assertions are disabled. Both consumer builds here are Release.

## Build directories and retained fixes

- `^/build/fmtcolor-package`: fmt package build from the sibling source.
- `^/build/fmtcolor-install`: headers, lib/libfmt.a, and CMake package files.
- `^/build/msys-gcc16`: TaskWarrior application and CTest suite.
- `^/build/libshared-installed-fresh`: standalone libshared and its own tests.
- `^/build/color-cxx17-check`: disposable probes and validation logs.
- `^/coord`: persistent coordination working tree; never put it under build.

Terry may delete all of build to force a clean rebuild. These directories, logs,
probes, and local dependency installation then need recreation; coordination
documents remain in coord. Preserve experiments unless deletion is requested.

MSYS build fixes include HAVE_TIMEGM detection, the local vendor/ring patch
selected by Cargo.toml's patch.crates-io section, and
`_POSIX_C_SOURCE=200809L` private to test_runner on MSYS. Keep the vendored
ring 0.17.14 Cygwin support and existing TaskChampion/Corrosion integration.
Rust currently emits ring lifetime-syntax warnings; the verified builds succeed.

## Configure and build

Run from `^/` in MSYS Bash with the MSYS compiler and Terry's Cargo bin directory
on PATH. In tool automation use bash --noprofile --norc, not a login shell.
The following makes the development source override explicit; verify its HEAD
against the recorded revision first; it is an inspected snapshot, not an enforced
portable pin. A deleted build needs the configure steps again.

```bash
fmt_prefix="$PWD/build/fmtcolor-install"
cmake -S ../fmt -B build/fmtcolor-package -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ \
  -DFMT_TEST=OFF -DFMT_DOC=OFF -DFMT_INSTALL=ON \
  -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_LIBDIR=lib \
  -DCMAKE_INSTALL_PREFIX="$fmt_prefix"
cmake --build build/fmtcolor-package --parallel 8
cmake --install build/fmtcolor-package

cmake -S . -B build/msys-gcc16 -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PWD/build/msys-gcc16/local-install" \
  -Dfmt_DIR="$fmt_prefix/lib/cmake/fmt"
cmake --build build/msys-gcc16 \
  --target task_executable test_runner --parallel 8

cmake -S src/libshared -B build/libshared-installed-fresh -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -Dfmt_DIR="$fmt_prefix/lib/cmake/fmt"
cmake --build build/libshared-installed-fresh --parallel 8
```

Only fmt is installed by these commands, under the workspace. Do not run a
standalone libshared install to its cached /usr/local prefix. The experimental
application is `^/build/msys-gcc16/src/task.exe`; do not overwrite Terry's normal
installation. Configuration can regenerate cmake.h files in the source trees.

For a header-only API change, refresh the local fmt installation and rebuild
consumers. A CMake reconfigure is not intrinsically needed, but is appropriate
when package configuration or build definitions change. A new header must be
listed in the library's install rules to reach the application.

## TaskWarrior tests

Use temporary test configuration/data. Set TMPDIR to a workspace-local directory.
Run C++ tests serially because some share a fixed temporary directory. Script
tests can run concurrently; ids.test.py usually takes around 100 seconds.

```bash
mkdir -p build/msys-gcc16/test-tmp
export TMPDIR="$PWD/build/msys-gcc16/test-tmp"
ctest --test-dir build/msys-gcc16 --output-on-failure -j1 -R '[.]cpp$'
ctest --test-dir build/msys-gcc16 --output-on-failure -j8 -E '[.]cpp$'
```

For a fresh build, script tests may need the documentation/theme resources under
the configured workspace-local install prefix. They were installed locally with
`cmake -P build/msys-gcc16/doc/cmake_install.cmake`. Inspect the prefix before
repeating this resource-install step; do not install globally.

Latest full verification, October 27, after deliberately restoring the original
FmtColor / MyColor baseline and the recorded Corrosion checkout: all 178 CTest
entries passed (31 C++ and 147 script tests). Both projects were reconfigured,
rebuilt, and tested after restoration. These results correspond to the current
committed source revisions in README.md, not the discarded rebase branches.

Evidence under `^/build/color-cxx17-check/`:

- `restored-task-configure.log` and `restored-task-build.log`.
- `restored-task-cpp-tests.log`: 31 passed, zero failed.
- `restored-task-script-tests.log`: 147 passed, zero failed.
- `restored-shared-configure.log`, `restored-shared-build.log`, and
  `restored-shared-tests.log`: the standalone run described below.

Logs prefixed `trydevelop-` and `rebased-shared-` belong to the successful,
subsequently undone rebase rehearsal. That newer upstream base passed 179
TaskWarrior tests (32 C++ plus 147 scripts). Do not confuse its extra C++ test
or newer Corrosion revision with the current restored checkout. Future rebases
may rebuild substantial Rust dependencies even when an incremental build was
previously quick. Building the new base does not itself authorize source fixes.

This documentation update reads existing evidence; it is not another test run
or a CI result. No application CI run is claimed.

The earlier formatting pass exposed a dashed-header rule emitting spaces.
Table.cpp now uses `"{:-<{}}"`; fontunderline.test.py and the affected table,
report, and C++ tests were rerun successfully before the latest full pass.
Earlier CmdNews output comparisons covered colored/uncolored interactive news
and up-to-date cases. CmdColor comparisons covered all swatches, a sample, and a
legend byte for byte before Terry's subsequent manual review edits. Do not claim
those older exact-byte comparisons describe every later edit. Terry manually
confirmed the current color command renders correctly and bold works with
indexed colors in his terminal.

## Standalone libshared tests and known limitations

TaskWarrior's CTest suite does not replace libshared's own tests. The default
standalone build excludes test executables; build specific targets or target test.

```bash
cmake --build build/libshared-installed-fresh \
  --target color.t palette.t table.t composite.t fs.t --parallel 8
# Full suite, including building the remaining test targets:
cmake --build build/libshared-installed-fresh --target test --parallel 8
```

The latest restored-baseline full suite reports **11,894 assertions passed,
seven failed, and four skipped**. Its nonzero exit is caused by the seven known
emoji assertions, not a compilation failure. Color contains 1,097 passing
assertions, including the 39 added emphasis assertions.

| Suite | Current recorded result |
| --- | --- |
| Color | 1097 passed, 0 failed |
| Shared utilities | 211 passed, 0 failed |
| Palette | 3 passed, 0 failed |
| Table | 3 passed, 0 failed |
| Composite | 5 passed, 1 known emoji-width failure |
| Filesystem | 125 passed, 0 failed, 3 explicitly skipped |
| UTF-8 | 86 passed, 6 emoji-width failures |

The fourth overall skip is pre-existing elsewhere; do not describe all four
as newly gated filesystem tests. The temporarily added split() view tests were
reverted with that refactor, so shared utilities are back to 211 assertions.

Filesystem skips are guarded by __MSYS__ and limited to the observed file-owner
execute bit and directory other-read/other-execute bit assertions. Other
platforms retain the assertions; all other filesystem tests remain active.
Observed permission differences are documented, but their precise mount/ACL
cause was not established. Do not describe them as a proven universal MSYS2 bug.

Terry agreed to leave the seven emoji-related failures alone for now, not to
disable their assertions. The six UTF-8 cases return width 1 where width 2 is
expected; Composite has a related overlap/layout mismatch. The inspected
upstream libshared master `d899eba6d1a863f78f15e5e82d45d857278e9471` has identical
utf8.cpp, utf8.h, and utf8.t.cpp, and the last inspected TaskWarrior develop
selects that revision. Earlier upstream emoji/Composite fixes are already present.

An earlier October 27 run also failed the existing "monday -> next monday" and
"tuesday -> next tuesday" assertions, giving nine failures. Those two checks
passed in the latest run. Their cause was not established; do not attribute
them to FmtColor or silently adjust expectations. Investigate only if requested.

datetime.t and utf8.t also report obsolete planned assertion counts. Record these
separately from failed assertions. Do not infer a fix or change expectations
merely to make the suite green.

## Verification discipline

Record the full application/submodule/fmt revisions, dirty state, toolchain,
commands, results, and skips. Preserve distinctions between baseline problems,
new failures, historical logs, and checks after current edits. Re-run applicable
checks after source or dependency changes; do not cite earlier library CI as
application validation. No integration CI has been triggered or claimed here.

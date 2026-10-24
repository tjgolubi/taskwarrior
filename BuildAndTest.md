# TaskWarrior build and validation

[Document map](README.md) · [Workflow](Workflow.md) · [Integration design](CppDesign.md) · [Documentation](Website.md)

Updated October 23, 2026.

## Current evidence

No fresh TaskWarrior baseline build, FmtColor consumer build, integration test,
or runtime validation has been performed for this project. No dependency has
been installed or integrated. Existing local build directories and UCRT64 work
are not evidence for the new `upstream/develop` baseline.

FmtColor's coordination records report 43 successful jobs for its proposed pin
`d9d64d361591311901e06c662c24879128c63724`, including guide publication.
They also report 42 jobs for a separate upstream-compatibility branch, with
intentional test/documentation adjustments. These are historical dependency
results, not fresh verification here or proof of TaskWarrior compatibility.

## Establish a reproducible baseline

Before configuring or building:

1. Verify the upstream remote, selected `upstream/develop` full SHA, repository
   instructions, working tree, and exact submodule commits. Establish the fresh
   working checkout only when authorized; preserve the existing checkout.
2. Read that revision's CMake files, installation/development instructions,
   test documentation, and CI workflows. Determine actual compiler, C++ standard,
   Rust, native-library, and test-runner requirements.
3. Identify tool provenance and versions. Use the agreed MSYS environment on
   this PC; do not reinstall UCRT64 or copy incompatible caches. If the fresh
   application requires a toolchain decision, report it before changing tools.
4. Select a separate build directory. Inspect configure-time downloads,
   submodule initialization, generated source-tree files, and other side effects
   before running CMake. A configure command is not necessarily read-only.
5. Once build work is authorized, build and test the unmodified baseline first.
   Record failures before attributing them to FmtColor.

Use MSYS2 Git as described in [Workflow.md](Workflow.md). Reuse existing
credentials and agent setup. Network acquisition and installation must follow
available permissions; do not change global settings as an incidental fix.

## Commands to confirm, not yet verified

The previously inspected checkout documents this general sequence. Confirm
commands and target names at the fresh baseline before using them:

```bash
cmake -S . -B build/fmtcolor-baseline -DCMAKE_BUILD_TYPE=Release
cmake --build build/fmtcolor-baseline --parallel 8
cmake --build build/fmtcolor-baseline --target test_runner --target task_executable
ctest --test-dir build/fmtcolor-baseline -N
ctest --test-dir build/fmtcolor-baseline --output-on-failure
```

Listing tests establishes actual names before selecting focused tests. Determine
whether low-level color tests in a submodule need separate build targets or a
standalone configuration. Do not assume TaskWarrior's CTest invocation covers
them automatically. Confirm Python test dependencies and test executable paths.
The example sequence is not an instruction to run builds during assessment.

## Dependency consumer verification

After the integration plan is accepted, record how FmtColor is acquired, pinned,
configured, and linked. Verify the compiler actually selects the pinned
`<fmt/color.h>`, rather than a system {fmt} header. Check for existing {fmt}
dependencies, conflicting versions, target visibility, and header-only versus
compiled-library consistency before choosing a mode.

Verify reproducibility without an implicit sibling checkout. Document an
explicit local source override if one is supported. Test the language mode
TaskWarrior actually requires; FmtColor's C++11 support does not require changing
the application's language standard or running its whole compiler matrix.

## Application checks after changes

Choose checks from the source assessment and changed behavior:

- Existing low-level color parsing, conversion, comparison, and output tests.
- Foreground/background colors, specified black, empty styles, attributes,
  brightness, 16/256/truecolor modes, and reset/restoration behavior.
- Rule precedence and blending, representative themes, reports, tables,
  disabled color, and width/truncation where affected.
- Relevant CLI color, color-rule, and diagnostic tests discovered at baseline.
- Configuration round trips or numeric translation if those interfaces remain.

Add targeted regressions for newly introduced behavior or gaps in existing
coverage, not tests that merely repeat an implementation detail. Broaden to the
applicable full application suite after focused checks pass. Submodule changes
also need that repository's relevant checks. A visual terminal check complements
rather than replaces automated assertions.

Use temporary test data/configuration and explicitly isolated task commands;
never run exploratory commands against Terry's real task database or taskrc.
Do not install the experimental executable over the normal task command.

## Recording results

For each meaningful result, record application branch/full SHA, submodule SHAs,
FmtColor pin, local diff status, toolchain, build configuration, command, result,
and skips. Label baseline failures, unavailable tests, and manual observations.
Do not treat successful jobs with skipped tests as complete exercised coverage.

Inspect TaskWarrior's actual CI requirements. Any changed commit or dependency
pin needs new applicable checks. FmtColor's upstream-compatibility evidence does
not establish application semantics, ABI, performance, or persisted-data safety.

# TaskWarrior / FmtColor coordination

Updated October 28, 2026.

## Start here in a new conversation

Read this file, [Workflow.md](Workflow.md), [CppDesign.md](CppDesign.md), and
[BuildAndTest.md](BuildAndTest.md) before acting. These are the authoritative
coordination documents on TaskWarrior's `doc/Coord` branch in `^/coord/`.
Source and Terry's newer instructions take precedence. Change things only on
an imperative instruction; statements of intent are discussion. Follow Terry's
next task rather than automatically starting another cleanup or Git operation.

The purpose is to showcase FmtColor's readable, named styles in an application
Terry uses. Avoid requiring programmers to consult an SGR manual. Performance
is a possible benefit, not a measured outcome. Do not disparage the original code.

## Document map

| Document | Purpose |
| --- | --- |
| [Workflow.md](Workflow.md) | Authorization, paths, Git, and working trees. |
| [CppDesign.md](CppDesign.md) | Interfaces, output conventions, and open work. |
| [BuildAndTest.md](BuildAndTest.md) | Current commands and validation evidence. |

## Paths

`^` and `taskwarrior:` mean `~/prj/taskwarrior`, regardless of current directory:
`C:/msys64/home/tjgolubi/prj/taskwarrior` or `/home/tjgolubi/prj/taskwarrior`.
`.` retains its shell meaning.

- `^/`: TaskWarrior source, branch `color-dev`.
- `^/coord/`: coordination worktree, branch `doc/Coord`.
- `libshared:`: separate repository/submodule at `^/src/libshared`, `color-dev`.
- Dependency fmt: nested submodule at `^/src/libshared/ext/fmt`, `color-dev`.
- `fmt:` historically means sibling `^/../fmt`, a separate development checkout.
  It is no longer the source consumed by these builds. Distinguish the two paths.

Terry explicitly prefers `ext` to `external`. The earlier external/fmt path was
renamed. Do not move it back or invent another dependency location.

## Verified current baseline

Inspected October 28 after Terry amended and pushed the integration. TaskWarrior,
libshared, and nested fmt are clean. All recorded recursive gitlinks match the
checkouts. This documentation update remains uncommitted on `doc/Coord`; it does
not authorize a documentation commit or push. `/coord/` is locally ignored by
TaskWarrior through `.git/info/exclude`.

| Repository | Current revision |
| --- | --- |
| TaskWarrior `color-dev` | `9e3f918894727b0e20d979a222fd10706f5cf613` |
| libshared `color-dev` | `58ae81d0a90b93839c1157f0cac3e85f888154cb` |
| nested fmt `color-dev` | `5b3f49ff7599df5beb651c3ccc8a403d7a7386a1` |
| Corrosion detached | `2522f7abad9eaa88b05d07fd492841c529ca2bf1` |

TaskWarrior history is upstream develop `61e542918`, then MSYS build support
`101261b26`, then the single squashed FmtColor integration `9e3f91889`.
Libshared history is upstream master `d899eba6d`, then the missing `<ctime>`
fix `3e94fdc5f`, then the single combined integration `58ae81d0a`. The latter
includes the fmt submodule and CMake setup. Truck and color-tjg were intermediate
history-management branches; do not assume they are current work targets.

The earlier rebase rehearsal was undone, but Terry has since performed the real
rebases and squashes. FmtColor/MyColor and the old pre-rebase Corrosion revision
are historical, not the current baseline. Both previously missing upstream
libshared commits df59fd691 and d899eba6d are now included. Their general
subprocess fixes remain present; they did not fix the known emoji failures.

## Dependency and publication

Both consumers now build the pinned nested fmt source directly with
`add_subdirectory`, not `find_package` or `build/fmtcolor-install`.
TaskWarrior compiles selected libshared sources using its own CMake entry point,
so it explicitly adds the same nested fmt directory. Both use C++17 and compact
SGR mode. TaskWarrior defines `_POSIX_C_SOURCE=200809L` privately for fmt on MSYS.

TaskWarrior's committed `.gitmodules` points to
`https://github.com/tjgolubi/libshared.git`. Libshared's points to
`https://github.com/tjgolubi/fmt.git`, path `ext/fmt`, branch `color-dev`.
Git records an exact fmt commit; normal submodule update does not follow the
latest branch tip. Initialization is `git submodule update --init --recursive`.
Before running it, check the parent's gitlink: update checks out that recorded
revision and can detach a submodule from its development branch.

Terry reported everything pushed, then amended and force-pushed the INSTALL and
CI fixes. Current local color-dev tracking matches origin/color-dev. The agent
did not push. Forks are tjgolubi/taskwarrior, tjgolubi/libshared, and tjgolubi/fmt.

INSTALL now documents recursive initialization and the pinned nested fmt source.
All eight test/docker recipes now use git submodule update --init --recursive.
CPack source exclusions for build, target, and test now match directory paths
rather than substrings, preserving required ring sources such as build.rs,
target.h, and constant_time_test.c. These fixes are included in current HEAD.

GitHub Actions was inspected for 9e3f91889: all enabled jobs passed, including
the basic Linux tests, Cargo tests, seven Docker builds/tests, and release
tarball build. Both macOS jobs are disabled by the workflow and skipped.
Links and verification limits are in BuildAndTest.md.

## Current implementation and validation

`Color` is a global alias for `fmt::text_style`; helpers Parse, Spec, strip_sgr,
and colorize are in namespace `Style`. Do not rename them again. Named factories,
fluent composition, signed numeric interfaces, and styled ostream support are
integrated. Prefer fluent composition over `|` when all else is equal.

Latest October 28 validation:

- TaskWarrior at `3272705b9`: rebuilt successfully; all 179 CTest entries passed
  (32 C++ and 147 scripts). This is the last full local suite, before the INSTALL
  and CI setup amendments. Current 9e3f91889 passed all enabled CI jobs; its
  regenerated source archive also built locally on MSYS2.
- Libshared at `58ae81d0a`: configured/built successfully using ext/fmt; the full
  suite reports 11,894 passed, seven known emoji failures, four skipped.
- All 1,097 Color assertions passed. Date/time assertions passed this run.
- The three observed MSYS filesystem permission cases are explicitly skipped
  in the integration; they still fail in the upstream baseline without those guards.
- See BuildAndTest.md for paths, logs, and limits. No fresh tests run for this
  documentation-only update.

The deferred split() string_view refactor remains reverted. Keep it separate
from color work. Optional disposable patches live under build/color-cxx17-check.
No integer-interface reminder remains pending; the requested API was implemented.
Remaining work is in [CppDesign.md](CppDesign.md#to-do-and-open-questions).

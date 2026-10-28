# TaskWarrior / FmtColor coordination

Updated October 28, 2026.

## Start here in a new conversation

Read this file, [Workflow.md](Workflow.md), [CppDesign.md](CppDesign.md), and
[BuildAndTest.md](BuildAndTest.md) before acting. These are the authoritative
coordination documents on TaskWarrior's `doc/Coord` branch, checked out in the
separate working tree `^/coord/`. Source and Terry's newer instructions take
precedence over this handoff. Do not resume an abandoned rebase or automatically
start another cleanup pass; continue with the task Terry requests.

The purpose is to showcase FmtColor's readable, named styles in a colorized
application Terry actually uses. Avoid requiring programmers to consult an SGR
manual. Performance is a possible benefit, not a measured outcome. Describe the
integration without disparaging TaskWarrior's original implementation.

## Document map

| Document | Purpose |
| --- | --- |
| [Workflow.md](Workflow.md) | Authorization, path notation, Git, and working trees. |
| [CppDesign.md](CppDesign.md) | Implemented interfaces, output conventions, and remaining work. |
| [BuildAndTest.md](BuildAndTest.md) | Dependency installation, commands, and validation evidence. |

## Paths

`^` and `taskwarrior:` always mean `~/prj/taskwarrior`, regardless of the current
directory. On this PC that is `C:/msys64/home/tjgolubi/prj/taskwarrior`, or
`/home/tjgolubi/prj/taskwarrior` in MSYS. `.` retains its shell meaning.

- `^/`: TaskWarrior source working tree, branch `FmtColor`.
- `^/coord/`: coordination working tree, branch `doc/Coord`.
- `libshared:`: separate repository/submodule at `^/src/libshared`, branch `MyColor`.
- `fmt:`: sibling repository at `^/../fmt`, currently branch `feat/EmphFactories`.

## Verified current baseline

Inspected October 28 after Terry committed the emphasis-factory integration,
libshared first and then TaskWarrior. TaskWarrior, libshared, and fmt are clean;
this coordination update remains uncommitted on `doc/Coord`. No documentation
commit or push is authorized by this update. The parent repository locally
ignores `/coord/` through `.git/info/exclude`.

| Repository                    | Current HEAD                               |
| ----------------------------- | ------------------------------------------ |
| TaskWarrior, `FmtColor`       | `53edaec240e40f6e523e7d149e5e9d3c876328a0` |
| libshared, `MyColor`          | `c0b0a47f37393eae8fdd4ff0b54fb28f55efa637` |
| fmt, `feat/EmphFactories`     | `11b9f99d27fb61c26728d578f267102761975ac5` |
| Corrosion, detached submodule | `c4786e7ae2d5ade784aca29448df362c874ed04e` |

TaskWarrior's recorded submodule revisions match both checkouts. The parent
commit records the new libshared commit. Corrosion remains at the restored
pre-rebase revision. Both consumer builds were reconfigured against the refreshed
FmtColor installation on October 28; no new rebase was performed.

The TaskWarrior base remains v3.5.0, `3419d5ba1e6a780fbbb0a8d68d8fd31e675c1bf8`,
followed by the MSYS build fix `772905203fac096cbcaaa1aa324688e7826629f6` and
our integration commits. Source changes discussed in the preceding conversation
have been committed by Terry. Do not restart the old fresh-checkout assessment.

## Current implementation and validation

`Color` is a global alias for `fmt::text_style`; the old Color class is gone.
Application helpers are in namespace `Style`: Parse, Spec, strip_sgr, and colorize.
The namespaces and aliases have already been migrated; do not rename them again.

The local fmt installation is `^/build/fmtcolor-install`, refreshed from
`11b9f99d`. It contains conditional emphasis factories in addition to the signed
numeric interfaces, `emphasis::operator|=`, styled ostream insertion, and
assert/clamp constructors. The installed color.h hash matches the sibling source.
Both projects use C++17 and compact SGR mode. TaskWarrior now uses factories for
conditional bold styles and fluent setters for fixed bold colors; libshared's
Color tests demonstrate a mixture of factories, fluent chains, and constructors.
Prefer fluent composition over `|` when all else is equal; see CppDesign.md.

Latest verification, October 28:

- TaskWarrior: all 178 tests passed (31 C++ and 147 script entries).
- libshared Color: all 1,097 assertions passed after the final source edits.
- Earlier full libshared run with the updated library, before the Color test
  construction edits: 11,891 passed, ten failed, four skipped. Failures were the
  seven known emoji checks and three next-weekday date checks. The date failure
  cause was not investigated. The full libshared suite was not rerun after those edits.
- See BuildAndTest.md for logs, source revisions, and verification limits.

## Rebase rehearsal: completed, then deliberately undone

Terry successfully rebased TaskWarrior onto local upstream develop
`c280c347c1adc47e9bb1ad42c92958e2d7f92e9c`. We verified all seven integration
commits survived. Git could not merge the divergent libshared gitlink; selecting
our integration revision preserved the color implementation but initially left
out two upstream libshared commits.

We subsequently rebased libshared's temporary `TryDevelop` onto upstream
`d899eba6d1a863f78f15e5e82d45d857278e9471`. All six local commits were preserved
unchanged by range-diff, with no conflicts. Both projects rebuilt; TaskWarrior
passed 179 tests (32 C++ and 147 scripts), and libshared retained seven emoji
failures. The extra C++ entry belongs to the newer upstream base.

Terry then deleted both TryDevelop branches and restored everything to FmtColor /
MyColor. This was intentional. No active rebase or uncommitted gitlink update
remains. The rehearsal established that a later rebase is feasible; it is not
permission to rebase again. Historical tips, only for recovery/reference:
TaskWarrior `668dd43bc`; rebased libshared
`17a5126966e3f911b8975f65f529b119dbe56c21`.

Upstream libshared commits still absent from the restored MyColor:

- `df59fd69119d81cd320e67d3b868226ce5f1dd2d`: Haiku detection and one
  Haiku-specific Unicode skip, plus general subprocess fixes (retry interrupted
  select/read/write and use waitpid for the specific child). The subprocess
  changes are not restricted to Haiku and may matter on MSYS2 too.
- `d899eba6d1a863f78f15e5e82d45d857278e9471`: removes the Haiku CI job added by
  the first commit. There is no net Haiku CI-job addition.

Haiku is a separate open-source operating system inspired by BeOS. Do not dismiss
both commits as merely Haiku test changes. These upstream commits are not fixes
for our known emoji-width failures.

## Pending decisions and publication

Proposed upstream TaskWarrior work should ultimately be based on develop; their
contribution documentation uses that branch. Terry chose to keep working on the
original branches for now and plans to squash local feature commits before
publishing. Preserve upstream base commits when squashing.

Recommended dependency publication order is fmt, libshared, then TaskWarrior.
No pushes were performed by the agent, and remote publication was not freshly
verified for this handoff. Relevant forks are tjgolubi/fmt, tjgolubi/libshared,
and tjgolubi/taskwarrior. After rewriting/squashing libshared, the parent gitlink
must point to the new commit. Current `.gitmodules` and local submodule URL still
name GothenburgBitFactory/libshared; a fork that references our private-to-fork
commits needs an accessible fork URL before others can initialize it. No URL
change has been made.

A single fmt submodule at the TaskWarrior root (for example external/fmt) was
discussed as a way to make this trial self-contained. It has NOT been added and
CMake still consumes the installed package from the sibling source. Do not assume
that proposal is approved implementation work. Portable acquisition and pinning
remain unfinished.

The attempted split() string_view refactor was fully reverted at Terry's request.
It must be separate future work/commit, not mixed into the color integration.
Optional saved patches are disposable files under build/color-cxx17-check;
see CppDesign.md. There is no pending reminder automation: the integer-interface
reminder was delivered and deleted, and the library team implemented that request.

The next conversation should verify this baseline, then follow Terry's next
requested task. Remaining features and documentation work are tracked in
[CppDesign.md](CppDesign.md#to-do-and-open-questions).

# Workflow

[Document map](README.md) · [Integration design](CppDesign.md) · [Build and test](BuildAndTest.md)

Updated October 28, 2026.

## Authorization and scope

Change things only when Terry uses an imperative instruction. Statements such as
"I want", "we should", and "I plan" express intent, not authorization. Questions
may authorize read-only inspection, not changes. This explicit preference applies
to files, dependency layout, and Git history. Terry repeatedly corrected premature
actions; do not infer edit authorization from an intended outcome. Ask about
meaningful choices such as dependency folder placement before implementing them.
Terry prefers ext, not external. Read-only inspection needed to answer is allowed.

A request to review or check a file authorizes inspection, not fixes or an
unsolicited build/test run. Explain findings and leave the source unchanged.
Re-read saved files: Terry edits concurrently and sometimes has unsaved changes.
Preserve review pauses. Announcing an expansion of scope does not authorize it;
report unrelated build problems instead of fixing them incidentally.

Explain an authorized change before editing, keep it focused, and verify it.
When Terry fixes an error blocking an already-requested build/test and says
"Fixed", resume that authorized operation. Generating a commit message only
means supplying text. Terry normally commits and pushes himself.

In this project's code and examples, prefer fluent style composition over `|`
when all else is equal. Constructors and enum combinations remain appropriate
where clearer; do not mechanically replace them. See CppDesign.md for examples.

Commits, pushes, merges, branch switches, rebases, deletions, stashes, and network
fetches need authorization for the relevant operation. Editing permission alone
does not grant it. Do not create or message another chat without his request.
Do not send a library-team wish-list just because its contents are documented.

## Paths and coordination working tree

In discussion, `^` and `taskwarrior:` mean the TaskWarrior root,
`~/prj/taskwarrior`. `libshared:` means `^/src/libshared`; `fmt:` means the
sibling `^/../fmt`. Expand these to real paths in executable commands; they are
not shell syntax. `.` is the shell's current working directory.

The source tree at `^/` is on color-dev. Coordination documents are on doc/Coord
in the separate working tree `^/coord/`. Each worktree has its own checkout and
index while sharing repository history. Changing directories changes which
worktree Git reports; it does not switch the other worktree's branch.

Never put coordination documents or persistent worktrees under `^/build/`.
Terry routinely deletes that directory for clean rebuilds. The original temporary
coordination worktree was moved to coord; do not create a competing copy.
The old root FmtColorTODO.md is gone; use CppDesign.md's to-do list.

Review documentation with `git -C coord status` and `git -C coord diff` from `^/`.
These edits remain uncommitted until Terry commits on doc/Coord. `/coord/` is now
in the parent repository's local `.git/info/exclude`; it is not a tracked source
change or a submodule. Do not add the nested worktree to a TaskWarrior commit.

## Git and dependency boundaries

TaskWarrior and libshared are separate repositories. Commit libshared changes
separately, then record its resulting commit in the parent gitlink when instructed.
`git commit -a` picks up changed tracked gitlinks but does not commit the
submodule's dirty files or add untracked files.

The real rebases and squashes have now been completed on color-dev in both
repositories. Earlier FmtColor/MyColor and TryDevelop states are historical.
Current revisions are in README.md. Inspect git submodule status --recursive
before switching or updating: a parent branch switch does not automatically align
submodule checkouts, and update checks out the parent's recorded revisions.
Do not silently replace newer submodule work with an older staged gitlink.

A stale empty `.git/index.lock` previously blocked commits and was removed only
after inspection and Terry's approval. It is not a current blocker. A reboot
alone does not delete a stale file; never assume a lock is stale while Git or a
commit editor is using it.

FmtColor is maintained in the separate fmt project. Its source, release policy,
user guide, website, and docs/Coord branch belong there. Report missing library
capabilities rather than implementing them incidentally here. Use exact source
and selected dependency revisions; the library version or branch name does not identify
our custom API sufficiently.

Both consumers now build the nested fmt submodule at src/libshared/ext/fmt.
Libshared owns that submodule, pinned to a commit from tjgolubi/fmt color-dev.
TaskWarrior also adds it explicitly because it compiles selected libshared
sources without invoking libshared's top-level CMake. No local installation is
needed. The sibling ../fmt checkout remains separate development work.
The committed libshared URL points to Terry's fork. Terry reported everything
pushed; this does not authorize additional pushes or rewriting published history.

## Tools and verification

Use MSYS2 Git (`C:/msys64/usr/bin/git.exe` or `/usr/bin/git`), not native Windows
Git: worktree metadata may contain MSYS paths. Its Windows shared-memory startup
may need escalated execution in the tool runner. Do not change global settings
to work around sandbox limitations.

Use MSYS tools and objects, not UCRT64 or MSVC. Do not reinstall UCRT64. Preserve
source line endings; newly written coordination documents use LF. Use
`bash --noprofile --norc` for automation because login startup runs unrelated
personal jobs. Set PATH for the command rather than globally, including
`/home/tjgolubi/.cargo/bin` when building TaskWarrior.

Use isolated test data/configuration and the documented workspace-local TMPDIR.
Do not test against Terry's actual task database or install over his normal task
command. Run TaskWarrior C++ tests serially; script tests can run concurrently.
Check whole logs before claiming completed tests all pass, not just the tail.
Report known failures honestly; ignore-for-now is not permission to disable them.

Changing ../fmt does not change the pinned ext/fmt checkout. Update the dependency
revision only when instructed, and record the resulting gitlink in libshared and
then TaskWarrior as appropriate. Reconfigure when build definitions or dependency
paths change, rebuild consumers, and test the actual selected revision. A library
CI pass is not application validation. No fmt_DIR or refresh of the old installed
package is needed for current builds.

## Handoff

Update these documents when instructed, replacing stale information instead of
appending a chat transcript. Distinguish proposed work, source edits, commits,
pushes, local installation, and fresh versus historical tests. No new source
work, Git publication, or future rebase is authorized by this documentation update.

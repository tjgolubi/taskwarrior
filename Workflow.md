# Workflow

[Document map](README.md) · [Integration design](CppDesign.md) · [Build and test](BuildAndTest.md) · [Documentation](Website.md)

Updated October 27, 2026.

## Authorization and scope

Modify files when Terry gives an implementation instruction, normally in the
imperative mood. Questions about capability and statements such as "we should"
or "I am considering" are discussion, not edit authorization. Terry has
repeatedly corrected premature edits in this chat. Do not treat every "can you"
question as permission; explicit contextual authorization, such as "I'm asking",
can make the request clear. Read-only inspection needed to answer is allowed.

A request to review or check a file authorizes inspection, not fixes or an
unsolicited build/test run. Explain findings and leave the source unchanged.
Re-read saved files: Terry edits concurrently and sometimes has unsaved changes.
Preserve review pauses. Announcing an expansion of scope does not authorize it;
report unrelated build problems instead of fixing them incidentally.

Explain an authorized change before editing, keep it focused, and verify it.
When Terry fixes an error blocking an already-requested build/test and says
"Fixed", resume that authorized operation. Generating a commit message only
means supplying text. Terry normally commits and pushes himself.

Commits, pushes, merges, branch switches, rebases, deletions, stashes, and network
fetches need authorization for the relevant operation. Editing permission alone
does not grant it. Do not create or message another chat without his request.
Do not send a library-team wish-list just because its contents are documented.

## Paths and coordination working tree

In discussion, `^` and `taskwarrior:` mean the TaskWarrior root,
`~/prj/taskwarrior`. `libshared:` means `^/src/libshared`; `fmt:` means the
sibling `^/../fmt`. Expand these to real paths in executable commands; they are
not shell syntax. `.` is the shell's current working directory.

The source tree at `^/` is on FmtColor. Coordination documents are on doc/Coord
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

Both TryDevelop branches were deliberately deleted after a successful rebase
rehearsal. FmtColor / MyColor are the current baseline again; exact revisions and
rehearsal results are in README.md. Do not recreate, rebase, or merge automatically.
On any branch restoration, inspect `git submodule status --recursive`: changing
the parent branch does not necessarily restore the checked-out submodule commits.
Corrosion was restored to c4786e7ae, then CMake was regenerated and tests rerun.

A stale empty `.git/index.lock` previously blocked commits and was removed only
after inspection and Terry's approval. It is not a current blocker. A reboot
alone does not delete a stale file; never assume a lock is stale while Git or a
commit editor is using it.

FmtColor is maintained in the separate fmt project. Its source, release policy,
user guide, website, and docs/Coord branch belong there. Report missing library
capabilities rather than implementing them incidentally here. Use exact source
and installed revisions; the library version or branch name does not identify
our custom API sufficiently.

The current dependency is still a locally installed package from ../fmt. A root
fmt submodule was discussed but not added. Publishing requires accessible
library commits and correct submodule URLs; see README.md. Preserve MyColor and
FmtColor history until Terry explicitly chooses the next Git operation.

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

Changing a library header in ../fmt does not update installed headers. Refresh
build/fmtcolor-install before testing new APIs. Reconfigure when requested or
when build definitions change, and rebuild consumers. Record which installed
revision was actually tested. A library CI pass is not application validation.

## Handoff

Update these documents when instructed, replacing stale information instead of
appending a chat transcript. Distinguish proposed work, source edits, commits,
pushes, local installation, and fresh versus historical tests. No new source
work, Git publication, or future rebase is authorized by this documentation update.

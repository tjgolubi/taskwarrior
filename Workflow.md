# Workflow

[Document map](README.md) · [Integration design](CppDesign.md) · [Build and test](BuildAndTest.md) · [Documentation](Website.md)

Updated October 23, 2026.

## Authorization and scope

Modify project files only when Terry instructs us in the imperative mood.
Questions and statements of intent are discussion, not edit authorization.
Read-only inspection and checks needed to answer a prompt are allowed.
Explain proposed changes before editing, keep each increment focused, verify
it, and review it before proceeding. Preserve explicit stopping points.

The current instruction authorizes adapting these coordination documents. It
is not authorization to implement the integration, establish a new checkout,
change dependencies, or modify TaskWarrior or FmtColor source.

Commits, pushes, merges, branch switches, rebases, deletions, and stashes require
authorization for the relevant operation. Terry normally performs the final
commit and push. Editing permission does not imply permission for those actions.
Do not create a new Codex chat or message another chat without Terry's request.

## Project and dependency boundaries

TaskWarrior is the application under development. Use a fresh baseline from
`upstream/develop`, as approved by Terry. Verify the remote URL and full SHA
before beginning; fetching or creating/switching branches requires authorization.
Do not base the project on local `fix/Ucrt64` changes or the inapplicable
`TjgColor` material. Preserve the old checkout and its unrelated files.

Inspect TaskWarrior's own repository instructions. Identify submodules before
editing: a change inside a submodule belongs to a separate repository and may
require both a submodule commit and a parent gitlink update. Do not assume all
color implementation changes belong directly to the TaskWarrior repository.

FmtColor is an external dependency pinned to a full commit, initially proposed
in [README.md](README.md#starting-references). Its source, tests, user guide,
release history, and website belong to its own project. A missing capability
is a finding to discuss, not permission to change that dependency. If a library
change is separately authorized, coordinate it there and explicitly review any
new application pin.

These coordination documents govern this integration project. FmtColor's
squash policy, changelog generation, release process, and full library CI matrix
do not automatically govern TaskWarrior work. Follow the applicable application
repository instructions and the agreed integration scope.

## Before changes

Inspect branch, full commit, working tree, submodule state, relevant source,
and repository instructions. Resolve unrelated tracked changes with Terry;
do not discard or stash them automatically. Re-read files changed concurrently.
Preserve untracked experiments, scripts, archives, and logs; mention them only
when they affect the task or risk accidental inclusion. Exclude TjgColor from
the integration without deleting it.

Use MSYS2 Git (`C:\msys64\usr\bin\git.exe` from PowerShell, `/usr/bin/git`
in MSYS) and verify executable provenance. The inherited FmtColor environment
uses MSYS tools; UCRT64 was removed at Terry's request. Do not reinstall it or
revive the old TaskWarrior UCRT64 fixes as incidental setup work. Establish the
actual TaskWarrior toolchain requirements before configuring a build.

Reuse existing SSH agents, keys, identity, and configuration. Keep credentials
out of files, logs, tool output, and handoffs. Do not change global environment
or toolchain settings to work around a local build problem.

## Implementation and validation

After the assessment is reviewed and implementation is authorized, propose a
focused branch name and confirm the exact baseline before creating it. Keep
changes minimal and follow the affected repository's style. Preserve existing
line endings; new text files use LF.

Document the dependency pin, acquisition method, CMake integration, and linkage
mode so another checkout can reproduce the build without relying on a sibling
FmtColor tree or a developer's global include paths. An explicit local source
override may aid development, but must not silently select another revision.

Preserve valid configuration and agreed application semantics. Discuss behavior
changes and unsupported cases before implementation. Run appropriate baseline
and changed-code checks; do not claim an integration pass from library CI alone.
Use isolated test data and configuration, never Terry's real task database.

Do not invent a required TaskWarrior CI matrix from the FmtColor workflow.
Inspect actual workflows and repository requirements. Record full SHAs, jobs,
results, and skips for any CI evidence. Changed commits require fresh checks
before citing earlier results as current. Do not push or trigger publication
workflows without authorization.

## Task completion and handoff

Update affected coordination documents when instructed as part of task closure.
Replace superseded decisions instead of appending a diary. Record:

- The accepted application baseline and dependency pin.
- Implemented scope and compatibility decisions, separated from proposals.
- Relevant build/test commands, observed results, and limitations.
- Remaining questions and a concrete next task for a new chat.

Distinguish local edits, commits, pushes, and published results. Another machine
cannot read unpushed changes from GitHub. Do not record proposed work as finished
or historical library validation as application validation.

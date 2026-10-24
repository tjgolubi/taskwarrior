# TaskWarrior / FmtColor coordination

Updated October 23, 2026.

## Purpose

Evaluate and integrate FmtColor into TaskWarrior as a third-party dependency,
replacing TaskWarrior's existing color handling while preserving valid user
configuration and agreed application behavior. This is a TaskWarrior-centered
project, separate from development of FmtColor itself.

Each chat addresses a focused task. These documents supply its starting context,
accepted decisions, working rules, validation, and remaining questions. Update
them when completing a task so the next chat can begin from the current state.
Keep a coherent account rather than appending a conversation or event log.
Source and tests establish implemented behavior; newer explicit instructions
from Terry override older wording. Proposals are not implementation permission.

## Document map

| Document | Purpose |
| --- | --- |
| [Workflow.md](Workflow.md) | Authorization, repositories, Git, and handoff rules. |
| [CppDesign.md](CppDesign.md) | Integration boundaries, dependency API, and assessment questions. |
| [BuildAndTest.md](BuildAndTest.md) | Baseline discovery, build strategy, and validation requirements. |
| [Website.md](Website.md) | Documentation scope and external project references. |

## Accepted starting decisions

- Center the new Codex project on the TaskWarrior repository.
- Start fresh from TaskWarrior's `upstream/develop`. Terry approved this ref
  after initially referring to upstream master. Do not continue the existing
  local `fix/Ucrt64` work or use its patches as the integration baseline.
- Existing `TjgColor` material is not applicable. Do not adopt, repair, or migrate
  it. Preserve existing files and unrelated work when establishing the fresh
  checkout; starting fresh does not authorize deletion or resetting that work.
- Treat FmtColor as a third-party library with an explicit pinned revision.
  Initially target the {fmt}-based implementation, not the future independent
  C++23 library. The proposed initial pin is recorded below; dependency packaging
  and acquisition remain to be agreed.
- Begin with a read-only assessment and review its proposal before implementation.
- Keep TaskWarrior integration changes separate from FmtColor development.
  Report missing library capabilities instead of silently extending FmtColor.

## Starting references

These documents were adapted on `ForTaskWarrior`, split from the FmtColor
coordination branch `docs/Coord` at `4f256561`. They are coordination material,
not TaskWarrior or FmtColor product source. Their eventual home in the new
project remains to be settled; do not create competing authoritative copies.

TaskWarrior's starting ref is `upstream/develop`; its exact full SHA and remote
URL must be verified and recorded at project setup. Existing local refs are
not proof of the latest upstream state. Do not assume that the old local build,
source inspection, or tests describe the fresh baseline.

Proposed initial FmtColor dependency:

- Repository: [tjgolubi/fmt](https://github.com/tjgolubi/fmt).
- Development line: `FmtColorDev`.
- Pin: `d9d64d361591311901e06c662c24879128c63724`.
- Header/API: `<fmt/color.h>`, namespace `fmt`.
- Implementation: {fmt} with FmtColor extensions; supports C++11.

The upstream {fmt} baseline for that dependency is
`0fc31633e7d53ac895ae0a688b83b7d44f0b0c1f`. A moving branch name or inherited
`FMT_VERSION` does not uniquely identify the FmtColor feature set. Do not choose
`test/UpstreamCompatibility` as the application dependency.

## Current status and next task

No TaskWarrior integration has been implemented or validated by this project.
Only the coordination documents have been adapted. Local observations from the
previous checkout are discovery leads, not accepted baseline findings.

The first task is a read-only assessment against the selected upstream commit:

1. Establish repository/submodule ownership, clean starting state, and build inputs.
2. Trace color storage, parsing, composition, rendering, callers, and tests.
3. Identify persistence or numeric-representation dependencies and valid theme syntax.
4. Compare application behavior with the pinned FmtColor API and list gaps.
5. Propose one small integration increment with affected files, dependency setup,
   compatibility decisions, and tests; review it with Terry before editing code.

A compatibility adapter retaining TaskWarrior's existing `Color` interface and
parser is an initial hypothesis, not a chosen design. The assessment may suggest
a better boundary. Do not assume FmtColor's value-level OR or averaging matches
TaskWarrior's current blending policy.

# Documentation and external references

[Document map](README.md) · [Workflow](Workflow.md) · [Integration design](CppDesign.md) · [Build and test](BuildAndTest.md)

Updated October 23, 2026.

## Scope

This project currently has no integration website or publishing task. This file
retains its inherited name to preserve the coordination document map; it now
records documentation boundaries and reference locations.

Keep TaskWarrior integration instructions, dependency setup, supported behavior,
and validation with the application project. Update user-facing color/theme
instructions only when the agreed integration changes require it. The first
assessment should read existing application documentation before proposing
syntax changes or describing behavior as compatible.

The FmtColor website, generated guide, changelog, release process, and source
belong to the separate library project. Do not update them as incidental work
for TaskWarrior. Missing capabilities should be recorded in the assessment and
discussed separately. This document adaptation authorizes no publication.

## References

- [TaskWarrior upstream repository](https://github.com/GothenburgBitFactory/taskwarrior):
  use the approved `upstream/develop` ref and verify the actual configured remote
  and full commit when setting up the new project.
- [FmtColor source at the proposed pin](https://github.com/tjgolubi/fmt/tree/d9d64d361591311901e06c662c24879128c63724):
  implementation and tests establish the dependency API.
- [FmtColor generated user guide](https://tjgolubi.github.io/fmt/color/index.html):
  API references, examples, palette conversions, and behavior notes. It is a
  moving publication; verify details against the pinned source when they matter.
- [FmtColor project website](https://fmtcolor.ardentokapi66.chatgpt.site):
  motivation, contribution, compatibility evidence, and future design. Future
  work and downloadable demos must not be mistaken for implemented capabilities.
- FmtColor coordination branch `docs/Coord`: the library's own workflow, design,
  build, and publication context. This project's `ForTaskWarrior` adaptation
  does not replace or modify that branch's library guidance.

Use the generated guide for user-facing reference links; GitHub does not render
its source Markdown's MkDocs directives. Prefer exact commit links for code and
validation claims. Do not infer upstream acceptance or a numbered FmtColor
release from integration on `FmtColorDev`.

## Handoff documentation

At task completion, keep the coordination documents consistent with the actual
application/dependency revisions and observed checks. State what remains a
proposal. Document compatibility decisions and any application-specific adapter
logic so a new chat can proceed without reconstructing earlier discussions.

No TaskWarrior assessment report, accepted implementation design, or integration
validation exists yet. The first report should be grounded in the fresh upstream
baseline and should exclude the old TjgColor material.

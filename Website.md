# Documentation and external references

[Document map](README.md) · [Workflow](Workflow.md) · [Integration design](CppDesign.md) · [Build and test](BuildAndTest.md)

Updated October 27, 2026.

## Scope and ownership

The authoritative TaskWarrior coordination documents are on doc/Coord in
`^/coord/`. This project has no website implementation or publication task.
Website.md retains its inherited name and records documentation boundaries.
The to-do list belongs in CppDesign.md; do not recreate FmtColorTODO.md.

Keep application integration instructions, dependency setup, and validation
with TaskWarrior. FmtColor's source, tests, generated guide, releases, website,
and library coordination branch docs/Coord belong to its separate project.
Do not apply that project's release/squash/CI policies automatically here.

The current Color alias, Style helper namespace, added emphasis keywords and
canonical order, signed numeric interfaces, and styled stream output are
implemented. User-facing TaskWarrior documentation has not yet been comprehensively
updated to describe the new color syntax. In particular, do not describe the
implemented faint/italic/strike support as merely a proposal. Blink, conceal,
and the parser spelling strikethrough remain future decisions.

Use neutral language: this integration showcases readable named colors and
attributes in an application Terry uses. No measured speedup or upstream
acceptance has been established. Rebase feasibility and local test passes are
not claims that a pull request has been accepted or that CI has run.

## References

- [TaskWarrior upstream](https://github.com/GothenburgBitFactory/taskwarrior).
  Current integration base and historical develop inspection are distinguished
  in README.md. Proposed contributions should target develop, per its
  [branching model](https://github.com/GothenburgBitFactory/taskwarrior/blob/develop/doc/devel/contrib/branching.md).
  Terry deliberately restored the original feature branches after a successful
  rehearsal; do not automatically rebase them again.
- [FmtColor at the installed revision](https://github.com/tjgolubi/fmt/tree/eb0444023168fbe79f68f2c032bf1ca67ebd47b8).
  The exact revision exists locally; remote publication must be checked before
  relying on a GitHub link from another machine. Inspect ../fmt and the installed
  headers for authoritative API details. Styled stream insertion and ostream
  print/println are in fmt/color-ostream.h, not color.h alone.
- [FmtColor generated user guide](https://tjgolubi.github.io/fmt/color/index.html).
  This moving publication is appropriate for user-facing library documentation;
  exact source revisions support implementation claims. GitHub does not render
  all MkDocs directives in the guide's source Markdown.
- [FmtColor project website](https://fmtcolor.ardentokapi66.chatgpt.site).
  Proposed designs do not establish implemented APIs.
- [Haiku introduction](https://www.haiku-os.org/about/).
  Haiku is an open-source operating system inspired by BeOS. The upstream
  libshared support commit also contains platform-independent subprocess fixes;
  see README.md rather than assuming all its changes affect only Haiku.
- [Xterm control sequences](https://invisible-island.net/xterm/ctlseqs/ctlseqs.html)
  and [mintty control sequences](https://github.com/mintty/mintty/wiki/CtrlSeqs).
  OSC 10 and 11 query terminal default foreground/background. That proposal is
  separate from SGR styling and has not been integrated into TaskWarrior.

## Next documentation work

When authorized, document supported parser spellings and canonical output,
independent foreground/background brightness, and the distinction between
TaskWarrior's rgbRGB cube syntax and 24-bit RGB. Keep examples readable without
requiring the programmer to consult the SGR manual.

Before publishing or submitting upstream, settle dependency acquisition and
submodule URLs and commit the coordination updates separately. This handoff
update performs no commits, pushes, publication, or library changes.

# TaskWarrior integration design

[Document map](README.md) · [Workflow](Workflow.md) · [Build and test](BuildAndTest.md) · [Documentation](Website.md)

Updated October 23, 2026.

## Goal and boundaries

Replace TaskWarrior's color handling using FmtColor as a third-party dependency.
Start fresh from `upstream/develop`; ignore the inapplicable TjgColor material.
Preserve valid themes, configuration, and agreed rendering behavior. Do not
expand this into a general formatting rewrite or a redesign of task storage.

No adapter, representation, dependency packaging method, or implementation
increment has been accepted yet. The first task is assessment, not coding.
Retaining the application's `Color` interface and parser while replacing internal
style storage and ANSI generation is a hypothesis to evaluate.

## Assessment against the fresh baseline

Trace the actual source and callers, recording file and symbol references:

- Where color code lives, including submodule ownership and build targets.
- Color constructors, conversions, getters, equality, and numeric masks.
- Theme/configuration parsing, accepted color names and forms, error behavior,
  and conversion back to strings. Separate style syntax from ANSI parsing.
- Foreground/background presence, unspecified versus black, brightness versus
  bold, underline, reverse, resets, and disabled-color behavior.
- Blending and precedence when rules, rows, columns, and reports overlap.
- Palette upgrades, truecolor conversion, terminal capability selection, and
  output/redirect behavior.
- ANSI generation, embedded escapes, stripping, nesting, reset/restoration,
  visible width, truncation, and alignment.
- Persistence, exports, caches, numeric conversions, or other code that depends
  on the existing packed representation. Persistence has not been established.
- Existing unit and CLI tests, documentation, and representative themes.

Create a mapping from each required application operation to FmtColor support,
application adapter logic, or a capability gap. Distinguish current behavior,
intentional limitations, bugs, and proposed changes. Do not import conclusions
from the old local checkout without verifying them against the selected SHA.

## FmtColor capabilities at the proposed pin

The dependency is the {fmt}-based implementation at
`d9d64d361591311901e06c662c24879128c63724`, not the planned independent
C++23 library. Confirm relevant details in that revision's source and tests
before implementing the adapter.

- `<fmt/color.h>` supplies namespace `fmt`, `text_style`, public `color_t`,
  `term_color`, `rgb`, `cube_color`, `gray_color`, and `no_color`.
- `color_t` has five alternatives: unspecified, RGB, terminal, cube, and gray.
  C++11 type/index access, `get_if`, `holds_alternative`, and visitation are
  available. Do not borrow a reference from a temporary style getter.
- `term_color` uses indices 0-15, not SGR numbers. Cube channels are 0-5;
  indexed output is 16 plus cube index. Gray indices are 0-23; indexed output
  is 232 plus gray index. RGB construction from a packed integer is explicit.
- `no_color` means unspecified, distinct from black. It is not a terminal-reset
  command. Palette-to-RGB conversion uses fixed Xterm defaults, not a query of
  the user's terminal theme.
- Styles support foreground-first and emphasis-first constructors, fluent
  `fg()`, `bg()`, `emph()`, component presence queries, attribute controls,
  `empty()`, and `clear()`. Setters replace the selected component.
- Bold and faint are independent. The {fmt}-based implementation retains conceal
  and omits flash. Use `term_color` and `emphasis::strike` in new integration code.
- Color/style OR promotes two specified colors to RGB and combines channel bits;
  unspecified is identity. This is not sequential SGR state assignment.
- Free `blend()` averages specified color channels equally, rounding down, and
  ORs style emphasis. One specified color retains its kind. The old mutating
  `rgb::blend()` member is absent. Match application behavior explicitly rather
  than substituting this function based on its name.
- Styled formatting and ANSI output exist. Current packed color/style values
  and hashes are not stable serialization formats or binary contracts.

Named RGB constants, palette support, and style composition need not all be
exposed in TaskWarrior configuration in the first increment. Preserve the
application's supported syntax first; discuss new vocabulary separately.

## Capability gaps and compatibility decisions

Structured styled text, absolute boundaries, lazy nested blending, a general
style/ANSI parser, display-width handling, and serialization remain future
FmtColor work. Do not plan an application increment that silently assumes they
exist. Keep needed application logic or report the gap for separate discussion.

ANSI command interpretation updates state sequentially; FmtColor value OR and
averaging are separate operations. Resetting terminal state, restoring enclosing
styles, and clearing an in-memory style also have different effects. Establish
the adapter's responsibility for each.

Decide whether exact escape bytes are contractual for any tests or consumers.
Visually equivalent output may be acceptable, but this has not been approved
as a blanket reason to change existing tests. Record intentional output changes
with concrete before/after examples and justification.

Do not carry TaskWarrior's packed flags directly into FmtColor or serialize
FmtColor's private layout. If legacy numeric interfaces are required, design
explicit translation at the application boundary. Check valid configuration
round trips and distinguish unspecified channels from specified black.

## Expected assessment result

Present a proposed integration boundary, behavior mapping, library gaps,
repository/submodule changes, dependency setup, and a small first increment.
Include affected files and tests, unresolved compatibility choices, and baseline
build findings. Terry reviews that proposal before implementation begins.

The long-term goal is replacement of existing color handling; a narrow first
increment is a way to validate the boundary, not a declaration that the full
migration is complete.

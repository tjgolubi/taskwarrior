# TaskWarrior integration design

[Document map](README.md) · [Workflow](Workflow.md) · [Build and test](BuildAndTest.md)

Updated October 28, 2026.

## Implemented integration and scope

TaskWarrior uses the {fmt}-based FmtColor extension from the sibling fmt project,
installed locally at revision `11b9f99d27fb61c26728d578f267102761975ac5`
from branch `feat/EmphFactories`.
This is not the proposed independent C++23 library. Application and standalone
libshared builds use C++17. Exact repository revisions are in README.md.

The old Color class has been removed. libshared's Color.h now provides:

```cpp
using Color = fmt::text_style;

namespace Style {
  auto Parse(const std::string&) -> fmt::text_style;
  auto Spec(const fmt::text_style&) -> std::string;
  auto strip_sgr(std::string_view) -> std::string;
  // Also: colorize(text, specification), returning an owning std::string.
}
```

Color is exactly the FmtColor type, not a wrapper and not integer-backed public
compatibility. Style is the helper namespace. Earlier local Style aliases were
temporarily renamed TjgStyle and then removed in favor of Color. The source-wide
type replacement excluded Color.h and Color.cpp intentionally. Do not repeat it.

Style::colorize combines Parse with fmt::wrap_sgr. Tests compare styles and output,
not integral casts. No application code should depend on the private packed layout.

The approved formatting migration is limited to files that already use color
features, including actual style use through headers. Merely including Color.h
is not sufficient. Do not introduce fmt into unrelated files or start a general
formatting cleanup without Terry's instruction.

## Style construction and composition

- Terminal color indices are 0-15, not SGR codes. Foreground and background may
  independently use terminal, cube, gray, or 24-bit RGB representations.
- TaskWarrior's `rgbRGB` spelling means a 6x6x6 indexed cube, components 0-5.
  Its palette index is 16 + 36*r + 6*g + b. `fmt::cube_color{0,0,0}` corresponds
  to rgb000 and palette entry 16; it is not the 24-bit RGB representation.
  Eval.cpp uses the local alias Cube, replacing the misleading Rgb alias.
- Gray indices are 0-23, corresponding to palette entries 232-255. RGB truecolor
  specifications use 0xRRGGBB; fmt::rgb is the 24-bit representation.
- `fmt::no_color` means an unspecified channel, not black and not a reset.
  `Color{}` represents an empty whole style. Use Color{} for Table defaults and
  empty cells: implicitly converting no_color to color_t and then text_style
  would require two user-defined conversions, which C++ disallows.
- Terminal foreground and background brightness are independent and not sticky.
  Brightness is not bold. The parser's on separates channels; pending foreground
  brightness does not migrate into the background.
- Member style.overlay(overrides) mutates the style: emphasis is ORed and incoming
  specified colors replace the corresponding existing channels. Free
  fmt::overlay(base, overrides) returns a changed copy. No promotion to RGB or
  averaging is required. Do not substitute color bitwise OR for this policy.
- Fluent emphasis setters mutate and return a reference. Each free emphasis
  helper now has a factory overload taking an optional bool (default true):
  `fmt::bold()` creates a bold-only style, and `fmt::bold(false)` creates an empty
  style. The overload taking a text_style returns a changed copy, useful with
  const base styles; util.cpp retains `fmt::underline(header_style)`.
  fmt::emphasis supports operator|=, used by Parse.

Prefer fluent composition over `|` in this project's code and examples when
all else is equal. For example, `fmt::fg(Term::white).bg(Term::red).bold()` and
`fmt::italic().faint().reverse()` read naturally. Enum combinations and
constructors may be clearer for some cases; preserve that variety in Color
tests. `using enum fmt::emphasis` would make enum expressions concise, but it
requires C++20 and is not available in the current C++17 builds. Keep factory
calls qualified with fmt:: when using enumerator names in newer-language examples.

TaskWarrior's conditional bold styles in CmdDiagnostics, CmdHistory, CmdTags,
and CmdVersion now use `fmt::bold(Context::getContext().color())`. CLI2's dump
title and CmdNews's unconditional heading use `fmt::bold()`. CmdColor's three
constexpr bold primary-color styles use `fmt::fg(Term::red).bold()` and the
corresponding green/blue forms. NewsItem::render deliberately retains its
default-constructed bold/underline styles and member setters inside the existing
color-enabled branch after Terry's review. Do not reapply the earlier proposal
to move those initializers outside that branch. Table's test single_cell style
was discussed but not changed.

libshared's Color test expected values now use single-attribute factories and
selected fluent chains alongside existing constructors. Parser inputs and all
1,097 assertions remain intact. Parse's emphasis accumulator was not refactored.
These changes are committed in TaskWarrior `53edaec24` and libshared `c0b0a47f3`.

The current text_style is one uint64_t; fluent emphasis setters update its bits
without allocation and return text_style&. A chain on a factory temporary is
copied into the owning expected value within the same full expression.
Constant arguments allow compiler folding, and constexpr initialization requires
constant evaluation; an ordinary initializer does not require it merely because
the functions are constexpr. No assembly comparison or runtime speedup was measured.

## Numeric interfaces and parser validation

FmtColor now accepts signed int in most public numeric color interfaces, avoiding
many casts. One deliberate remaining exception is the single-index cube_color
constructor, which takes unsigned. Color.cpp's validated indexed-color helper
uses `static_cast<unsigned>(index - 16)` for that overload. Cube component and
gray constructors take int; CmdColor's corresponding loop counters are int.
The remaining unsigned packed RGB value is a representation, not a cube component.
Hex parsing now uses strtoul followed by the existing maximum-value check.

Color constructors are noexcept. Invalid arguments assert when assertions are
enabled, with clamping when assertions are disabled. Do not depend on exceptions
or unconditional silent clamping. Parse retains its explicit range checks so
its existing invalid-range errors remain application policy, independent of the
library constructor behavior.

## Human-readable specifications

Use Style::Parse / Style::Spec for TaskWarrior configuration and command-line
syntax. This parser is not an SGR escape-sequence parser.

Implemented emphasis keywords:

| Accepted spelling | Canonical Spec spelling |
| ----------------- | --------- |
| strike            | strike    |
| underline         | underline |
| faint, dim        | faint     |
| bold              | bold      |
| italic            | italic    |
| inverse, reverse  | inverse   |

Canonical order is **strike underline faint bold italic inverse**, followed by
foreground and then `on background`. This is human-readable order, not SGR
numeric order. The regression suite covers aliases, uppercase input, repetition,
all six attributes together, colors, canonical order, and round trips. All 1,097
Color assertions pass. The parser does not currently accept blink, conceal,
or the spelling strikethrough, even though the library has additional attributes.
User-facing TaskWarrior documentation still needs updating.

Fixed production styles normally use Color constructors, fg/bg helpers, and
emphasis setters rather than parsing literal specifications. Meaningful examples
may deliberately share one specification between parsing and displayed command
text. CmdColor does this for its six sample commands and Effects labels; the two
green-background Effects examples explicitly overlay black foreground.

Basic swatches use parallel name/color arrays and two loops, ordered black, red,
blue, green, magenta, cyan, yellow, white. Their background row uses white text
before green and black text from green onward. Cube/gray/indexed swatches use
direct constructors rather than generating specifications for parsing.

initializeColorRules() parses configured `color.*` values into its private gsColor
map when color is enabled. Some commands still parse configuration separately.
A shared cache accessor is a future possibility, not an implemented redesign.

## Formatting and stream output

Use fmt/color.h for styles and fmt/color-ostream.h for styled stream insertion
and styled ostream print/println. The installed header supports narrow and wide
streams. Preserve the existing destination: std::cout, std::cerr, or the command's
stringstream. Unqualified print/println default to stdout and are not replacements
for a stream argument. Do not mix output destinations incidentally.

```cpp
out << fmt::styled(text, style) << '\n';
fmt::print(out, "Result: {}", fmt::styled(value, style));
fmt::println(out, style, "Result: {}", value);
```

styled() takes value first, style second. wrap_sgr takes style first, text second.
The returned styled_arg borrows its value; immediate insertion of literals and
same-expression temporaries is safe. Do not retain a styled view past its value's
lifetime. Stream insertion uses the value's ostream operator; formatting through
fmt uses its formatter, so custom-type support is not automatically identical.

CmdColor and CmdDiagnostics now stream styled values instead of allocating
wrap_sgr strings for those insertions. CmdShow uses styled values as formatting
arguments. The remaining scan found 50 wrap_sgr calls in 11 files: they return,
store, concatenate, or pass owning strings to string-taking functions. They are
not direct ostream insertions. Do not mechanically replace them with a borrowed
styled_arg. Eval's token rendering and the Table/Column/Palette tests are examples.

wrap_sgr returns already-formed text wrapped in SGR; it does not interpret embedded
escapes. Empty text returns an empty string, and empty styles return unchanged
text. A reset restores terminal defaults, not a parent style. Styled ostream
insertion of a nonempty style writes a prefix and reset even for empty text,
so it is not universally byte-equivalent to wrap_sgr. Current converted labels
and swatches are nonempty. Use fmt::format when actual substitution is needed.

CmdNews uses ostream print/println and explicitly flushes its prompt before
reading. println writes a newline after the style reset; it does not guarantee
flushing. It still converts Version to std::string for fmt formatting, because
Version has ostream insertion but no fmt formatter in this project.

Legacy format(pattern, args...) starts at {1}; fmt starts at {0}, with automatic
{} available. Explicit and automatic indexing cannot be mixed. The legacy helper
does not interpret {} at all. Keep numeric format(value), format(value, width,
precision), and independent helpers such as formatBytes, formatTime, printable,
and obfuscateText where appropriate.

Keep leftJustify/rightJustify where already used: they measure terminal-visible
UTF-8 text while skipping SGR; fmt alignment is not equivalent for all input.
Blank padding can use format_to with a dynamic width. Dashed rules must use '-'
fill, e.g. "{:-<{}}". format_to takes an output iterator such as
`auto append = std::back_inserter(out);`, not the string directly.

TaskWarrior disables color when stdout is redirected. Deliberate SGR capture is
`task rc._forcecolor:on color rgb123 > task.txt`. Terry manually confirmed bright
foreground/background and bold with indexed colors in his terminal. The obsolete
CmdNews double-wrapping workaround has been removed.

## FmtColor wish-list status

- [x] Prefer signed numeric interfaces where practical. Implemented by the team;
  single-index cube_color and internal representations retain unsigned types.
- [x] Supply ostream insertion for styled() results in fmt/color-ostream.h,
  returning the stream for chaining. Implemented, including wide-stream support,
  and now exercised by TaskWarrior's output conversions.

These requests are no longer pending reminders. They do not authorize further
library changes or messages to another chat.

## Deferred split() refactor

Both overloads are back to their original interface and owning results:

```cpp
std::vector<std::string> split(const std::string&, char);
std::vector<std::string> split(const std::string&);
```

A string_view-input / vector<string_view>-result implementation and caller audit
were built and tested, then fully reverted at Terry's request because it belongs
in its own later commit. It is not part of the current integration. The caller
audit found temporary-input lifetime problems in CLI2, config lookups, and Msg;
other callers need ownership, mutation, or NUL-terminated numeric input. Any later
view migration must revisit all callers, not just change the helper signature.
Style::Parse still takes const std::string&, not string_view.

Saved optional patches are `^/build/color-cxx17-check/deferred-split-libshared.patch`
and `deferred-split-taskwarrior.patch`. They describe the deferred implementation
against the then-current color code, not standalone upstream-ready commits.
They are disposable, may become stale, and must not be applied automatically.
The temporary ten view assertions were removed; shared.t is back to 211 checks.

## To-do and open questions

- [ ] Decide portable acquisition and pinning for FmtColor. A single root fmt
  submodule (e.g. external/fmt) was discussed, not implemented; current CMake uses
  the workspace-local installation built from ../fmt. Share one dependency
  between TaskWarrior and libshared rather than embedding duplicate copies.
- [ ] When Terry is ready, prepare proposed upstream work on develop. The rebase
  rehearsal succeeded and was deliberately undone; preserve the original branches
  until explicitly told to redo it. See README.md for revisions and missing
  upstream libshared subprocess changes.
- [ ] Prepare publication deliberately: fmt first, then libshared, then TaskWarrior.
  Terry plans to squash local changes before pushing. Update the parent gitlink
  after any libshared rewrite and settle the .gitmodules fork URL. Verify remote
  state; do not assume these branches or commits have been pushed.
- [ ] Update TaskWarrior's user-facing color documentation for implemented keywords,
  aliases, canonical order, and independent brightness. Consider blink, conceal,
  and strikethrough spelling separately; they are not implemented parser features.
- [ ] Consider a shared accessor for parsed configured styles without losing the
  original user specification strings.
- [ ] Provide light/dark/automatic default palettes, preserving explicit user
  colors. In automatic mode, query OSC 10/11 for default foreground/background,
  use primarily background brightness with foreground contrast as supporting
  information, and provide a short timeout and documented fallback. Respect
  redirected output and color-disable policy. Add tests for explicit and automatic
  selection, user overrides, and detection failure. This remains proposed work,
  not a dependency on a full terminal UI framework. A disposable MSYS2 query
  example and executable are under `build/color-cxx17-check/terminal-colors.*`;
  only Terry's actual terminal can establish its real default colors.
- [ ] Investigate emoji-width failures only when requested. Keep the assertions;
  ignoring known failures is not permission for broad test exclusions.
- [ ] Revisit split() views as their own explicitly authorized task and dedicated
  commits, with a complete caller-lifetime audit.

Structured styled text, SGR parsing, terminal UI frameworks, and other library
design belong to the separate FmtColor project. Discussion does not establish
implemented APIs or authorize incidental implementation here.

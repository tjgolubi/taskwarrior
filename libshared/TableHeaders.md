# Mixed-height table headers cause an out-of-bounds read

**Status** — Reproduced in standalone `libshared` on Linux with GCC 14.2 in C++23 mode. The `fix/BadAlloc` diagnostic revision `da2bf2a` reports the bad index through `std::vector::at()`. Reproduction on upstream `develop` and application of the fix are pending.

## Reproduce

Build and link this example against standalone `libshared`. It creates an 80-column table with one data row and headers of different heights.

```cpp
#include <Table.h>
#include <iostream>

int main() {
  Table table;
  table.width(80);
  table.add("Name");
  table.add("Start\nDate");
  const auto row = table.addRow();
  table.set(row, 0, "Example");
  table.set(row, 1, "2026-11-24");
  std::cout << table.render();
}
```

Rendering throws `std::out_of_range` when it reads index 1 from the one-line `Name` header. The earlier unchecked access has undefined behavior. The expected layout places `Name` beside `Date`, with an empty cell above `Name`.

## Cause

`Table::render()` computes the maximum header height and pads shorter headers at the top. For a one-line header in a two-line header block, the padding condition correctly skips the first row. On the second row, it uses the table-wide row index `i` to read the shorter column's own vector. That vector's only line is at index 0. [Header rendering loop](https://github.com/tjgolubi/libshared/blob/da2bf2aefd2950f0363519a3673c3e3a19d9b169/src/Table.cpp#L210-L223)

## Proposed fix

Compute the number of padding rows for each column. Once the current row is past the padding, subtract that offset to get the index into the column's header lines.

```cpp
const auto& lines = headers.at(c);
const auto padding = max_lines - lines.size();
if (i < padding)
  out += _header.colorize(std::string(widths.at(c), ' '));
else
  out += lines.at(i - padding);
```

A regression test should assert both header lines and the data row. That checks the bottom alignment as well as successful rendering.

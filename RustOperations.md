# Taskwarrior constructs operation wrappers from Rust vector iterators

**Status** — Commit `02a6dd89f` on `fix/BadAlloc` replaces the iterator-based
conversion with indexed access. The same pre-fix code is present on
`develop` and `ref/Cpp23`. A C++17 runtime reproduction is still pending.

## Affected code

`task info` uses `Operation::operations()` to display task history, and
`task undo` uses it to display operations awaiting confirmation. The
function wraps elements from a `rust::Vec<tc::Operation>` in C++
`Operation` objects. Each wrapper holds a pointer to the corresponding
opaque Rust operation.

The old implementation constructs the C++ vector from
`{operations.begin(), operations.end()}`. The fix bypasses the range
constructor and its use of the CXX bridge iterators. The generated CXX
header inspected locally uses `size_of<T>()` for both the iterator stride
and indexed access. The exact failure mechanism in the affected build
remains unverified.

## Fix

Read the operation count with `operations.size()`, reserve that many
wrappers, and append each wrapper with
`result.emplace_back(operations[i])`. This uses the Rust vector's
element count directly and avoids computing a range length from bridge
iterators.

To verify the fix, compare the two implementations in the same build
configuration. Run `task info` on a task with several journal entries
and exercise the `task undo` confirmation path. Check that each command
displays the expected operations in order.

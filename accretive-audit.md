# Accretiveness audit: RISC-V concept exercises

## What "Accretive" means

A concept exercise ships a stub in which every function is just

```asm
.global function_name
function_name:
        ret
```

The exercise is **Accretive** if a student who correctly implements the first
*K* tasks (obeying every instruction for those tasks), and leaves the remaining
functions as stubs, sees **all tests for tasks 1..K pass**.

We want every concept exercise to be Accretive.
`bin/check-accretive` checks this, and CI runs it after `bin/verify-exercises`.

## How an exercise fails to be Accretive

The test file, Unity and the solution are linked into one program before any
test runs. That gives these failure modes.

**Mode A: link time.** The test file refers to a symbol (a function, or a
table in the data section) that the stub does not define. Until the student
writes it, nothing links, so **no** task passes. If the missing symbol is
created in **task 1**, the exercise is still Accretive, because doing task 1
makes the program link. `.meta/ownership.json` records this with
`"requires-task1-to-link": true`.

**Mode B: run time.** A test for task *K* calls a function the instructions
assign to a later task *M > K*. The program links, but that function is still a
stub, so task *K*'s test fails even though the student did tasks 1..K
correctly. This typically happens when task *K*'s result can only be
*observed* through a later task's function.

**Ordering.** A stub can crash the program. For example, a function meant to
return a pointer returns its first argument, and the test then reads through
it. When the test program dies, every test after it goes unreported. So every
test for tasks 1..K must run before any test that reaches a later task's
function.

**Free pass (RISC-V specific).** `ret` leaves `a0`, the first argument,
untouched, so a stub "returns" its first argument. When every test for a task
happens to expect that value, the stub passes the task before the student has
written anything. Such tests teach nothing, so each task needs at least one
test that the stub fails.

**Structural.** The test file is not grouped into tasks by `// TASK: N`
comments, so no prefix of tasks can be checked. The test runner uses the
same markers to show each test under its task.

## Methodology

`bin/check-accretive [slug...]` builds and runs each exercise with the same
tools as `bin/verify-exercises` (zig and qemu-riscv32).

- Each `<slug>_test.c` marks its task groups with `// TASK: N` lines. Every
  test function belongs to the nearest marker above it.
- Each `.meta/ownership.json` maps every `.global` symbol of the stub to the
  task that implements it:

  ```json
  {
    "tasks": {
      "hold_capacity": 1,
      "crates_with_spares": 2
    }
  }
  ```

- The checker never parses assembly. A `.S` file goes through the C
  preprocessor, so assembling a file with `-Dname=__accretive_exemplar_name`
  renames that symbol everywhere in the file, which moves it out of the way.
  For each *K* from 0 to *N*:
  - the exemplar is assembled with every symbol owned by a task *> K* renamed,
  - the stub is assembled with every symbol owned by a task *≤ K* renamed,
  - both are linked with the tests (all of them enabled) and run under qemu.
- **Mode A:** at *K* = 0 the stub alone must link, unless ownership says
  `"requires-task1-to-link": true`.
- **Mode B and Ordering:** for each *K ≥ 1*, every test for tasks 1..K must
  report `PASS`. A test that never reports, because the program crashed first,
  counts as failing.
- **Free pass:** for each *K < N*, at least one test for task *K + 1* must not
  pass.
- **Structure:**
  - the markers must ascend and cover exactly the `## 1.` … `## N.`
    headings of `instructions.md`;
  - every test must follow a marker;
  - `main()` must `RUN_TEST` each test once, in the order they are defined;
  - the symbols in `ownership.json` must be exactly the stub's `.global`s;
  - each symbol must be named, in backticks, in its own task's section of
    `instructions.md`.

### What it does not catch

Renaming applies to a whole file. If the exemplar's task-*K* function calls a
function from a later task, the renamed copy is still called, so the checker
cannot see that a student's task-*K* code would call a stub. Instructions
should only ever ask a task to reuse functions from *earlier* tasks.

## Rules for writing a new exercise

1. The stub defines every symbol the tests use, as a `.global` that does
   nothing but `ret`.
2. Each task's tests use only that task's functions and those of earlier
   tasks.
3. Tests are grouped by `// TASK: N`, in task order, and `main()` runs them in
   the same order.
4. Each task has at least one test whose expected value is not its first
   argument.
5. `.meta/ownership.json` names every stub function and its task.

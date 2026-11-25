# Hints

## General

- Leave the `.option arch, +zbb` line at the top of the file, or the assembler will refuse the Zbb instructions.
- Every task can be done in one to three instructions, with no loops.
- `clz` and `ctz` give 32 when no bit is set.

## 1. Count the hatchlings

- One Zbb instruction counts the set bits.

## 2. Find the first hatchling

- `ctz` gives the position of the lowest set bit.
- For an empty nest, `ctz` gives 32, not -1, so check for 0 first with `beqz` or `bnez`.

## 3. Find the last hatchling

- `clz` counts the zeros above the highest set bit, starting from bit 31.
- If the highest set bit is bit 31, `clz` gives 0, so the place number is 31 minus the count.
- Work out what 31 minus the count is for an empty nest: you may not need a branch at all.

## 4. Count the eggs still waiting

- An egg is still waiting if its place holds an egg **and** has **not** hatched.
- `andn` does "and not" in one instruction.
- Then count what is left.

## 5. Read the sand thermometer

- The readings are signed, so use `max` and `min`, not `maxu` and `minu`.
- Take the larger of the reading and the lowest value, then the smaller of that and the highest value.

## 6. Plan the night patrol

- Moving every turn up one place, with volunteer 31 going back round to volunteer 0, is a rotate left.
- `rol` uses only the lowest 5 bits of the amount, so 32 nights brings the rota back to where it started.

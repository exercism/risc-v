# Hints

## General

- Keep the `.option arch, +v` line at the top of the file: without it, the assembler rejects every vector instruction.
- Use `e8` in `vsetvli`, so that every element is one byte.
- `vsetvli t0, zero, e8, m1, ta, ma` asks for as many bytes as fit in a register.
- Load with `vle8ff.v`, then read the number of bytes it really loaded with `csrr t0, vl`.
- `vmseq.vi` with 0 and `vfirst.m` find the zero byte in a strip.
- The loop has no count, so it has no test at the top: it ends at the strip that holds the zero byte.
- A pointer moves on by `vl` bytes each round.
- In the strip that holds the zero byte, `vsetvli` with the index of the zero byte as the count cuts `vl` down to just the bytes before it.
- Each function needs its own label names.

## 1. Measure a line

- Copy the start address of the line into a spare register before the loop.
- When you find the zero byte, the pointer plus its index is the address of the zero byte.
- That address minus the start address is the length.

## 2. Count the pauses

- Put `'/'` in an ordinary register and compare each strip with it using `vmseq.vx`.
- Cut `vl` down before the compare in the strip that holds the zero byte, so that a `/` after it is not counted.
- `vcpop.m` counts the 1 bits of a mask; add each round's count to a total.
- Keep the index from `vfirst.m` in a register, so that after counting you can tell whether this was the last strip.

## 3. Count the capitals

- Use two unsigned compares: greater than `'A' - 1`, and at most `'Z'`.
- `vmand.mm` keeps only the bytes that pass both.
- Signed compares see bytes from 128 to 255 as negative numbers, so they give the wrong answer.

## 4. Shout a line

- Find the lowercase letters with a mask in `v0`.
- `vsub.vx` with `'a' - 'A'` in an ordinary register, and `, v0.t` at the end, changes only those letters.
- You store the whole strip with `vse8.v`, so the other bytes must keep their values: use `mu` in every `vsetvli`.
- If the tests show `0xFF` where a byte should be unchanged, the inactive elements were filled with ones: check that you asked for `mu`.
- Cut `vl` down before the store in the strip that holds the zero byte, so that nothing after the line is changed.

## 5. Find a cue

- The marker arrives in `a1`, so `vmseq.vx` can compare with it directly.
- Cut `vl` down first, so that a marker after the zero byte is not found.
- If `vfirst.m` finds a marker, its index plus the bytes in earlier strips is the answer.
- If the strip held the zero byte and no marker, return -1.

## 6. Copy a line

- Load from `src` and store to `dest`, moving both pointers on by `vl` bytes.
- In the strip that holds the zero byte, cut `vl` down to its index plus 1, so that the zero byte is copied and nothing after it.
- Only `src` needs `vle8ff.v`: `vse8.v` writes exactly `vl` bytes.

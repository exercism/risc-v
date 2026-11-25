# Hints

## General

- A local array of 16 words takes 64 bytes.
  Put it at the bottom of the frame, at `0(sp)` to `60(sp)`, and any saved registers above it.
- A new local array holds junk, so set every element to 0 before you count anything in it.
- Element `m` of a local array at `sp` is at `sp + 4 * m`.
- Tasks 1, 2 and 3 all start by building the same tally.
  You can write the tally once as a helper of your own, without `.global`, that takes the row, `n` and the address of the tally.
  A function that calls it must save `ra`.
- Every `addi sp, sp, -N` needs a matching `addi sp, sp, N` before `ret`, and `N` must be a multiple of 16.

## 1. Count the different motifs

- One way: set `tally[m]` to how many times motif `m` appears, then count the elements of the tally that are not 0.
- An empty row uses no motifs at all.

## 2. Find the favourite motif

- Build the tally, then look through it for the largest count.
- Keep the best motif and its count in two registers.
- To keep the smaller motif when there is a tie, only replace the best when a count is strictly larger.

## 3. Mix the dye

- `dye_needed` wants the address of the tally in `a0`, and 16 in `a1`.
- The address of the tally is `sp`, if the tally is at the bottom of your frame.
- Your frame must stay in place, and `sp` must be a multiple of 16, while `dye_needed` runs.
- This function makes a call, so save `ra` in the frame.

## 4. Rotate the border

- The motif at index `i` moves to index `(i + k) % n`.
  `rem` gives the remainder.
- Work out `4 * n`, round it up with `addi t0, t0, 15` and `andi t0, t0, -16`, and `sub` it from `sp`.
- Before the `sub`, save `s0` and copy `sp` into it.
  At the end, `mv sp, s0` throws the whole local array away, whatever its size.
- When `n` is 0, the array has size 0, and the loops must not run at all.

## 5. Smooth the pattern

- First copy the whole row into the local array.
- Then, for each `i` from 1 to `n - 2`, compare `copy[i - 1]` with `copy[i + 1]`, and change `row[i]` when they are equal.
- Read the neighbours from the copy, not from the row: the row is being changed as you go.
- `lw` with an offset of `-4` or `4` reaches the element before or after an address.

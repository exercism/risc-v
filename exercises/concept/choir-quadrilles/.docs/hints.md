# Hints

## General

- Keep the `.option arch, +v` line at the top of the file: without it, the assembler rejects every vector instruction.
- Every function uses the strip-mining loop from the vectors concept, with the count tested at the top.
- A permutation inside a register only moves elements within one strip.
  Think about where each round should read from, and where it should write to.
- Each function needs its own label names.

## 1. Every other singer steps forward

- Work out the number of notes to copy first: `(count + 1) / 2`, using `addi` and a shift.
  Use that as the count for the loop, and keep a copy to return.
- `vlse32.v` loads with a stride, in bytes, held in an ordinary register.
  Every second `int32_t` is 8 bytes on.
- Each round, the tune pointer moves on by `vl * 8` bytes, but the destination pointer moves on by `vl * 4`.

## 2. Turn the line around

- Start reading at the last note of the tune: the address of the tune plus `(count - 1) * 4`.
- A stride of -4 makes `vlse32.v` walk backwards through memory.
- Store each strip forwards, with `vse32.v`.
- Each round, the destination pointer moves forwards by `vl * 4` bytes, and the tune pointer moves backwards by `vl * 4` bytes.

## 3. Partners swap places

- `vid.v` gives `[0, 1, 2, 3, ...]`.
  `vxor.vi` with 1 turns it into `[1, 0, 3, 2, ...]`.
- `vrgather.vv` picks elements of a vector using that index vector.
  Its destination must be a different register from both of its sources.
- If `vl` were odd, the last singer of a strip would need their partner from the next strip.
  After the first `vsetvli`, clear the lowest bit of `vl` with `andi` and -2, and use a second `vsetvli` to ask for exactly that many.

## 4. Keep the high voices

- `vmsgt.vx` builds a mask of the notes higher than the threshold.
- `vcompress.vm` packs the chosen notes at the front of another register, and `vcpop.m` says how many there are.
- Before storing, use `vsetvli` to set `vl` to that number, so that only the chosen notes are stored.
  Keep the strip's own `vl` in a different register, for moving the tune pointer on.
- If the tests show `-1` after the kept notes, the whole strip was stored: under `ta`, the elements past the packed notes are filled with ones.
- The destination pointer moves on by the number of notes kept, times 4.

## 5. Echo the previous note

- `vslide1up.vx` moves every element up one place and puts an ordinary register into element 0.
  Its destination must be a different register from its source.
- In the first round, element 0 is the given first note.
  In every later round, it is the last note of the round before.
- To get the last note of a strip into an ordinary register, slide it down to element 0 with `vslidedown.vx` by `vl - 1`, then use `vmv.x.s`.

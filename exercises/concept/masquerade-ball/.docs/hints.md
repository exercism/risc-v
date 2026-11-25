# Hints

## General

- Keep the `.option arch, +v` line at the top of the file: without it, the assembler rejects every vector instruction.
- Every function uses the strip-mining loop from the vectors concept, with the count tested at the top.
- A compare such as `vmseq.vx` writes a mask: one bit for each element.
- A mask that controls another instruction, with `v0.t` or in `vmerge`, must be in `v0`.
- Each function needs its own label names.

## 1. Count the masks of one colour

- Compare each strip with the colour using `vmseq.vx`.
- `vcpop.m` counts the 1 bits of a mask.
- Keep the count so far in an ordinary register, add each round's count to it, and return it at the end.

## 2. Find the first guest without a mask

- `vmseq.vi` compares with an immediate, such as 0.
- `vfirst.m` gives the index of the first 1 bit in this strip, or -1.
- As soon as `vfirst.m` gives a number that is not negative, you can leave the loop.
- Copy the start address of the array into a spare register before the loop.
  When you find a match, the pointer minus the start address, shifted right by 2, is the number of guests in earlier strips.
- If the loop ends without a match, return -1.

## 3. Cap the donations

- `vmsgt.vx` sets a mask bit for each donation greater than the cap.
- `vmerge.vxm vd, vs2, rs1, v0` takes `rs1` where the mask bit is 1, and `vs2[i]` where it is 0.
- Store the whole strip back.

## 4. Discount the juniors

- Load a strip of prices and a strip of ages into two different vector registers.
- `vmslt.vx` sets a mask bit for each age less than the age limit.
- Add `, v0.t` to a `vsub.vx` so that it only changes the juniors' prices.
- You store the whole strip of prices afterwards, so the inactive prices must keep their values: use `mu`, not `ma`, in `vsetvli`.
- If the tests show `-1` where an adult's price should be unchanged, the inactive elements were filled with ones: check that you asked for `mu`.

## 5. Add up the juniors' donations

- Build the mask from the ages, as in task 4.
- Add `, v0.t` to `vredsum.vs` so that only the juniors' donations are added to the seed.
- Carry the total from one round to the next, as in the vector reductions concept.

## 6. Count the lucky-door winners

- The ticket numbers are unsigned, so use the unsigned compares, `vmsltu.vx` and `vmsleu.vx`.
- There is no "greater than or equal" compare for an ordinary register.
  Find the tickets less than the low number, and flip that mask with `vmnot.m`.
- `vmand.mm` keeps only the bits that are 1 in both masks.
- These masks only need counting, so they do not have to be in `v0`.

# Hints

## General

- Keep the `.option arch, +v` line at the top of the file: without it, the assembler rejects every vector instruction.
- Every function uses the same loop shape: `vsetvli`, load, compute, store, move the pointers on, subtract `vl` from the count, and go round again while the count is more than 0.
- Test the count at the top of the loop, so that a count of 0 does nothing.
- Use `e32` in `vsetvli`, because every element is an `int32_t`.
- `vsetvli` puts `vl`, the number of elements for this round, in its first operand.
  Use that register to move on, never a fixed number such as 4.
- Pointers move in bytes, so move each one on by `vl * 4`, which is `vl` shifted left by 2.
- Each function needs its own label names.

## 1. Fill the trays

- Nothing needs to be loaded.
- `vmv.v.x` copies one ordinary register into every element of a vector register.
- Put the `vmv.v.x` after the `vsetvli` in the loop, so that it fills the right number of elements each round.

## 2. Add extra portions

- Load the trays, add with `vadd.vx`, and store them back to the same address.

## 3. Scale the recipe

- This is like task 2, but with multiplication.
- `vmul.vx` multiplies every element by an ordinary register.

## 4. Add up the portions

- Load from both arrays into two different vector registers.
- `vadd.vv` adds two vector registers element by element.
- There are three pointers, and each one moves on by the same number of bytes each round.

## 5. Count the leftovers

- This is like task 4, but with subtraction.
- The order of the operands of `vsub.vv` matters, just as for `sub`.

## 6. Find the room on the trays

- You need the capacity minus each element, not each element minus the capacity.
- `vrsub.vx` subtracts in the reverse order.

# Hints

## General

- A function that makes a `call` needs a stack frame: `addi sp, sp, -16` at the start, and `addi sp, sp, 16` just before `ret`.
- Save `ra` in the frame with `sw ra, 12(sp)` before the first `call`, and load it back with `lw ra, 12(sp)` before `ret`.
- A value you need after a `call` belongs in an `s` register.
  Save each `s` register you use in the frame first, and load it back before `ret`.
- The frame size must be a multiple of 16 bytes, and each saved register takes 4 bytes.
- Every function, including `trim_board` and your own earlier ones, may change any `a` or `t` register.
  `trim_board` really does change all of them.

## 1. Measure a plank

- This is a leaf function: one `mul` does it, and it needs no stack frame.

## 2. Measure a hull

- `trim_board` wants the board length in `a0`, but the boards are there now.
  Keep the boards in `s0` before the call.
- After the call, put the boards and the trimmed length where `plank_length` wants them.
- `ra` and `s0` fit in a 16-byte frame.
- The two carved pieces add 60 centimetres in all.

## 3. Order the timber

- You call `trim_board` once and `plank_length` twice.
- Before calling `trim_board`, keep both numbers of boards in `s` registers.
- Keep the trimmed length in an `s` register too, because it is needed for both planks.
- The first plank's length must survive the second call.
  Once the hull boards are used, their `s` register is free to hold it.
- `ra` and three `s` registers fill a 16-byte frame exactly.

## 4. Line up the fleet

- This is a counting loop, with a `call` in its body.
- Keep everything the loop needs (the number of canoes left, the boards in this hull, the board length and the total) in `s` registers.
- `ra` and four `s` registers take 20 bytes, so round the frame up to 32 bytes.
- Set `a0` and `a1` again before every `call`.

## 5. Price the fleet

- The price arrives in `a3`, and `fleet_length` may change `a3`, so keep the price in an `s` register.
- The first three arguments are already where `fleet_length` wants them.
- Multiply the total length by the price first, and then divide by 100.
  `div` rounds down for numbers that are not negative.

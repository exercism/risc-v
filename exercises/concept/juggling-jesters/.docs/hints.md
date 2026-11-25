# Hints

## General

- `call name` jumps to a function, and puts the return address in `ra`.
- A function that does a `call` loses its own return address, so copy `ra` into a temporary register first, and copy it back before `ret`.
  Pick a register that the function you call does not use.
- Put a helper's arguments in `a0`, `a1`, … before the `call`.
  Its result is in `a0` afterwards.
- A helper may change `a0` to `a7` and `t0` to `t6`.
  Keep any value you need after a call in a register that you know the helper does not touch.

## 1. Count the catches

- This needs no helper: one `mul` does it.

## 2. Score an act

- The balls and rounds are already in `a0` and `a1`, where `catch_count` wants them.
- Adding a register to itself doubles it.
- Keep `ra` in a register that `catch_count` does not use, such as `t6`.

## 3. Count the encore

- You call `catch_count` twice.
- Before the first call, copy the balls and the rounds into registers that `catch_count` does not touch.
- Keep the first result safe too, before the second call.

## 4. Add up the whole show

- This is a counting loop, like the ones you have written before, with a `call` in its body.
- The loop counter is the number of balls in this act.
- Each iteration, set `a0` and `a1` again before the `call`, because `a0` now holds the last result.
- Keep the counter, the total, the number of acts and the number of rounds in registers that `catch_count` does not use.

## 5. Read the score cards

- Write the helper as a plain label, without `.global`.
- The helper needs two tests: one for marks below 0, and one for marks above 10.
- For a small helper that calls nothing, `jal t5, helper` keeps `ra` safe: the return address goes in `t5`, and the helper goes back with `jr t5`.
- Keep the second and third marks, and the running total, in registers the helper does not use.

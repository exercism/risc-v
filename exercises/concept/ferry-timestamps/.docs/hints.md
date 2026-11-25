# Hints

## General

- A `uint64_t` argument takes two registers: the low half first, then the high half.
- A `uint64_t` result goes in `a0` (low half) and `a1` (high half), so set both before `ret`.
- Check the C declarations at the top of the test file to see which registers each argument arrives in.

## 1. Add a delay

- The time arrives in `a0` and `a1`, and the delay in `a2`.
- Add the delay to the low half, then use `sltu` to find out whether the sum wrapped around.
- The carry, 0 or 1, is the only thing to add to the high half.

## 2. Work out the journey time

- Subtract the departure time from the arrival time, not the other way round.
- Work out the borrow with `sltu` before you overwrite a low half.
- Take the borrow away from the high half of the difference.

## 3. Put departures in order

- If the high halves are different, they decide the answer on their own.
- Only if the high halves are equal do the low halves matter.
- Times are unsigned, so use `bltu` or `sltu` for both halves.

## 4. Convert minutes to microseconds

- Load 60000000 into a temporary register with `li`.
- `mul` gives the low half of a product, and `mulhu` gives the high half.
- Do not overwrite the minutes before both instructions have used them.

## 5. Work out the arrival time

- First turn the minutes into a 64-bit number of microseconds, as in task 4.
- Then add two 64-bit numbers: add the low halves, find the carry, and add the high halves and the carry.

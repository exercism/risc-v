# Hints

## General

- Every number in this exercise is a `uint32_t`, an unsigned 32-bit number.
- Many of the test values are `0x80000000` (2147483648) or bigger.
  Signed instructions such as `blt`, `slt` and `div` read these as negative numbers, and give the wrong answer.
  Use the unsigned instructions, whose names end in `u`.

## 1. Print the next ticket

- Adding wraps around by itself.
  You do not need a special case for `0xFFFFFFFF`.

## 2. Count the people waiting

- Subtract the ticket being served from the last ticket printed.
- Subtraction wraps around too, and the wrapped answer is the right one.

## 3. Check the counter number

- Subtract 1 first, so that counters 1 to 12 become 0 to 11.
- Counter 0 then becomes `0xFFFFFFFF`, a huge unsigned number.
- One unsigned comparison with 12 now rejects numbers that are too small and numbers that are too big.
- `sltiu` compares with an immediate and gives 0 or 1.

## 4. Show the longer-open counter first

- "A has been open longer than B" is the same as "B is less than A".
- `sltu` compares two registers as unsigned numbers.

## 5. Work out the average wait

- `divu` divides unsigned numbers.
- Check for 0 customers before you divide, with `bnez` or `beqz`.

## 6. Show the seconds

- First turn microseconds into whole seconds: there are 1000000 microseconds in a second.
- The seconds part is the remainder when the whole seconds are divided by 60.
- Use `divu` and `remu`, not `div` and `rem`.

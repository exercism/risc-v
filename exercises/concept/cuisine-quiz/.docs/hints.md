# Hints

## General

- The tests fill the buffer with `'#'` characters before each test, so a missing zero byte shows up as extra `#`s.
- Load 10 into a register once, before the loop, because `divu` and `remu` have no immediate form.
- Add `'0'` to a digit to get its character, and subtract `'0'` from a character to get its digit.

## 1. Show a score

- `remu` by 10 gives the last digit, and `divu` by 10 gives the other digits.
- Test at the bottom of the loop, so that 0 still writes one digit.
- The digits come out last-first, so either reverse them afterwards, or count them first and write them from the end backwards.
- Keep the start of the buffer, so that you can work out the length.

## 2. Show the penalty points

- `bgez` can skip the sign when the total is not negative.
- After writing the `'-'`, use `neg` to get the magnitude, and carry on as in task 1.
- `neg` of -2147483648 gives `0x80000000`, which is only right when you read it as unsigned, so use `divu` and `remu`, not `div` and `rem`.

## 3. Number the tables

- Count the digits first, with a do-while loop of `divu` by 10.
- The length is the larger of the number of digits and `width`.
- Write from the end of the text backwards, exactly `length` characters.
  Once the number has run out of digits, `remu` gives 0, which is just the padding you need.

## 4. Read an answer sheet

- For each character, subtract `'0'` and use the one-compare range check: as unsigned, a digit gives less than 10.
- The zero byte is not a digit, so the same check stops the loop at the end of the string.
- Multiply the number so far by 10 with `mul`, then add the new digit.

## 5. Read a penalty

- Check the first character for `'-'`, remember the answer in a register, and skip it if it is there.
- Read the digits as in task 4, then use `neg` at the end if there was a `'-'`.
- The digits of `"-2147483648"` make 2147483648, which is `0x80000000`, and `neg` of that is -2147483648.

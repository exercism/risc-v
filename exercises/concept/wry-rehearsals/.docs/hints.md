# Hints

## General

- A line arrives in `a0` as the address of its first character.
- Load each character with `lbu`, and move to the next one by adding 1 to the pointer.
- Stop when you load the zero byte, and test for it at the top of the loop so that the empty string `""` works.
- You can write characters in single quotes, such as `'A'` or `' '`, wherever a number would go.
  Write the zero byte as `zero` or `0`, not `'\0'`.

## 1. Measure a line

- Keep the start address, walk a second pointer to the zero byte, then subtract.
- Or count the characters in a register as you go.

## 2. Count the pauses

- The character to look for arrives in `a1`.
- `beq` and `bne` compare a loaded character with `a1`.
- `lb` would turn bytes from 128 to 255 into negative numbers, so they would never match `a1`.

## 3. Find the first word

- This is like task 1, but the loop has two ways to stop: a zero byte or a space.
- Load `' '` into a register before the loop, so the loop can compare with it.

## 4. Spot the shouts

- Use the one-compare range check: subtract `'a'`, then compare with 26 as unsigned.
  Do the same with `'A'` for uppercase letters.
- As soon as you find a lowercase letter you know the answer is 0, and you can return straight away.
- Keep a register that becomes 1 when you see an uppercase letter.

## 5. Whisper a line

- Only uppercase letters change, so check each character with a range check first.
- A lowercase letter's code is 32 more than its uppercase letter's, and you can write 32 as `'a' - 'A'`.
- Store the changed character back to the same address with `sb`.

## 6. Copy a cue

- Use two pointers: one into `src` to read, and one into `dest` to write.
- Keep the start of `dest`, so that you can work out the length at the end.
- Do not forget the zero byte at the end of `dest`.
  If you store each byte before you test it, the zero byte gets copied too.

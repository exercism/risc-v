# Hints

## General

- Define each constant once, near the top of the file, with `.equ NAME, value`.
- Use UPPER_SNAKE_CASE names, such as `KNIGHT` and `KNIGHT_VALUE`.
- Your constants must have the same values as the `enum` declarations at the top of the test file.
- A constant can be used anywhere an immediate can, for example in `li` and `addi`.
- Branches still compare two registers, so load a constant into a temporary register with `li` before comparing with it.

## 1. Count the value of a piece

- Define one constant for each piece code, and another for each piece value.
- This is an `if`-`elif`-`else` chain: compare the code with `PAWN`, then `KNIGHT`, and so on.
- Once you have checked the first five pieces, the only one left is the king.

## 2. Promote a pawn

- The player gains a queen and loses a pawn.
- The assembler can work out `QUEEN_VALUE - PAWN_VALUE` for you, right inside an `addi`.

## 3. Count the empty squares

- A constant can be defined from another one, for example with `*`.
- Remember the order of `sub`: the number of squares comes first.

## 4. Stay on the board

- A constant such as `BOARD_SIZE - 1` gives the last index.
- The square is off the board if either number is less than 0 or greater than the last index.
- Each of those four tests can branch to the same "off the board" label.

## 5. Score a game

- Define constants for the result codes and for the half points of each result.
- Compare the result code with each code in turn.

## 6. Decide a team match

- Compare the two scores with branches, and return one of your result-code constants.
- Return the code, such as `RESULT_WIN`, not the half points.

# Hints

## General

- Each argument arrives in `a0`, `a1`, `a2`, `a3`, in the order of the C declaration at the top of the test file.
- An output parameter is an address: store the answer there with `sw`.
- For `sw`, the value to store comes first and the address comes second.
- A `void` function returns nothing, so it does not matter what is left in `a0`.

## 1. Split a clue

- The two numbers arrive in `a0` and `a1`, and the two pointers in `a2` and `a3`.
- Work out each answer in a temporary register, then store it through its pointer.

## 2. Question the neighbours

- The desk number arrives in `a0`, and the return value must also leave in `a0`.
- Store the left desk through the pointer in `a1` before you change `a0`.

## 3. Sum up the case

- The clues are 4 bytes apart, at offsets 0, 4 and 8 from the pointer in `a0`.
- Load all three clues first, then work out the total, then store the two answers.

## 4. Keep the tally

- Load the tally into a register and keep that register unchanged, because it is the value to return.
- Work out the new tally in a different register, and store it back to the same address.

## 5. Unscramble a message

- Load both numbers before you store anything.
- Work out both new numbers in registers from the old numbers, then store them.

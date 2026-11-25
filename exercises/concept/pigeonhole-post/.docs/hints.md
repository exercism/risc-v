# Hints

## General

- A pointer argument arrives in a register as an address.
- `lw` loads the word at an address into a register, and `sw` stores a register into the word at an address.
- Each `int32_t` takes 4 bytes, so neighbouring counts are 4 bytes apart.
- For `sw`, the value to store comes first and the address comes second.

## 1. Check the first pigeonhole

- The first count is at offset 0 from the pointer in `a0`.
- You can load straight into `a0`.

## 2. Check the third pigeonhole

- The second count is 4 bytes after the first.
  How far along is the third?

## 3. Count a whole row

- Load each of the four counts into its own register, then add them up.

## 4. Deliver some letters

- Load the count, add the new letters to it in a register, then store it back to the same address.

## 5. Move letters to the right family

- Load both counts before you store anything.
- The `zero` register always holds 0, and you can store it like any other register.

## 6. Swap two pigeonholes

- Load both counts into two different registers.
- Then store each register through the other pointer.

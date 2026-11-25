# Hints

## General

- Write the codes in binary with `0b`, or in hex with `0x`, whichever you find easier to read.
- `and` keeps bits, `or` sets bits, and `xor` flips bits.
- The immediate forms `andi`, `ori` and `xori` take a number between -2048 and 2047.

## 1. Listen to the first four beats

- You need a mask with a 1 on each of bits 0 to 3.
- `andi` keeps only the bits under the mask's 1s.

## 2. Add some knocks

- `or` gives a 1 wherever either input has a 1.

## 3. Flip some beats

- `xor` flips each bit of one register where the other register has a 1.

## 4. Remove some knocks

- `and` keeps the bits under a mask's 1s, but here the marked beats are the ones to clear.
- `not` flips every bit, so it turns the marked beats into the ones to keep.

## 5. Check a knock at the door

- `xor` gives a 1 where two bits differ, but you want a 1 where they are the same.
- After flipping every bit, bits 8 to 31 are all 1, so clear them with a mask.
- The mask for bits 0 to 7 is `0xFF`, which fits in an immediate.

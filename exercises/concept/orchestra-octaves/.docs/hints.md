# Hints

## General

- Shifting left by `n` places multiplies by 2 to the power `n`, and shifting right divides by it.
- Instructions ending in `i`, such as `slli`, take the shift amount as an immediate.
  The others take it from a register.

## 1. Go up some octaves

- Each octave up doubles the frequency, which is a shift left by one place.
- The number of octaves arrives in `a1`, so use the register form of the shift.

## 2. Go down some octaves

- Each octave down halves the frequency, which is a shift right by one place.
- A frequency is never negative, so the empty places should be filled with zeros.

## 3. Read a note's octave

- First shift the octave field down so that it starts at bit 0.
- Then use `andi` with a mask to clear the duration and bar number above it.
- The octave field is 4 bits wide.

## 4. Read a note's duration

- This works the same way as reading the octave, with a different shift amount.

## 5. Write a new note

- The pitch is already in the right place.
- Shift the octave and the duration up to their places, then combine all three with `or`.
- You will need a temporary register, such as `t0`.

## 6. Tune in halves

- The offset can be negative, so use a shift that keeps the sign.
- Repeated halving, rounding down each time, is the same as one arithmetic shift right by the number of turns.

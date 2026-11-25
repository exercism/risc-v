# Instructions

Your school in Neiafu is putting on a drama production, and the school orchestra is rehearsing music from Tchaikovsky's ballet _Swan Lake_.
You play the violin, and you also look after the orchestra's laptop, which stores the music and helps everyone tune.

The music teacher would like a few small functions for the laptop.

## 1. Go up some octaves

A note one **octave** higher has exactly twice the frequency.
The laptop measures frequencies in whole hertz (Hz), so the A string of a violin is 440 Hz, and the A one octave above it is 880 Hz.

Implement the `octave_up` function, which takes a frequency and a number of octaves, and returns the frequency that many octaves higher.

```c
octave_up(440, 1);
// => 880
```

## 2. Go down some octaves

A note one octave lower has half the frequency.
When the result is not a whole number, the laptop rounds it down.

Implement the `octave_down` function, which takes a frequency and a number of octaves, and returns the frequency that many octaves lower.

```c
octave_down(440, 3);
// => 55
```

## 3. Read a note's octave

The laptop stores each note of the music as a single 32-bit number, with several fields packed into it:

| Bits     | Field                                                  |
| -------- | ------------------------------------------------------ |
| 0 to 7   | the pitch: 0 for C, 1 for C♯, and so on up to 11 for B |
| 8 to 11  | the octave, from 0 to 15                               |
| 12 to 15 | the duration, in beats                                 |
| 16 to 31 | the number of the bar the note is in                   |

For example, `0x0003240B` is a B (11) in octave 4, lasting 2 beats, in bar 3.

Implement the `note_octave` function, which takes a packed note and returns its octave.

```c
note_octave(0x0003240B);
// => 4
```

## 4. Read a note's duration

Implement the `note_duration` function, which takes a packed note and returns its duration in beats.

```c
note_duration(0x0003240B);
// => 2
```

## 5. Write a new note

The teacher is writing a new part for the oboe.

Implement the `pack_note` function, which takes a pitch, an octave and a duration, and returns them packed into a note, in bar 0.
You can assume each value fits in its field.

```c
pack_note(9, 4, 2);
// => 0x00002409
```

## 6. Tune in halves

The laptop's tuner shows how far a string is from the right note, in **cents** (hundredths of a semitone).
A positive offset means the string is too high (sharp), and a negative offset means it is too low (flat).

Each turn of the violin's fine tuner halves the offset.
When the offset is odd, the tuner rounds it down, towards the lower number: half of 5 is 2, and half of -5 is -3.

Implement the `offset_after_turns` function, which takes an offset in cents and a number of turns, and returns the offset after that many turns.

```c
offset_after_turns(-40, 3);
// => -5
```

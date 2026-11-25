# Instructions

The sailing club in Port Vila runs a Saturday course for young sailors.
This term, the course covers the International Code of Signals.
Each letter from `A` to `Z` has its own flag, which a ship can fly to send a message without a radio.
For example, flag `A` means "I have a diver down; keep well clear at slow speed", and flag `O` means "Man overboard".

On the radio, each letter is spoken as a code word, so that `B` and `D` cannot be mixed up:

| Letter | Word     | Letter | Word     | Letter | Word     |
| ------ | -------- | ------ | -------- | ------ | -------- |
| `A`    | Alfa     | `J`    | Juliett  | `S`    | Sierra   |
| `B`    | Bravo    | `K`    | Kilo     | `T`    | Tango    |
| `C`    | Charlie  | `L`    | Lima     | `U`    | Uniform  |
| `D`    | Delta    | `M`    | Mike     | `V`    | Victor   |
| `E`    | Echo     | `N`    | November | `W`    | Whiskey  |
| `F`    | Foxtrot  | `O`    | Oscar    | `X`    | X-ray    |
| `G`    | Golf     | `P`    | Papa     | `Y`    | Yankee   |
| `H`    | Hotel    | `Q`    | Quebec   | `Z`    | Zulu     |
| `I`    | India    | `R`    | Romeo    |        |          |

The spellings `Alfa` and `Juliett` are correct: they are chosen so that speakers of many languages say them the same way.

The club's instructor would like a small program to help the sailors practise.
Use lookup tables in your read-only data for every task, rather than chains of branches.

## 1. Size the word cards

The sailors practise with cards that show a code word, with one square for each character.
`X-ray` needs 5 squares, because the hyphen gets a square too.

Implement the `phonetic_length` function, which takes a character and returns the number of characters in its code word.
If the character is not a capital letter from `A` to `Z`, it returns -1.

```c
phonetic_length('N');
// => 8
phonetic_length('?');
// => -1
```

## 2. Say the letter

Implement the `phonetic_word` function, which takes a capital letter from `A` to `Z` and returns its code word as a string.
Spell each word exactly as it is in the table above, including the capital letter and the hyphen in `X-ray`.
The letter is always a capital letter.

```c
phonetic_word('J');
// => "Juliett"
```

## 3. Know the colours

Every flag uses some of five colours.
The test file numbers the colours so that each one is a single bit, and a set of colours is the bits added together:

| Colour | Value |
| ------ | ----- |
| white  | 1     |
| blue   | 2     |
| red    | 4     |
| yellow | 8     |
| black  | 16    |

These are the colours of each flag:

| Flag | Colours            | Flag | Colours            | Flag | Colours                  |
| ---- | ------------------ | ---- | ------------------ | ---- | ------------------------ |
| `A`  | white, blue        | `J`  | blue, white        | `S`  | white, blue              |
| `B`  | red                | `K`  | yellow, blue       | `T`  | red, white, blue         |
| `C`  | blue, white, red   | `L`  | yellow, black      | `U`  | red, white               |
| `D`  | yellow, blue       | `M`  | blue, white        | `V`  | white, red               |
| `E`  | blue, red          | `N`  | blue, white        | `W`  | blue, white, red         |
| `F`  | white, red         | `O`  | red, yellow        | `X`  | white, blue              |
| `G`  | yellow, blue       | `P`  | blue, white        | `Y`  | yellow, red              |
| `H`  | white, red         | `Q`  | yellow             | `Z`  | yellow, blue, red, black |
| `I`  | yellow, black      | `R`  | red, yellow        |      |                          |

Implement the `flag_colours` function, which takes a capital letter from `A` to `Z` and returns the set of colours on its flag.
The letter is always a capital letter.

```c
flag_colours('A');
// => 3, which is white (1) + blue (2)
```

## 4. Count the colours

The club is sewing new flags, and needs to know how many colours of cloth each flag takes.

Implement the `colour_count` function, which takes a capital letter from `A` to `Z` and returns the number of colours on its flag.
The letter is always a capital letter.

Look up the letter's set of colours in your table from task 3.
A set of five colours is a number from 0 to 31, so use it as the index into a second table of 32 bytes, which holds the number of colours in each set.

```c
colour_count('C');
// => 3
```

## 5. Time the radio call

Before a race, the instructor reads a short message over the radio, spelling each letter with its code word.
To know how long the call will take, the instructor counts the characters in all the code words.

Implement the `spell_length` function, which takes a message and returns the total number of characters in the code words for its letters.
Only capital letters are spelled.
Every other character, such as a space or a digit, adds nothing.

```c
spell_length("SOS");
// => 17, which is Sierra (6) + Oscar (5) + Sierra (6)
```

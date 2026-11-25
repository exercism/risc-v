# Instructions

Your school in Port Moresby is putting on a comedy, _The Fish That Never Bit_, at the end of term.
Your friend Gari plays Uncle Bomai, who has sat on the same jetty since Tuesday, waiting for a bite, and has a dry, gently mocking remark for everything.
Gari keeps all of Uncle Bomai's wry lines in a notebook, and rehearses them at home until they sound just right.
Dorcas, the stage manager, has typed up the whole script, and wants some small functions to help her check it.

Each line of the script is a string: a row of characters that ends with a zero byte.

## 1. Measure a line

Gari wants to know how long each of Uncle Bomai's lines is, so that the longest ones get the most practice.

Implement the `line_length` function, which takes a line and returns the number of characters in it.
Spaces and punctuation count as characters, but the zero byte at the end does not.

```c
line_length("Oh good, another sunny day.");
// => 27
```

## 2. Count the pauses

Dorcas marks each place where an actor should pause with a `/`.

Implement the `count_char` function, which takes a line and a character, and returns how many times that character appears in the line.
The character is never the zero byte, but it can be any other byte from 1 to 255.

```c
count_char("Lovely. / Just lovely. / Truly.", '/');
// => 2
```

## 3. Find the first word

The prompter, who sits beside the stage, only whispers the first word of a line when an actor forgets it.

Implement the `first_word_length` function, which takes a line and returns the number of characters before the first space.
If the line has no space, the whole line is one word.
A line that starts with a space has a first word of length 0.

```c
first_word_length("Perfect, the fish are only shy.");
// => 8
```

## 4. Spot the shouts

Lines that Uncle Bomai must shout are typed in capital letters.

Implement the `is_shouted` function, which returns 1 if the line has at least one uppercase letter and no lowercase letters, and 0 otherwise.
Digits, spaces and punctuation do not count as letters.

```c
is_shouted("WHAT A SURPRISE!");
// => 1
is_shouted("What a surprise!");
// => 0
```

## 5. Whisper a line

In the second act a heron lands on the jetty, and Uncle Bomai must whisper every line so that he does not scare it away.

Implement the `whisper` function, which changes the line in place so that every uppercase letter becomes lowercase.
All the other characters stay the same.
The function returns nothing.

```c
char line[] = "HUSH, Uncle Bomai!";
whisper(line);
// line is now "hush, uncle bomai!"
```

## 6. Copy a cue

A **cue** is the line just before yours, which tells you it is your turn to speak.
Gari copies each cue into the notebook, so Uncle Bomai knows when to come in.

Implement the `copy_cue` function, which takes a buffer `dest` and a line `src`.
It copies the line into the buffer, ends it with a zero byte, and returns the number of characters copied, not counting the zero byte.
The buffer is always big enough.

```c
char notes[40];
copy_cue(notes, "Has anyone seen my hook?");
// => 24, and notes is now "Has anyone seen my hook?"
```

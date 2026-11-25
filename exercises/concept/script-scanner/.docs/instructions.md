# Instructions

A theatre group in Suva, Fiji, is rehearsing a new play, _The Voyage of the Drua_.
A **drua** is a large Fijian sailing canoe with two hulls.
Mere, the stage manager, keeps the whole script on a laptop, and some of its lines are very long.

Josefa is the **prompter**: he sits beside the stage and whispers a line to any actor who forgets it.
He wants a script scanner that checks each line quickly, so that he is never too slow with a whisper.

Each line of the script is a string: a row of bytes that ends with a zero byte.
Your job is to write the functions using byte vectors, so that each instruction looks at many characters at once.

Every function in this exercise must work for lines of any length, must use `vle8ff.v` to read a line, and must not read or change anything after the zero byte except through `vle8ff.v`.
A line may end right at the end of the memory that your program can read.

## 1. Measure a line

Josefa wants to know how long each line is, so that he can find the longest ones.

Implement the `script_length` function, which takes a line and returns the number of characters in it, not counting the zero byte.

```c
script_length("Bula vinaka, welcome to Suva!");
// => 29
```

## 2. Count the pauses

Mere marks each place where an actor should pause with a `/`.

Implement the `count_pauses` function, which takes a line and returns how many `/` characters it has.

```c
count_pauses("Bula! / Welcome / to the show.");
// => 2
```

## 3. Count the capitals

The names of places and people start with a capital letter, and scene headings are typed all in capitals.
Mere checks that the headings have not lost any letters.

Implement the `count_capitals` function, which takes a line and returns how many uppercase letters, `'A'` to `'Z'`, it has.
Every other byte, including the bytes from 128 to 255, is not an uppercase letter.

```c
count_capitals("Act One, Scene Two: SUVA HARBOUR");
// => 15
```

## 4. Shout a line

In the storm scene, the sailors must shout every line over the noise of the wind.

Implement the `shout` function, which changes a line in place so that every lowercase letter, `'a'` to `'z'`, becomes uppercase.
All the other bytes stay the same.
The function returns nothing.

```c
char line[] = "bula, everyone!";
shout(line);
// line is now "BULA, EVERYONE!"
```

## 5. Find a cue

A **cue** is a signal that tells an actor when to do something.
Mere marks cues in a line with a marker character, such as `*` for a drum beat.

Implement the `find_cue` function, which takes a line and a marker, and returns the index of the first marker in the line.
If the line has no marker, it returns -1.
The marker is never the zero byte, but it can be any other byte from 1 to 255.

```c
find_cue("Wait for the drum * then enter", '*');
// => 18
```

## 6. Copy a line

Josefa copies the lines he whispers most often onto his own prompt sheet.

Implement the `copy_line` function, which takes a buffer `dest` and a line `src`.
It copies the line into the buffer, including its zero byte.
It must not write anything in `dest` after the zero byte.
The buffer is always big enough, and the function returns nothing.

```c
char sheet[24];
copy_line(sheet, "Exit, stage left.");
// sheet is now "Exit, stage left."
```

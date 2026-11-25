# Instructions

You belong to the Kukum Detective Club, a group of students at a school in Honiara.
The editor of a local newspaper has found an old cardboard box in the back of her office.
It is full of wrinkled case records from the 1970s: lost goats, missing canoes, and strange lights over Savo.
She has lent the box to the club, and asked whether you can solve any of the old mysteries.

The club has typed every record into a computer.
Each record is a string, and a list of records is an array of pointers to those strings.
Write some functions to help the club search the box.

## 1. Count the records

The first list the club typed up ends with a null pointer, so it has no count.

Implement the `record_count` function, which takes a NULL-terminated array of records and returns how many records it holds.
An empty record, `""`, is still a record.

```c
const char *records[] = {"Missing canoe at Point Cruz", "Goat in the church hall", "Lights over Savo", NULL};
record_count(records);
// => 3
```

## 2. Find the longest record

The longest records usually have the most clues.
From now on, every list comes with a count.

Implement the `longest_record` function, which takes an array of records and its count, and returns the index of the longest record.
The count is at least 1.
If two or more records are the longest, return the index of the first of them.

```c
const char *records[] = {"Lost key", "Missing canoe at Point Cruz", "Goat in the church hall"};
longest_record(records, 3);
// => 1
```

## 3. Find a name

The club has a list of the people who reported the cases.

Implement the `find_name` function, which takes an array of names, its count, and a name to look for.
It returns the index of the first name in the array whose text is exactly the same as the name to look for, or -1 if there is none.
The name to look for is a different string from those in the array, so compare the text, not the pointers.

```c
const char *names[] = {"Ruth Kiko", "John Maeli", "Grace Tovua", "Peter Ramo"};
find_name(names, 4, "Grace Tovua");
// => 2
find_name(names, 4, "Grace");
// => -1
```

## 4. Sort by first letter

The club wants to file the records in folders by their first letter.

Implement the `count_starting_with` function, which takes an array of records, its count and a letter.
It returns how many of the records start with exactly that letter.
Capital and small letters are different, and an empty record does not start with any letter.

```c
const char *records[] = {"Missing canoe at Point Cruz", "Goat in the church hall", "Market clock stopped at noon"};
count_starting_with(records, 3, 'M');
// => 2
```

## 5. Read the map

At the bottom of the box is a hand-drawn map of an old storeroom.
The club has typed it in as rows of characters: `#` is a wall, `.` is floor, and `X` marks a spot.

Implement the `grid_char` function, which takes an array of rows, a row number and a column number, and returns the character at that place on the map.
Row 0 is the top row, and column 0 is the first character of each row.
The row and column are always inside the map.

```c
const char *rows[] = {"#########", "#..X....#", "#.......#", "####.####"};
grid_char(rows, 1, 3);
// => 'X'
```

## 6. Collect the short records

Short records are quick to read, so the club wants a list of them.

Implement the `collect_short` function, which takes an output array `out`, an array of records, its count, and a maximum length.
For each record whose length is at most the maximum length, in order, store the record's pointer in the next element of `out`.
Do not copy the characters.
Return how many pointers you stored.
`out` is always big enough, and you must not change any element after the ones you store.

```c
const char *records[] = {"Lost key", "Missing canoe at Point Cruz", "Odd note", "Goat in the church hall", "Fish"};
const char *out[5];
collect_short(out, records, 5, 8);
// => 3, and out holds records[0], records[2] and records[4]
```

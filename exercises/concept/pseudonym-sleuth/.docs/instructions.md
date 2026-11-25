# Instructions

At a secondary school in Apia, the student newspaper comes out every Friday.
For the past month, each issue has had a funny poem about the school canteen, signed only "The Gecko".
Nobody knows who The Gecko is.
"The Gecko" is a **pseudonym**: a made-up name that a writer uses instead of their real one.

Sina runs the school's mystery club, and she has decided to find out.
The Gecko hides a few numbers in every poem, and Sina is sure they are clues.
She would like some small functions to help her work through them.
Each one has more than one answer, so most of the answers go back through pointers.

## 1. Split a clue

Every poem ends with two numbers.
Sina has noticed that their sum and their difference both mean something.

Implement the `split_clue` function.
It takes the two numbers, a pointer for the sum and a pointer for the difference.
It stores the first number plus the second through the sum pointer, and the first number minus the second through the difference pointer.
It returns nothing.

```c
int32_t sum;
int32_t difference;
split_clue(12, 5, &sum, &difference);
// sum => 17
// difference => 7
```

## 2. Question the neighbours

The latest poem was left on a desk in the library.
The desks are numbered along a row, and the students at the desks on either side might have seen who left it.

Implement the `desk_neighbours` function.
It takes the number of the desk where the poem was found, and a pointer for the desk on the left.
It stores the number of the desk on the left, which is one less, through the pointer.
It returns the number of the desk on the right, which is one more.

```c
int32_t left;
desk_neighbours(14, &left);
// => 15
// left => 13
```

## 3. Sum up the case

Sina's notebook holds the clues from the last three poems, oldest first.

Implement the `case_summary` function.
It takes a pointer to the three clues, a pointer for the total and a pointer for the latest clue.
It stores the total of the three clues through the total pointer, and the third clue through the latest pointer.
It returns nothing.

```c
int32_t clues[] = {3, 8, 6};
int32_t total;
int32_t latest;
case_summary(clues, &total, &latest);
// total => 17
// latest => 6
```

## 4. Keep the tally

The club keeps a tally of how many clues it has found so far.
When someone finds more, Sina wants to know what the tally was before, as well as update it.

Implement the `update_tally` function.
It takes a pointer to the tally and the number of clues just found.
It adds the new clues to the tally in memory, and returns the tally from before the update.

```c
int32_t tally = 9;
update_tally(&tally, 4);
// => 9
// tally => 13
```

## 5. Unscramble a message

The Gecko has started scrambling the two numbers in each message.
To unscramble them, both numbers are replaced at the same time: the first becomes the sum of the two, and the second becomes the difference, the first minus the second.
Both new numbers are worked out from the old numbers.

Implement the `unscramble` function.
It takes a pointer to the first number and a pointer to the second number, and replaces both in memory.
It returns nothing.

```c
int32_t first = 7;
int32_t second = 3;
unscramble(&first, &second);
// first => 10
// second => 4
```

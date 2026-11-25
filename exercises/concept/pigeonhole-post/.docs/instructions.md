# Instructions

In a village in the Eastern Highlands of Papua New Guinea, the post office is a small room beside the school.
On the wall is a wooden rack of **pigeonholes**: little open boxes, one for each family, with a number painted above each one.
Every week the mail truck comes up the road from Goroka, and the postmaster, Mrs Gima, sorts the letters into the pigeonholes.

Mrs Gima keeps a count of the letters waiting in each pigeonhole on the post office computer.
Each count is an `int32_t`, and the counts for neighbouring pigeonholes sit next to each other in memory.
She would like some small functions to read and update them.

## 1. Check the first pigeonhole

Implement the `letters_in_first` function.
It takes a pointer to the counts for a row of pigeonholes, and returns the number of letters in the first one.

```c
int32_t row[] = {7, 2, 9, 4};
letters_in_first(row);
// => 7
```

## 2. Check the third pigeonhole

Implement the `letters_in_third` function.
It takes a pointer to the counts for a row of pigeonholes, and returns the number of letters in the third one.

```c
int32_t row[] = {7, 2, 9, 4};
letters_in_third(row);
// => 9
```

## 3. Count a whole row

Each row of the rack has four pigeonholes.

Implement the `total_of_four` function.
It takes a pointer to the counts for a row of four pigeonholes, and returns the total number of letters in the row.

```c
int32_t row[] = {7, 2, 9, 4};
total_of_four(row);
// => 22
```

## 4. Deliver some letters

Implement the `deliver` function.
It takes a pointer to the count for one pigeonhole, and the number of letters Mrs Gima has just put in it.
It adds the new letters to the count in memory, and returns nothing.

```c
int32_t pigeonhole = 5;
deliver(&pigeonhole, 3);
// pigeonhole => 8
```

## 5. Move letters to the right family

Sometimes a family moves house, and their letters must go to their new pigeonhole.

Implement the `move_letters` function.
It takes a pointer to the count for the old pigeonhole, and a pointer to the count for the new one.
It adds the old pigeonhole's letters to the new pigeonhole's count, and sets the old count to 0.
It returns nothing.

```c
int32_t old_pigeonhole = 4;
int32_t new_pigeonhole = 1;
move_letters(&old_pigeonhole, &new_pigeonhole);
// old_pigeonhole => 0
// new_pigeonhole => 5
```

## 6. Swap two pigeonholes

Two families have swapped houses, so Mrs Gima swaps the letters in their pigeonholes.

Implement the `swap_pigeonholes` function.
It takes pointers to the counts for two pigeonholes, and swaps the two counts in memory.
It returns nothing.

```c
int32_t first = 6;
int32_t second = 2;
swap_pigeonholes(&first, &second);
// first => 2
// second => 6
```

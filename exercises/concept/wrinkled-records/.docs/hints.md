# Hints

## General

- Each element of an array of records is a pointer: load it with `lw`, not `lbu`, and move to the next element by adding 4.
- Once you have a record's pointer, load its characters with `lbu`, and move to the next character by adding 1.
- When a function needs a loop over the array and a loop over each string, keep the array pointer and the string pointer in different registers.

## 1. Count the records

- Load each element with `lw`, and stop when it is 0.
- You can count in a register as you go, or keep the start address and turn the final distance in bytes into a count.

## 2. Find the longest record

- For each record, find its length with an inner loop, as in the strings concept.
- Keep two values as you go: the length of the longest record so far, and its index.
- To keep the first of several longest records, replace the best only when a record is strictly longer.

## 3. Find a name

- `beq` on two pointers compares addresses, which is not what you want here.
- For each name, walk two pointers along the two strings together, one character at a time.
- Stop the inner loop as soon as the characters differ, and move on to the next name.
- If the characters are the same and are zero, both strings have ended together: you have found the name.

## 4. Sort by first letter

- The first character of a record is at offset 0 from its pointer.
- An empty record's first byte is 0, which is never equal to a letter, so it needs no special check.

## 5. Read the map

- First work out the address of `rows[row]` with a shift and an add, and load the row's pointer with `lw`.
- Then add the column to that pointer, and load the character with `lbu`.
- There is no loop in this task.

## 6. Collect the short records

- Use one pointer to walk through `records`, and another to walk through `out`.
- Store a record's pointer with `sw`, then move the `out` pointer on by 4.
- Count the pointers you store, so that you can return the count at the end.

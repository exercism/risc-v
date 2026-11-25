# Instructions

On a headland at the edge of Vavaʻu, in the north of Tonga, there is a small white lighthouse.
Its keeper, Lupe, watches the channel from the window at the top.
Every morning the village fishing boats go out past the headland, and every afternoon they come home again.

There are 32 fishing boats, and each one has a number from 0 to 31 painted on its bow.
Lupe keeps her log of the day's sightings as a single `uint32_t`.
Bit `i` of the log is 1 if boat `i` has passed the lighthouse today, and 0 if it has not.

For example, the log `0x00000019` is `0b11001`, which means that boats 0, 3 and 4 have passed.

Lupe would like some functions to help her keep the log.

## 1. Log a sighting

Implement the `sight` function, which takes the log and the number of a boat that has just passed.
It returns the log with that boat added.
If the boat is already in the log, the log does not change.

```c
sight(0x00000010, 3);
// => 0x00000018
```

## 2. Check for a boat

A worried family calls the lighthouse on the radio to ask whether their boat has gone past.

Implement the `has_passed` function, which takes the log and the number of a boat.
It returns 1 if that boat is in the log, and 0 if it is not.

```c
has_passed(0x00000018, 3);
// => 1
```

## 3. Find the missing boats

Each morning, the harbour master sends Lupe the set of boats that went out to sea, in the same form as the log.
Before dark, Lupe checks which of them she has not yet seen come home.

Implement the `missing` function, which takes the set of boats that went out and the log of boats seen.
It returns the set of boats that went out but are not in the log.
Boats in the log that did not go out are not missing.

```c
missing(0x000000FF, 0x0000000F);
// => 0x000000F0
```

## 4. Count the boats

Implement the `boats_seen` function, which takes the log and returns the number of boats in it.

```c
boats_seen(0x00000019);
// => 3
```

## 5. Name the first boat

Implement the `first_boat` function, which takes the log and returns the smallest boat number in it.
If the log is empty, it returns -1.

```c
first_boat(0x00012000);
// => 13
```

## 6. Read the flag message

Boats with no radio signal to the lighthouse with flags, and the harbour master passes their messages on to Lupe as a string.
Each lowercase letter from `'a'` to `'z'` stands for a boat: `'a'` is boat 0, `'b'` is boat 1, and so on up to `'z'`, which is boat 25.
Every other character, such as a space, a comma or a capital letter, is just there to make the message easier to read, and stands for no boat.

Implement the `log_from_flags` function, which takes the message and returns a log of the boats it names.

```c
log_from_flags("a, b, z");
// => 0x02000003
```

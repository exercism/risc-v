# Instructions

Kila runs the mystery club at her secondary school in Goroka.
On Friday, the school band's violin bow went missing from the music room, just before the drama club's dress rehearsal.
Kila is sure that someone only borrowed it, but she wants to find out who.
Kila has collected every student's alibi: the story of where they were, and when.

Kila writes every time as the number of minutes since midnight.
For example, 9:30 in the morning is 570, and 2:15 in the afternoon is 855.

She would like some small functions to check the alibis.
Each one returns 1 for true or 0 for false, and nothing else.

## 1. Put events in order

Implement the `is_earlier` function, which takes two times and returns 1 if the first time is earlier than the second, or 0 if it is not.

```c
is_earlier(600, 615);
// => 1
```

## 2. Compare two stories

Two students say they were together.
Their stories agree only if they give exactly the same time.

Implement the `alibis_agree` function, which takes the two times and returns 1 if they are equal, or 0 if they are not.

```c
alibis_agree(720, 720);
// => 1
```

## 3. Check who was in the room

The music room's sign-in sheet shows the time each student arrived and the time they left.
A student was present at time `t` if they had arrived at or before `t`, and had not yet left.
A student who left at exactly `t` was not present.

Implement the `was_present` function, which takes the arrival time, the leaving time and the time `t`, and returns 1 if the student was present at `t`, or 0 if not.

```c
was_present(600, 660, 630);
// => 1
```

## 4. Name the suspects

A student is a suspect if they were present when the bow went missing **and** they had a motive.

Implement the `is_suspect` function, which takes two booleans, `present` and `has_motive`, each 0 or 1.
It returns 1 if the student is a suspect, or 0 if not.

```c
is_suspect(1, 0);
// => 0
```

## 5. Clear the innocent

A student is cleared if they were away from the music room, **or** if at least one other student saw them somewhere else.

Implement the `is_cleared` function, which takes a boolean `away` (0 or 1) and the number of witnesses (0 or more).
It returns 1 if the student is cleared, or 0 if not.

```c
is_cleared(0, 2);
// => 1
```

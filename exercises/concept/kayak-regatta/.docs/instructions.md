# Instructions

Every year, the school on Aitutaki, in the Cook Islands, holds a regatta in the lagoon.
Crews race kayaks and outrigger canoes, called vaka, around a course marked with buoys.
Families watch from the beach, and the timekeepers sit under a tree with a laptop.

Each crew paddles several laps of the course.
The laptop keeps each lap time in an array of `int32_t`, in tenths of a second, so a lap of 1 minute and 0.5 seconds is stored as 605.
The timekeepers want the results quickly, before the prize-giving.
Your job is to write the functions using vector instructions, so that each instruction works on many laps at once.

Every function in this exercise must work for any number of elements it is allowed, and must not read any element past the last one it is given.

## 1. Add up the laps

Implement the `total_time` function, which takes the address of an array of lap times and the number of laps.
It returns the total time for all the laps.
If there are no laps, it returns 0.

```c
const int32_t laps[] = {600, 612, 598, 605};
total_time(laps, 4);
// => 2415
```

## 2. Find the fastest lap

The fastest lap is the one with the smallest time.

Implement the `fastest_lap` function, which takes the address of an array of lap times and the number of laps.
It returns the smallest lap time.
There is always at least one lap.

```c
const int32_t laps[] = {612, 598, 605, 600, 601};
fastest_lap(laps, 5);
// => 598
```

## 3. Find the best improvement

The timekeepers compare each crew's time with its time at last year's regatta.
A positive change means the crew was that many tenths of a second faster this year.
A negative change means it was slower.

Implement the `best_improvement` function, which takes the address of an array of changes and the number of crews.
It returns the largest change.
There is always at least one crew, and every crew may have been slower.

```c
const int32_t changes[] = {-15, -3, -8};
best_improvement(changes, 3);
// => -3
```

## 4. Find the longest paddle

Each club adds up how far its paddlers went in training over the whole season, in millimetres.
Some clubs paddle more than 2147483647 millimetres, too many for an `int32_t`, so these distances are kept in an array of `uint32_t`.

Implement the `longest_paddle` function, which takes the address of an array of distances and the number of clubs.
It returns the longest distance.
If there are no clubs, it returns 0.

```c
const uint32_t millimetres[] = {1200000000, 3000000000, 2500000000, 900000000};
longest_paddle(millimetres, 4);
// => 3000000000
```

## 5. Add up a team's laps

In the doubles race, each paddler of a two-person team paddles every lap, and their times are added together.
The laptop keeps one array of lap times for each paddler.

Implement the `team_total` function, which takes the addresses of the two paddlers' arrays and the number of laps.
It returns the total time of every lap by both paddlers.
If there are no laps, it returns 0.

```c
const int32_t first[] = {600, 610, 605, 598};
const int32_t second[] = {620, 615, 600, 611};
team_total(first, second, 4);
// => 4859
```

## 6. Find the spread of the laps

A steady crew paddles every lap in nearly the same time.
The spread is the slowest lap time minus the fastest lap time.

Implement the `lap_spread` function, which takes the address of an array of lap times and the number of laps.
It returns the spread, going through the array only once.
There is always at least one lap.

```c
const int32_t laps[] = {612, 598, 605, 640, 601};
lap_spread(laps, 5);
// => 42
```

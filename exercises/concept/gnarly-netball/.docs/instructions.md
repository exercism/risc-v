# Instructions

Your school is hosting this year's inter-school netball competition, and you have offered to help the organisers.
Twelve schools are taking part, from the town and from the villages along the coast.
One team, from a school near the surf beach, calls itself the Gnarly Waves.
(Surfers say "gnarly", with a silent g, for anything wild and exciting.)

The organisers would like some functions to help with the ladder and the other numbers.

## 1. Count the ladder points

Teams move up the ladder by earning points.
A win is worth 4 points, a draw is worth 2 points, and a loss is worth nothing.

Implement the `ladder_points` function, which takes the number of games a team has won and the number it has drawn, and returns the team's points.

```c
ladder_points(3, 1);
// => 14
```

## 2. Find the goals per quarter

A game of netball has four quarters.
The coaches want to know how many goals their team scores in an average quarter, as a whole number.

Implement the `goals_per_quarter` function, which takes the goals a team scored in a game and returns the goals per quarter, ignoring any fraction.

```c
goals_per_quarter(38);
// => 9
```

## 3. Find the average margin

A team's **margin** in a game is its goals minus the other team's goals.
A team that is losing more than it wins has a negative total margin.

Implement the `average_margin` function, which takes a team's total margin over several games and the number of games, and returns the average margin per game.
Ignore any fraction, so that the result moves towards zero: an average of -3.5 becomes -3.

```c
average_margin(-7, 2);
// => -3
```

## 4. Share the oranges

At half-time, the players share a box of oranges.
Every player gets the same number, and any that are left over go back in the box for the umpires.

Implement the `oranges_left` function, which takes the number of oranges and the number of players, and returns the number of oranges left over.
If there are no players, every orange is left over.

```c
oranges_left(30, 7);
// => 2
```

## 5. Book the buses

Each school travels to the competition by bus.
A bus can carry a fixed number of people, and nobody can be left behind, so a part-full bus still counts as a whole bus.

Implement the `buses_needed` function, which takes the number of people travelling and the number of seats on each bus, and returns the number of buses to book.
The number of seats is always at least 1.

```c
buses_needed(41, 20);
// => 3
```

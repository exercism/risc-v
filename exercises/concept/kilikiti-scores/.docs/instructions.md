# Instructions

It is Saturday afternoon on Upolu, and two villages are playing **kilikiti**, the Samoan form of cricket.
The bats are long and carved from wood, the ball is made of rubber, and whole families come to watch and sing.
Each side can have many more batters than in English cricket.

Lagi is the scorer for her village.
In the past she wrote every run in a notebook, but this season the school lent her a laptop.
She keeps the scores in arrays of `int32_t`: one element per batter, or one element per over.
An **over** is a group of balls bowled by one bowler.

Each function receives the address of the first score and, where needed, the number of scores in the array.

## 1. Look up one batter's runs

Implement the `runs_by` function.
It takes an array of batters' scores and the index of one batter, and returns that batter's runs.
The first batter has index 0.

```c
int32_t scores[] = {12, 40, 7, 23};
runs_by(scores, 2);
// => 7
```

## 2. Add up the runs

Implement the `total_runs` function.
It takes an array of scores and the number of scores, and returns their total.
An empty array has a total of 0.

```c
int32_t scores[] = {12, 40, 7, 23};
total_runs(scores, 4);
// => 82
```

## 3. Find the highest score

Implement the `highest_score` function.
It takes an array of batters' scores and the number of scores, and returns the highest score.
There is always at least one score.

```c
int32_t scores[] = {12, 40, 7, 23};
highest_score(scores, 4);
// => 40
```

## 4. Count the ducks

A batter who is out without scoring any runs is said to be out for a **duck**.

Implement the `count_ducks` function.
It takes an array of batters' scores and the number of scores, and returns how many of the scores are 0.

```c
int32_t scores[] = {0, 15, 0, 3, 0};
count_ducks(scores, 5);
// => 3
```

## 5. Add the bonus runs

Sometimes the umpires award bonus runs to every over, for example when the bowling side delays the game.

Implement the `add_bonus` function.
It takes an array of runs per over, the number of overs, and the bonus.
It adds the bonus to every element of the array in memory, and returns nothing.

```c
int32_t overs[] = {6, 2, 9};
add_bonus(overs, 3, 1);
// overs => {7, 3, 10}
```

## 6. Find the first century

A score of 100 or more is called a **century**, and the village sings loudly for the batter who makes one.

Implement the `first_century` function.
It takes an array of batters' scores and the number of scores.
It returns the index of the first score that is 100 or more, or -1 if there is none.

```c
int32_t scores[] = {45, 101, 18, 120};
first_century(scores, 4);
// => 1
```

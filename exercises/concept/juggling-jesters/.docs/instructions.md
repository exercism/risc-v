# Instructions

A small travelling circus is touring the Pacific.
This week its striped big top stands on a beach near Nadi, in Fiji, and next week it sails on to Apia, in Samoa.
There are acrobats, a unicyclist, a tightrope walker and, the crowd's favourite, the juggling jesters.
The jesters wear bright costumes with bells on their hats, and they juggle balls high over the sand.

The ringmaster keeps score of every show.
She would like some small functions to do the sums, and each one may use the ones before it.

## 1. Count the catches

In one round of juggling, every ball is thrown and caught once.

Implement the `catch_count` function, which takes the number of balls and the number of rounds, and returns the number of catches.

```c
catch_count(3, 5);
// => 15
```

## 2. Score an act

The ringmaster gives an act 2 points for every catch, and 10 more points for the bow at the end.

Implement the `act_score` function, which takes the number of balls and the number of rounds, and returns the act's score.
Use `catch_count` to count the catches.

```c
act_score(3, 5);
// => 40
```

## 3. Count the encore

When the crowd cheers loudly enough, the jesters juggle again for an encore, with one more ball than before and the same number of rounds.

Implement the `encore` function, which takes the number of balls and the number of rounds in the first performance.
It returns the total number of catches in the first performance and the encore together.
Use `catch_count` for each of them.

```c
encore(3, 5);
// => 35
```

## 4. Add up the whole show

On a long evening, the jesters come back again and again.
In the first act they juggle 1 ball, in the second act 2 balls, in the third act 3 balls, and so on.
Every act lasts the same number of rounds.

Implement the `show_total` function, which takes the number of acts and the number of rounds in each act, and returns the total number of catches in the whole show.
Use `catch_count` for each act.

```c
show_total(3, 4);
// => 24
```

## 5. Read the score cards

At the end of the show, three children in the front row hold up score cards for the jesters.
A fair mark is between 0 and 10.
The children are very excited, so some cards say more than 10, and some even say less than 0.
The ringmaster counts any mark above 10 as 10, and any mark below 0 as 0.

Implement the `crowd_rating` function, which takes the three marks and returns the total of the fair marks.
Write a helper function, without `.global`, that turns one mark into a fair mark, and call it for each card.

```c
crowd_rating(7, 12, -3);
// => 17
```

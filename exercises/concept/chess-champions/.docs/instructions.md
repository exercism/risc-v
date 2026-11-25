# Instructions

The chess club at your school in Nukuʻalofa, Tonga, meets every Wednesday after classes.
This year, for the first time, the club will send a team of four players to a schools tournament.

The club's coach, Mr Havili, played for Tonga at the Chess Olympiad when he was young.
The club raised money with a car wash to buy a small electronic chess board that they can program, and he asks you to write some functions for it.

The test file gives each chess piece, and each game result, a number code:

| Piece  | Code |
| ------ | ---- |
| pawn   | 1    |
| knight | 2    |
| bishop | 3    |
| rook   | 4    |
| queen  | 5    |
| king   | 6    |

| Result | Code |
| ------ | ---- |
| win    | 1    |
| draw   | 2    |
| loss   | 3    |

Define your own constants for these codes, and for every other number in your solution, with the same values as the test file.

## 1. Count the value of a piece

Chess players count how strong a position is by giving each piece a value:

| Piece  | Value |
| ------ | ----- |
| pawn   | 1     |
| knight | 3     |
| bishop | 3     |
| rook   | 5     |
| queen  | 9     |
| king   | 0     |

The king is given 0, because it can never be captured, so it is not counted.

Implement the `piece_value` function, which takes a piece code and returns the value of that piece.

```c
piece_value(KNIGHT);
// => 3
```

## 2. Promote a pawn

The total value of all of a player's pieces is called their **material**.
When a pawn reaches the far side of the board, it is **promoted**: it becomes a queen.
The player loses a pawn and gains a queen.

Implement the `promote_pawn` function, which takes a player's material before the promotion and returns their material after it.
Work out the change from your piece-value constants, not by hand.

```c
promote_pawn(20);
// => 28
```

## 3. Count the empty squares

A chess board has 8 rows of 8 squares.
The coach likes to ask how many squares are still empty.

Implement the `empty_squares` function, which takes the number of pieces on the board and returns the number of empty squares.
Define the board size as a constant, and work out the number of squares from it.

```c
empty_squares(20);
// => 44
```

## 4. Stay on the board

The board numbers its rows (called **ranks**) and its columns (called **files**) from 0 to 7.
When it works out where a piece could move, it must check that the square is really on the board.

Implement the `on_board` function, which takes a rank and a file and returns 1 if both are from 0 to 7, and 0 otherwise.
Work out the last index from your board size constant.

```c
on_board(2, 9);
// => 0
```

## 5. Score a game

In a tournament, a win scores 1 point, a draw scores half a point, and a loss scores nothing.
To avoid halves, the board counts in **half points**: a win is 2 half points, a draw is 1, and a loss is 0.

Implement the `result_points` function, which takes a result code and returns the number of half points it scores.

```c
result_points(RESULT_DRAW);
// => 1
```

## 6. Decide a team match

In a team match, each of the four players plays one game against a player from the other school.
The team with more half points in total wins the match.

Implement the `match_result` function, which takes your team's half points and the other team's half points, and returns the result code for your team.

```c
match_result(3, 5);
// => RESULT_LOSS
```

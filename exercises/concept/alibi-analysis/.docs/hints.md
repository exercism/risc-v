# Hints

## General

- A comparison instruction writes 1 into its destination register for true, and 0 for false.
- Each function must return exactly 0 or 1 in `a0`.
- You do not need any branches, although you may use them.

## 1. Put events in order

- An earlier time is a smaller number.
- `slt` compares two registers.

## 2. Compare two stories

- Two numbers are equal when their difference is zero.
- `seqz` tests whether a register is zero.

## 3. Check who was in the room

- There are two conditions: `arrive <= t` and `t < leave`.
- `arrive <= t` is the opposite of `arrive > t`, and `xori` with 1 flips a boolean.
- `and` combines two booleans.

## 4. Name the suspects

- Both inputs are already 0 or 1.
- `and` gives 1 only when both of its inputs are 1.

## 5. Clear the innocent

- The number of witnesses could be more than 1, so turn it into 0 or 1 first.
- `snez` gives 1 for any number that is not zero.
- `or` gives 1 when either of its inputs is 1.

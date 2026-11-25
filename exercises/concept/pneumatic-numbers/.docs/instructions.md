# Instructions

A small science museum in Suva has an old calculating machine that runs on air.
Instead of electricity, it uses pipes, valves and bellows: puffs of air push brass wheels round, one tooth at a time.
For many years it sat silent in a glass case.

This year, the science club at a nearby secondary school has cleaned it, mended its leaking pipes and fitted a small computer to control the valves.
The machine works again, and visitors love the hiss and click of its wheels.

The machine has a dial with four **operator codes**:

| Code | Operation | Symbol |
| ---- | --------- | ------ |
| 0    | add       | `+`    |
| 1    | subtract  | `-`    |
| 2    | multiply  | `*`    |
| 3    | divide    | `/`    |

Any other code is not a setting on the dial.

The club would like your help with the control program.

## 1. Show the symbol

The machine's display shows the symbol for the dial's setting.

Implement the `operator_symbol` function, which takes an operator code and returns its symbol, as a character.
For any other code, it returns `'?'`.

```c
operator_symbol(2);
// => '*'
```

## 2. Work out the answer

Implement the `calculate` function, which takes an operator code and two numbers, `a` and `b`, and returns `a + b`, `a - b`, `a * b` or `a / b`.
Use a jump table to choose the operation.

Division throws away any fraction, so the answer moves towards zero, as `div` does.
The machine cannot divide by zero: when `b` is 0, the answer is 0.
For any other operator code, the answer is also 0.

```c
calculate(1, 12, 7);
// => 5
```

## 3. Run a paper tape

The machine can also follow a program punched into a paper tape, one step code at a time.
Each step changes the number shown on the machine's main wheel:

| Code | Step                      |
| ---- | ------------------------- |
| 0    | add one                   |
| 1    | double                    |
| 2    | change the sign           |
| 3    | clear the wheel to zero   |

Any other code is a hole punched by mistake, and the machine skips it.

Implement the `run_program` function, which takes an array of step codes, the number of codes, and the number the wheel starts at.
It returns the number on the wheel after the last step.
Use a jump table to choose the code for each step.

```c
const int32_t codes[] = {0, 1, 0};
run_program(codes, 3, 3);
// => 9
```

## 4. Try a new operation

The club wants visitors to invent their own operations, written as C functions on the museum's computer.

Implement the `apply` function, which takes a pointer to a function, `fn`, and two numbers, `a` and `b`.
It returns `fn(a, b)`.

```c
apply(larger, 4, 9);
// => 9
```

## 5. Hand over the operation

The museum's computer would like to use the machine's own operations as well.

The test file names the type of a pointer to a function that takes two `int32_t` and returns one:

```c
typedef int32_t (*binary_op)(int32_t, int32_t);
```

Implement the `operation` function, which takes an operator code, and returns a pointer to a function that carries out that operation.
The function returned takes `a` and `b`, and returns the same answer as `calculate` does.
For any other operator code, `operation` returns a null pointer.

```c
operation(1)(12, 7);
// => 5
```

# Hints

## General

- Start each function with its base case: test for it, and return the answer straight away.
  The base case makes no call, so it needs no stack frame.
- The recursive case makes a call, so it needs a frame: save `ra`, and each `s` register you use.
- A value you need after the recursive call belongs in an `s` register, not a `t` or `a` register.
  The inner call uses the same `t` and `a` registers for its own work.
- Make sure that every path through the recursive case runs the epilogue before `ret`.
- If a test never finishes or crashes, check that every call moves closer to the base case.

## 1. Grow the colony

- The base case is 0 generations.
- Call `colony_size` with one generation fewer, to get the size of one chick's family.
- Both chicks' families are the same size, so you need only one call.
- Nothing needs to survive the call except `ra`.

## 2. Check the leg bands

- A number less than 10 has only one digit, so it is its own total.
- `rem` by 10 gives the last digit, and `div` by 10 gives the other digits.
- Keep the last digit in an `s` register while the call works out the rest.

## 3. Count the climbing routes

- A chick's first hop is either 1 ledge or 2 ledges.
  After that, it has 1 or 2 ledges fewer left to climb.
- So the routes up `n` ledges are the routes up `n - 1` ledges plus the routes up `n - 2` ledges.
- 0 ledges and 1 ledge each have exactly one route.
- Keep `n` in an `s` register for the second call, and keep the first answer in another `s` register.

## 4. Count the family

- The base case is the empty family: an address of 0.
- Keep the pterodactyl's address in an `s` register, so you can still load its second chick after the first call.
- `lw a0, 4(s0)` loads the first chick's address, ready to pass to the call.
- Add the two families together, and 1 for the pterodactyl itself.

## 5. Count the generations

- Count the generations of each chick's family, and keep whichever is more.
- Then add 1 for the pterodactyl's own generation.
- A branch can choose the larger of two numbers.

## 6. Find the widest wings

- Find the largest wingspan in each chick's family first.
- Then compare three numbers: the two answers and the pterodactyl's own wingspan, at offset 0.
- `larger` changes every `a` register except its result, so load its arguments again before each call.

# Hints

## General

- Keep the `.option arch, +v` line at the top of the file: without it, the assembler rejects every vector instruction.
- Every function uses the strip-mining loop from the vectors concept, with the count tested at the top.
- Keep the answer so far in an ordinary register, such as `t2`, and return it in `a0` at the end.
- In each round, put the answer so far into element 0 of a vector register with `vmv.s.x`, use it as the seed of the reduction, and read the new answer back with `vmv.x.s`.
- Put the `vmv.s.x` after the `vsetvli`, because it does nothing while `vl` is 0.
- A reduction takes the vector to reduce second and the seed third: `vredsum.vs vd, vs2, vs1`.
- Each function needs its own label names.

## 1. Add up the laps

- Start the total at 0, so that no laps gives 0.
- `vredsum.vs` adds up the elements of a vector, plus the seed.

## 2. Find the fastest lap

- `vredmin.vs` finds the smallest of the seed and the elements.
- There is always at least one lap, so the first lap time, loaded with `lw`, makes a good seed.
- A seed of 0 would be wrong: 0 is smaller than every lap time.

## 3. Find the best improvement

- `vredmax.vs` finds the largest of the seed and the elements.
- If every change is negative, a seed of 0 would win, so seed with the first change, or with the smallest `int32_t`.

## 4. Find the longest paddle

- Distances of 2147483648 or more have their top bit set, so `vredmax.vs` would treat them as negative.
- `vredmaxu.vs` compares unsigned numbers.
- 0 is the smallest unsigned number, so it is a good seed, and it is also the answer for no clubs.

## 5. Add up a team's laps

- Load a strip from each array into two different vector registers, add them with `vadd.vv`, and then reduce the sums.
- Both pointers move on by the same number of bytes each round.

## 6. Find the spread of the laps

- You need both the smallest and the largest lap time, so keep two answers so far, in two ordinary registers.
- In each round, do a `vredmin.vs` and a `vredmax.vs` on the same loaded strip, with a different seed register for each.
- Subtract after the loop.

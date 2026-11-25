# Introduction

So far, element `i` of a result has always come from element `i` of the inputs.
Sometimes you want to **move** elements instead: reverse them, take every second one, swap neighbours, or shift them along by one place.
Changing the order of elements is called a **permutation**.

NumPy has many ways to do this:

```python
notes = np.array([10, 20, 30, 40, 50])
print(notes[::-1])                  # [50 40 30 20 10]  reversed
print(notes[::2])                   # [10 30 50]        every second element
print(notes[[1, 0, 3, 2]])          # [20 10 40 30]     chosen by an index array
print(notes[notes > 25])            # [30 40 50]        chosen by a mask
print(np.roll(notes, 1))            # [50 10 20 30 40]  shifted along by one
```

The vector extension can do all of these.
Some of them happen while loading or storing, and some of them happen inside a vector register.

## Strided loads and stores

`vle32.v` loads elements that sit next to each other in memory, 4 bytes apart.
A **strided** load lets you choose the distance between the elements, called the **stride**.
The stride is a number of **bytes**, and it goes in an ordinary register:

```asm
        li      t2, 12                  # stride: 12 bytes, so every third int32_t
        vlse32.v v8, (a1), t2           # v8[i] = the word at address a1 + i * 12
        vsse32.v v8, (a0), t2           # the word at address a0 + i * 12 = v8[i]
```

```python
v8 = a[::3]
```

The `s` in `vlse32` and `vsse32` stands for "strided".
A stride can be negative, to walk backwards through memory.
With a stride of -4, element 0 comes from the address you give, element 1 from the word just before it, and so on.

In a strip-mining loop, a strided load reads `vl` elements, and they cover `vl * stride` bytes.
So the pointer must move on by `vl * stride` bytes each round, not by `vl * 4`.

## Element numbers: `vid.v`

`vid.v` writes each element's own index into it:

```asm
        vid.v   v9                      # v9 = [0, 1, 2, 3, ...], up to vl - 1
```

```python
v9 = np.arange(vl)
```

This is useful for building **index vectors**, which say where each element should come from.
Ordinary vector arithmetic turns it into other patterns.
The bitwise instructions have vector forms too, such as `vand`, `vor` and `vxor`, with `.vv`, `.vx` and `.vi` endings:

```asm
        vid.v   v9                      # v9 = [0, 1, 2, 3, 4, 5, ...]
        vadd.vv v9, v9, v9              # v9 = [0, 2, 4, 6, 8, 10, ...]
        vand.vi v10, v9, 3              # v10 = [0, 2, 0, 2, 0, 2, ...]
```

## Gathering: `vrgather`

`vrgather.vv` builds a new vector by picking elements of another vector, using an index vector:

```asm
        vrgather.vv v10, v8, v9         # v10[i] = v8[v9[i]]
```

```python
v10 = v8[v9]
```

The operands are, in order: the destination, the vector to pick from, and the index vector.
If an index is too big for the register, the element becomes 0.
For example, with `v8 = [10, 20, 30, 40]` and `v9 = [3, 3, 0, 1]`, `v10 = [40, 40, 10, 20]`.

The destination must be a different register from both sources.
`vrgather.vv v8, v8, v9` is not allowed, because the instruction would overwrite elements that it still needs to read.

`vrgather.vx` and `vrgather.vi` use one index for every element.
They copy one element into the whole vector:

```asm
        vrgather.vi v10, v8, 2          # v10[i] = v8[2] for every i
```

## Sliding

A **slide** moves every element along by some number of places.

```asm
        vslidedown.vx v10, v8, t1       # v10[i] = v8[i + t1]
        vslideup.vx v10, v8, t1         # v10[i + t1] = v8[i]; v10[0] to v10[t1 - 1] are not changed
```

`vslidedown` moves elements towards element 0.
Elements that would come from past the end of the register become 0.
`vslideup` moves elements away from element 0, and leaves the first `t1` elements of the destination as they were.
Both also have a `.vi` form, with an immediate number of places.

Two more slides move by exactly one place, and fill the gap with an ordinary register:

```asm
        vslide1up.vx v10, v8, a3        # v10[0] = a3, v10[i] = v8[i - 1]
        vslide1down.vx v10, v8, a3      # v10[i] = v8[i + 1], v10[vl - 1] = a3
```

```python
v10 = [a3] + v8[:vl - 1]
v10 = v8[1:vl] + [a3]
```

For `vslideup` and `vslide1up`, the destination must be a different register from the source.

`vslidedown` is also a way to read any element you like into an ordinary register.
Slide that element down to element 0, then use `vmv.x.s`:

```asm
        vslidedown.vi v10, v8, 2        # v10[0] = v8[2]
        vmv.x.s t2, v10                 # t2 = v8[2]
```

## Compressing: `vcompress.vm`

`vcompress.vm` packs the elements chosen by a mask together, at the start of the destination:

```asm
        vmsgt.vi v0, v8, 0              # mask: v8[i] > 0
        vcompress.vm v10, v8, v0        # v10 = the elements of v8 whose mask bit is 1, in order
        vcpop.m t3, v0                  # t3 = how many there are
```

```python
v10 = v8[v8 > 0]
```

For example, with `v8 = [5, -1, 7, 0]` and the mask `[1, 0, 1, 0]`, `v10` starts `[5, 7, ...]`.
Only the first `t3` elements of `v10` have meaning: the rest are tail elements.
To store just those, set `vl` to `t3` with another `vsetvli` before the store.
`vsetvli` never changes the contents of the vector registers.

The destination of `vcompress` must be different from its source and from the mask.
`vcompress` is never masked with `v0.t`: the mask is its third operand.

## Permutations in a strip-mining loop

In a strip-mining loop, each round works on one strip, and a vector instruction only sees the elements of that strip.
So a permutation inside a register only moves elements **within** a strip.

When elements must move from one strip to another, you need a plan:

- **Choose where to read and write.**
  Each round can read from, or write to, any part of the array.
  The pointers do not have to start at the beginning, and do not have to move forwards.
- **Carry values in ordinary registers.**
  Keep a value from one round in an ordinary register, and put it into the next round's vector.
- **Keep pairs inside one strip.**
  `vl` may be odd, even when the count is even, unless you choose it yourself.
  `vsetvli` gives exactly the count you ask for when it fits in a register, so you can work out an even count and ask for that.

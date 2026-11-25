# About

A **permutation** changes the order of elements.
RVV has two kinds: memory accesses that read or write elements in a chosen order, and instructions that move elements between positions inside a vector register.

## Strided loads and stores

| Instruction                 | Element `i`                                   |
| --------------------------- | --------------------------------------------- |
| `vlse32.v vd, (rs1), rs2`   | `vd[i] = ` the word at `rs1 + i * rs2`        |
| `vsse32.v vs3, (rs1), rs2`  | the word at `rs1 + i * rs2 = vs3[i]`          |

The stride `rs2` is a signed number of bytes.
`vlse8.v`, `vlse16.v` and `vlse64.v` (and the matching stores) work the same way for other element widths.

- A stride of `4 * k` reads every `k`th `int32_t`, like `a[::k]`.
- A negative stride walks backwards, like `a[::-1]`.
- A stride of 0 reads the same word into every element.
- A stride equal to the size of a struct reads one field from each struct in an array of structs.

Strided accesses can be slower than unit-stride `vle32.v` on real hardware, because the elements may be spread over many cache lines.
They can be masked with `v0.t`.

## Indexed loads and stores

Indexed ("gather/scatter") accesses take a vector of byte offsets:

| Instruction                    | Element `i`                                         |
| ------------------------------ | --------------------------------------------------- |
| `vluxei32.v vd, (rs1), vs2`    | `vd[i] = ` the word at `rs1 + vs2[i]`               |
| `vloxei32.v vd, (rs1), vs2`    | the same, but the elements are accessed in order    |
| `vsuxei32.v vs3, (rs1), vs2`   | the word at `rs1 + vs2[i] = vs3[i]`                 |
| `vsoxei32.v vs3, (rs1), vs2`   | the same, in order                                  |

The offsets are bytes, so an index vector of element numbers must be shifted left by 2 first (`vsll.vi`).
`u` means unordered and `o` means ordered; the difference matters only for memory-mapped devices and for scatters that write the same address twice.

## `vid.v` and `viota.m`

| Instruction          | Element `i`                                           |
| -------------------- | ----------------------------------------------------- |
| `vid.v vd`           | `i`                                                   |
| `viota.m vd, vs2`    | how many of mask bits `0` to `i - 1` of `vs2` are set |

`viota.m` gives, for each chosen element, the place it would have after compressing, so with an indexed store it can do a "scatter compress".

Useful index vectors built from `vid.v`:

| Index vector   | Built with                                 | Effect with `vrgather.vv`   |
| -------------- | ------------------------------------------ | --------------------------- |
| `i ^ 1`        | `vxor.vi`                                  | swap neighbouring pairs     |
| `vl - 1 - i`   | `vrsub.vx` with `vl - 1`                   | reverse the strip           |
| `2 * i`        | `vadd.vv` with itself, or `vsll.vi`        | every second element        |
| `i & ~3`       | `vand.vi` with -4                          | repeat the first of each group of four |

## Gathers

| Instruction                    | Element `i`                                          |
| ------------------------------ | ---------------------------------------------------- |
| `vrgather.vv vd, vs2, vs1`     | `vs1[i] < VLMAX ? vs2[vs1[i]] : 0`                   |
| `vrgather.vx vd, vs2, rs1`     | `rs1 < VLMAX ? vs2[rs1] : 0`                         |
| `vrgather.vi vd, vs2, uimm`    | `uimm < VLMAX ? vs2[uimm] : 0`, `uimm` from 0 to 31  |
| `vrgatherei16.vv vd, vs2, vs1` | like `vrgather.vv`, but the indices are 16 bits wide |

The indices are unsigned.
**VLMAX** is the number of elements that fit in the register group, not `vl`.
An index from `vl` to VLMAX - 1 reads a tail element of `vs2`, whose value is whatever happens to be there, so keep indices below `vl`.

`vd` must not overlap `vs2` or `vs1`.
`vrgatherei16` is useful for byte elements, whose 8-bit indices could not reach past element 255.

## Slides

| Instruction                    | Element `i` of `vd`                                          |
| ------------------------------ | ------------------------------------------------------------ |
| `vslideup.vx vd, vs2, rs1`     | `vs2[i - rs1]` for `i >= rs1`; unchanged for `i < rs1`       |
| `vslidedown.vx vd, vs2, rs1`   | `vs2[i + rs1]` if `i + rs1 < VLMAX`, else 0                  |
| `vslide1up.vx vd, vs2, rs1`    | `rs1` for `i = 0`, else `vs2[i - 1]`                         |
| `vslide1down.vx vd, vs2, rs1`  | `vs2[i + 1]` for `i < vl - 1`, and `rs1` for `i = vl - 1`    |

`vslideup` and `vslidedown` also have a `.vi` form with an immediate from 0 to 31.

- `vslideup` never writes the elements below the offset, whatever the tail and mask policies.
  So two strips can be joined: load the second half at an offset into a register that already holds the first half.
- `vslidedown` reads up to VLMAX, not up to `vl`, so with a short `vl` it can bring in tail elements of `vs2`.
- For `vslideup` and `vslide1up`, `vd` must not overlap `vs2`.
  `vslidedown` and `vslide1down` allow it.
- `vslidedown` by `k`, followed by `vmv.x.s`, reads element `k` into an ordinary register.

`vslide1up` with the previous strip's last element carried in a register is the usual way to compute differences or "previous element" arrays, `a[i] - a[i - 1]`, across a strip-mining loop.

## `vcompress.vm`

```asm
        vcompress.vm vd, vs2, vs1   # pack the elements of vs2 whose bit in mask vs1 is 1
```

The chosen elements go into `vd[0]`, `vd[1]`, … in order.
The elements after them, up to `vl - 1`, are tail elements: kept with `tu`, possibly all ones with `ta`.
`vcpop.m` on the same mask gives the number of chosen elements.

`vd` must not overlap `vs2` or `vs1`, and `vcompress` cannot itself be masked with `v0.t`.
The mask can be in any register.

There is no "expand" instruction, the opposite of compress.
Use `viota.m` to build indices, then a masked `vrgather.vv`.

## Permutations across strips

A strip-mining loop sees one strip at a time, so a register permutation moves elements only within a strip.
For a permutation of the whole array:

- **Reverse:** walk the source from its end, and either load with stride -4, or load forwards and reverse each strip with `vrgather.vv` and the index vector `vl - 1 - vid`.
- **Shift by one:** carry the last element of each strip into the next round in an ordinary register, and insert it with `vslide1up.vx`.
  Or load from `src - 4`, after handling the first element separately.
- **Pairs or groups:** make sure each strip holds whole groups.
  For `AVL <= VLMAX`, `vsetvli` gives `vl = AVL` exactly.
  For `VLMAX < AVL < 2 * VLMAX`, the specification allows any `vl` from `ceil(AVL / 2)` to VLMAX, which may be odd, so round `vl` down to a multiple of the group size and call `vsetvli` again.
- **Filter:** `vcompress.vm` per strip, and move the destination pointer on by the count kept, not by `vl`.

## Comparing with Python and NumPy

| Python / NumPy           | RVV                                                     |
| ------------------------ | ------------------------------------------------------- |
| `a[::2]`                 | `vlse32.v` with stride 8                                |
| `a[::-1]`                | `vlse32.v` with stride -4 from the last element         |
| `b[::2] = a`             | `vsse32.v` with stride 8                                |
| `np.arange(n)`           | `vid.v`                                                 |
| `a[idx]`                 | `vrgather.vv`, or `vluxei32.v` from memory              |
| `np.full(n, a[k])`       | `vrgather.vx` or `vrgather.vi`                          |
| `a[mask]`                | `vcompress.vm`, with `vcpop.m` for the length           |
| `np.roll(a, 1)`          | `vslideup` / `vslide1up`, carrying across strips        |
| `np.roll(a, -1)`         | `vslidedown` / `vslide1down`                            |
| `np.diff(a)`             | `vslide1up.vx` and `vsub.vv`                            |

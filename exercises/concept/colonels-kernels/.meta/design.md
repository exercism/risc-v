# Design

## Learning objectives

- Know that a C struct is a block of memory holding its fields in the order they are declared, each at a fixed offset.
- Read a field through a pointer to a struct with the load that matches its type, at the field's offset.
- Update a field in place with a load, change and store of the right size.
- Work out offsets by hand: each field is aligned to its own alignment, with padding before it if needed.
- Know that a struct's size is rounded up to a multiple of its largest alignment, so arrays of structs stay aligned.
- Know that element `i` of an array of structs is at `base + i * size`, where the size includes the padding.
- Use `mul` for a size that is not a power of 2, and walk a pointer by the size.
- Keep an index alongside a walking pointer, to return the index of an element.
- Relate a struct to a Python dataclass, or a dict with fixed keys.

## Out of scope

- Byte fields (`uint8_t`, `char`) and strings: `lb`, `lbu`, `sb` (the `strings` concept).
- Naming offsets with `.equ` (the `constants` concept is not a prerequisite; mentioned in `about.md` only).
- Structs passed or returned by value, nested structs and unions (covered in `about.md` only).
- 64-bit fields.
- Comparison instructions such as `slt`, and unsigned branches.
- Calling other functions, and the stack.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `structs`

## Prerequisites

- `arrays`: element addresses, walking a pointer to an end pointer, empty arrays, keeping an index.
- `halfwords`: `lh`, `lhu`, `sh` and 2-byte alignment, for the `int16_t` fields.
  Through them, `memory`, `loops`, `shifts`, `bitwise-operations`, `conditionals`, `multiplication-and-division` and `basics`.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: reading `weight_grams` at offset 2, or `aisle` of a crate at offset 6, which ignores the padding.
- `essential`: stepping by 10 through an array of crates, which forgets the padding at the end of the struct.
- `essential`: using `lw` for an `int16_t` field, which also reads the neighbouring field.
- `essential`: using `sh` for `stock` in `restock`, or `sw` for an `int16_t` field.
- `essential`: a do-while loop in `stock_value`, `items_in_aisle` or `items_to_shelve`, which reads an element of an empty array.
- `actionable`: working out `12 * count` with `slli` by 3 and by 2 and an `add` is correct, but `li` and `mul` are clearer.
- `informative`: the offsets could be named with `.equ`, once the student has met constants.

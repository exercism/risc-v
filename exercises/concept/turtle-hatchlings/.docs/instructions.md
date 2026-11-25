# Instructions

Hawksbill turtles come ashore at night on the beaches of Aleipata, at the eastern end of Upolu in Samoa, to lay their eggs in the sand.
A group of volunteers from the village school helps to look after the nests until the baby turtles, called hatchlings, dig their way out and run down to the sea.

A nest holds many eggs, but to keep their records simple, the volunteers follow 32 marked places in each nest, numbered 0 to 31.
They keep the record for a nest as a single `uint32_t`.
Bit `i` is 1 if the egg in place `i` has hatched, and 0 if it has not.

For example, the record `0x00000019` is `0b11001`, which means that the eggs in places 0, 3 and 4 have hatched.

The volunteers' new data logger has a RISC-V processor with the Zbb extension, and they would like some quick functions for it.

## 1. Count the hatchlings

Implement the `hatched` function, which takes a nest's record and returns the number of eggs that have hatched.

```c
hatched(0x00000019);
// => 3
```

## 2. Find the first hatchling

Implement the `first_hatched` function, which takes a nest's record and returns the lowest place number whose egg has hatched.
If no egg has hatched yet, it returns -1.

```c
first_hatched(0x00000140);
// => 6
```

## 3. Find the last hatchling

Implement the `last_hatched` function, which takes a nest's record and returns the highest place number whose egg has hatched.
If no egg has hatched yet, it returns -1.

```c
last_hatched(0x00000140);
// => 8
```

## 4. Count the eggs still waiting

Not every marked place holds an egg.
When a nest is first marked, the volunteers record which places hold an egg, in the same form as the hatching record.

Each morning they check the nest again.
Sometimes a ghost crab digs through a place that never held an egg, and the morning check marks it as hatched by mistake.
Those places must be ignored.

Implement the `still_waiting` function, which takes the record of places that hold an egg and the morning's hatching record.
It returns the number of eggs that have not hatched yet.

```c
still_waiting(0x000000FF, 0x0000000F);
// => 4
```

## 5. Read the sand thermometer

Whether a hatchling is male or female depends on how warm the sand is while the egg grows.
At about 29 °C, there are equal numbers of each.

A thermometer buried next to the nest reports how far the sand is from 29 °C, in tenths of a degree.
For example, 12 means 30.2 °C, and -7 means 28.3 °C.
Sometimes it reports a value far outside the range it can really measure, and the logger must pull such a value back into that range.

Implement the `clamp_reading` function, which takes a reading, the lowest value the thermometer can measure, and the highest.
It returns the reading if it is within the range, the lowest value if the reading is below the range, and the highest value if it is above the range.

```c
clamp_reading(-73, -50, 50);
// => -50
```

## 6. Plan the night patrol

The 32 volunteers are numbered 0 to 31, and some of them patrol the beach each night to watch for turtles coming ashore.
The patrol rota is a `uint32_t` in which bit `i` is 1 if volunteer `i` is on patrol.

Each night, every patrol turn passes to the volunteer with the next number up.
The turn of volunteer 31 passes back round to volunteer 0.

Implement the `rotate_watch` function, which takes tonight's rota and a number of nights, and returns the rota for that many nights from now.

```c
rotate_watch(0x80000001, 1);
// => 0x00000003
```

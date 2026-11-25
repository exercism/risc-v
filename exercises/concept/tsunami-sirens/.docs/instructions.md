# Instructions

Every year, schools along the coast of Samoa practise a tsunami drill.
When the siren sounds, students and teachers walk calmly, in their class groups, along a marked path to high ground.

At your school in Apia, the science club is building a model warning panel for this year's drill.
A small tide gauge in the harbour measures how far the sea is above its normal level, in centimetres.
The panel turns that reading into an alert level and decides how long the siren should sound.

The club would like you to write the functions inside the panel.

## 1. Pick the larger reading

The panel has two tide gauges, one at each end of the harbour wall.
To be safe, it always uses the larger of the two readings.

Implement the `larger` function, which takes two readings and returns the larger one.
Readings can be negative when the tide is low.

```c
larger(12, 30);
// => 30
```

## 2. Measure the change in the tide

Every few minutes the panel compares the new reading with the one before it.
It only cares about how big the change is, not whether the sea went up or down.

Implement the `tide_change` function, which takes the earlier reading and the later reading, and returns the size of the change between them.
The result is never negative.

```c
tide_change(150, 120);
// => 30
```

## 3. Choose an alert level

The panel shows one of four alert levels, depending on the height of the sea above normal:

| Height above normal       | Level | Meaning                                |
| ------------------------- | ----- | -------------------------------------- |
| less than 30 cm           | 0     | no threat                              |
| 30 cm to less than 100 cm | 1     | stay out of the water and off the beach |
| 100 cm to less than 300 cm | 2    | move to high ground                    |
| 300 cm or more            | 3     | move to high ground, as far as you can |

Implement the `alert_level` function, which takes the height in centimetres and returns the alert level.

```c
alert_level(45);
// => 1
```

## 4. Decide whether to walk to high ground

Students learn an important rule in the drill: if you feel a strong earthquake, or see the sea suddenly drain away, go to high ground straight away, without waiting for a siren.
In a real emergency, always follow official warnings and the advice of your local authorities.

Implement the `should_evacuate` function, which takes the alert level and a second argument that is non-zero if a strong earthquake was felt.
It returns 1 if everyone should walk to high ground, and 0 otherwise.
Everyone should go when the alert level is 2 or more, or when a strong earthquake was felt.

```c
should_evacuate(1, 1);
// => 1
```

## 5. Sound the siren

Each alert level has its own siren length:

| Level | Siren length |
| ----- | ------------ |
| 0     | 0 minutes (silent) |
| 1     | 2 minutes    |
| 2     | 5 minutes    |
| 3     | 10 minutes   |

Implement the `siren_minutes` function, which takes an alert level from 0 to 3 and returns how many minutes the siren should sound.

```c
siren_minutes(2);
// => 5
```

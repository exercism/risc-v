# Instructions

The marine science club at a high school in Neiafu, Tonga, is helping to look after the coral reefs of Vavaʻu.
Coral is alive, and it is very sensitive to heat.
If the sea stays too warm for too long, the coral turns white and may die.
This is called **coral bleaching**.

Every month, the club's divers fix small temperature loggers to the reef and collect the old ones.
To save memory, each logger stores its readings as an array of `int16_t`.
A reading is the difference from a baseline of 27.00 °C, counted in hundredths of a degree.
So 150 means 28.50 °C, and -75 means 26.25 °C.
Readings can be negative, because the water is often cooler than the baseline.

Each logger also records how deep it sits, in millimetres, as a `uint16_t`.
Some loggers sit far down the outer wall of the reef, so a depth can be as large as 60000 mm, which is 60 metres.

Each function receives the address of the first element and, where needed, the number of elements.

## 1. Read one temperature

Implement the `reading_at` function.
It takes a logger's array of readings and an index, and returns the reading at that index.
The first reading has index 0.

```c
int16_t log[] = {120, -35, 210, 88};
reading_at(log, 1);
// => -35
```

## 2. Find the coolest reading

Implement the `coolest` function.
It takes an array of readings and the number of readings, and returns the lowest reading.
There is always at least one reading.

```c
int16_t log[] = {120, -35, 210, -80, 88};
coolest(log, 5);
// => -80
```

## 3. Add up the changes

Some loggers save memory in another way.
They store how much the temperature has changed since the hour before, instead of the temperature itself.
Adding up all the changes gives the total change over the whole time.

Implement the `total_change` function.
It takes an array of hourly changes and the number of changes, and returns their sum as an `int32_t`.
An empty array has a total of 0.
The total may be too big to fit in an `int16_t`, but it always fits in an `int32_t`.

```c
int16_t changes[] = {25, -10, 40, -5};
total_change(changes, 4);
// => 50
```

## 4. Find the deepest logger

Implement the `deepest` function.
It takes an array of depths in millimetres and the number of depths, and returns the greatest depth.
The depths are `uint16_t`, from 0 to 60000.
There is always at least one depth.

```c
uint16_t depths[] = {350, 42000, 1800};
deepest(depths, 3);
// => 42000
```

## 5. Calibrate a logger

When a logger comes back up, the club checks it in a bucket of water next to a trusted thermometer.
Sometimes a logger reads a little too high or too low, so every reading must be corrected by the same amount.

Implement the `calibrate` function.
It takes an array of readings, the number of readings, and an offset in hundredths of a degree.
It adds the offset to every reading in memory, and returns nothing.
The corrected readings always fit in an `int16_t`.

```c
int16_t log[] = {120, -35, 210};
calibrate(log, 3, -20);
// log => {100, -55, 190}
```

## 6. Count the risky readings

Implement the `count_bleaching_risk` function.
It takes an array of readings, the number of readings, and a threshold.
It returns how many readings are at or above the threshold.

```c
int16_t log[] = {120, 250, 90, 300, 250};
count_bleaching_risk(log, 5, 250);
// => 3
```

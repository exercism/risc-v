# Instructions

You are helping out in the office of a ferry company in Tonga.
Its ferry, the _Vaka Fiefia_, carries passengers, cars and cargo between Nukuʻalofa and Vavaʻu, with a stop in Haʻapai on the way.

The booking system records every time as a `uint64_t`: the number of microseconds since midnight on 1 January 1970 (UTC).
A microsecond is a millionth of a second.
For example, 06:00 on 25 November 2026 in Tonga is `1795539600000000`.

These numbers are far too big for one 32-bit register, so each one arrives as a pair: the low half in one register and the high half in the next.
The office manager would like a few small functions to work with them.

## 1. Add a delay

Sometimes the ferry has to wait for the weather, or for a late truck of cargo.

Implement the `add_delay` function, which takes a time and a delay in microseconds, and returns the new time.
The delay is a `uint32_t`, so it is never more than about 71 minutes.

```c
add_delay(1795539600000000, 900000000);
// => 1795540500000000
```

## 2. Work out the journey time

Implement the `journey_time` function, which takes a departure time and an arrival time, and returns how many microseconds the journey took.
The arrival time is never earlier than the departure time.

```c
journey_time(1795539600000000, 1795608000000000);
// => 68400000000
```

## 3. Put departures in order

Implement the `departs_before` function, which takes two departure times.
It returns 1 if the first departure is strictly earlier than the second, and 0 if it is not.

```c
departs_before(1795539600000000, 1795608000000000);
// => 1
```

## 4. Convert minutes to microseconds

The timetable on the office wall gives times in minutes, but the booking system needs microseconds.
There are 60000000 microseconds in a minute.

Implement the `minutes_to_us` function, which takes a whole number of minutes, as a `uint32_t`, and returns the same length of time in microseconds.

```c
minutes_to_us(72);
// => 4320000000
```

## 5. Work out the arrival time

Implement the `arrival_time` function, which takes a departure time and a journey length in whole minutes, and returns the arrival time.

```c
arrival_time(1795539600000000, 1140);
// => 1795608000000000
```

# Instructions

Your aunt runs the busy post office on the main street of Lae.
Every morning, people queue to post parcels, pay bills and collect letters.
To keep the queue fair, there is a "take a number" machine by the door.
Each customer pulls a paper ticket with a number on it, then waits for that number to appear on the screen above one of the counters.

The machine has been running since the post office opened.
Its ticket counter is a 32-bit unsigned number, a `uint32_t`.
After ticket 4294967295 (`0xFFFFFFFF`), the next ticket is 0.
The machine also keeps times in **microseconds** (millionths of a second), again as `uint32_t` numbers: how long each counter has been open today, and how long customers have waited.

A new screen is being fitted, and your aunt has asked you to write the functions that drive it.

## 1. Print the next ticket

Implement the `next_ticket` function, which takes the number on the last ticket printed and returns the number on the next one.
After `0xFFFFFFFF` comes 0.

```c
next_ticket(41);
// => 42
```

## 2. Count the people waiting

The screen shows the ticket now being served.
Everyone holding a later ticket, up to the last ticket printed, is still waiting.

Implement the `tickets_waiting` function, which takes the ticket now being served and the last ticket printed, and returns how many people are still waiting.
This must still work after the counter has wrapped around to 0.

```c
tickets_waiting(40, 47);
// => 7

tickets_waiting(0xFFFFFFFE, 3);
// => 5
```

## 3. Check the counter number

The post office has 12 counters, numbered 1 to 12.
When a clerk opens a counter, they type its number into the machine.

Implement the `is_valid_counter` function, which takes a counter number and returns 1 if it is between 1 and 12, or 0 if it is not.
Use only one comparison.

```c
is_valid_counter(7);
// => 1

is_valid_counter(13);
// => 0
```

## 4. Show the longer-open counter first

Sometimes two counters call a ticket at the same moment.
Then the screen shows the call from the counter that has been open longer first.

Implement the `served_first` function, which takes how long counter A and counter B have been open today, in microseconds.
It returns 1 if counter A has been open longer than counter B, or 0 if not.

```c
served_first(3000000000, 1000000);
// => 1
```

## 5. Work out the average wait

At closing time, the machine adds up the waiting time of every customer that day.

Implement the `average_wait` function, which takes the total waiting time in microseconds and the number of customers.
It returns the average waiting time in microseconds, rounded down.
If there were no customers, it returns 0.

```c
average_wait(4000000000, 10);
// => 400000000
```

## 6. Show the seconds

Above each counter, the screen shows how long that counter has been open, as minutes and seconds, such as `66:40`.

Implement the `seconds_part` function, which takes a time in microseconds and returns the seconds part to show: the number of whole seconds, after taking away the whole minutes.

```c
seconds_part(4000000000);
// => 40
```

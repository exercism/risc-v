# Instructions

The nature club at your college on Upolu is running a bird count this term.
A visiting birdwatcher from Aotearoa New Zealand is helping.
Her name is Wren, after the tiny wrens of her home country, such as the tītipounamu.

Every afternoon the club walks the forest track behind the school with notebooks and binoculars.
They record the fuia, the noisy Samoan starling, and keep hoping for a glimpse of the manumea, the tooth-billed pigeon.
The manumea is Samoa's national bird, lives nowhere else in the world, and is now very rare.

Wren would like some small functions to help the club check its reckoning.

## 1. Add up the fuia

As the students get better at spotting, they record one more fuia each day than the day before: 1 on the first day, 2 on the second, 3 on the third, and so on.

Implement the `total_sightings` function, which takes the number of days and returns the total number of fuia recorded over those days.
The number of days is never negative.

```c
total_sightings(4);
// => 10
```

## 2. Train more spotters

Mrs Fiso, the club's science teacher, knows every forest bird by its call and its Samoan name.
She is busy with her classes, so she has taught the calls to Wren, and at the start Wren is the one trained spotter on the club's walks.
Each week, every trained spotter teaches one more person, so the number of trained spotters doubles every week.

Implement the `weeks_to_train` function, which takes the number of spotters the club needs and returns how many weeks it takes until there are at least that many.

```c
weeks_to_train(5);
// => 3
```

## 3. Size the tally boxes

The record sheet has one box for each digit of a count.

Implement the `digit_count` function, which takes a count and returns the number of digits needed to write it.
The count is never negative, and writing 0 needs one digit.

```c
digit_count(345);
// => 3
```

## 4. Check a band number

Some birds carry a small numbered band on one leg.
To catch mistakes when copying a band number into the notebook, Wren adds up its digits and writes the sum beside it.

Implement the `digit_sum` function, which takes a band number and returns the sum of its digits.
The band number is never negative.

```c
digit_sum(4721);
// => 14
```

## 5. Read the band the right way round

A band is easy to read from the wrong side.
Sometimes a student writes down its digits in reverse order.

Implement the `reverse_digits` function, which takes a number and returns the number made by writing its digits in reverse order.
Zeros that end up at the front disappear, so 1200 becomes 21.
The number is never negative.

```c
reverse_digits(1234);
// => 4321
```

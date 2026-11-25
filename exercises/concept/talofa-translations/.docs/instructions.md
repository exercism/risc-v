# Instructions

The Samoan language club at your school in Auckland is building a phrasebook kiosk for the school library.
Students from all over the Pacific use the library, so the kiosk will greet them in Samoan, Tongan and Tok Pisin.
Hello is _Talofa_ in Samoan, _Mālō e lelei_ in Tongan and _Gude_ in Tok Pisin.
The club has an old computer with a small screen, and you have offered to write some of its functions.

The functions that return text return the address of a string that your program keeps in its read-only data.
Copy each phrase exactly as it is written here, including capital letters, punctuation, the long vowels such as `ā`, and the `ʻ` in `Faʻafetai`.

## 1. Greet the visitor

When someone walks up to the kiosk, the screen shows a warm Samoan welcome.

Implement the `greeting` function, which returns the string `Talofa lava!`.

```c
greeting();
// => "Talofa lava!"
```

## 2. Say thank you

The kiosk can say thank you in three languages.
The test file uses these codes for them:

| Code | Language  | Thank you   |
| ---- | --------- | ----------- |
| 0    | Samoan    | `Faʻafetai` |
| 1    | Tongan    | `Mālō`      |
| 2    | Tok Pisin | `Tenkyu`    |

Implement the `thank_you` function, which takes a language code and returns the phrase for that language.
For any other code, it returns `Thank you`.

```c
thank_you(2);
// => "Tenkyu"
```

## 3. Count the days in a month

Each day, the kiosk shows a new phrase of the day.
To plan the calendar, the club needs to know how many days each month has.
This year is not a leap year, so February has 28 days.

Implement the `days_in_month` function, which takes a month from 1 (January) to 12 (December) and returns its number of days.
Keep the twelve numbers in a table of bytes in your read-only data.

```c
days_in_month(4);
// => 30
```

## 4. Number the phrase of the day

The club has written 365 phrases, numbered from 1, one for each day of the year.
The phrase for a day is the number of that day in the year: 1 January is day 1, and 1 February is day 32.

Implement the `day_of_year` function, which takes a month from 1 to 12 and a day of that month, and returns the day of the year.
A table of words, holding the number of days in the year before each month starts, makes this quick.

```c
day_of_year(2, 14);
// => 45
```

## 5. Count the visitors

The librarian would like to know how many people use the kiosk.
The count starts at 0 when the kiosk is switched on.

Implement the `visitors` function, which adds 1 to the count and returns the new count.
Keep the count in your writable data, so that it lasts from one call to the next.

```c
visitors();
// => 1
visitors();
// => 2
```

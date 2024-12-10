# Advent of Code 2024
## Stephen Houser

Contains my solutions to the [Advent of Code 2024](https://adventofcode.com/2024).

## Tools

- C++ 17 (only standard libraries)
- Visual Studio Code
- macOS / Linux
- git / GitHub

## Leaderboards

- [Stephen's Leaderboard](https://adventofcode.com/2023/leaderboard/private/view/1942246) (code: 1942246-ccb5e106)
- [Anthony's / Bowdoin IT Leaderboard](https://adventofcode.com/2023/leaderboard/private/view/2619876)

## Problems and Solutions

- Day 0: Template -- [solution](./day00)
- [Day 1: Historian Hysteria](https://adventofcode.com/2024/day/1) -- [solution](./day01)
- [Day 2: Red-Nosed Reports](https://adventofcode.com/2024/day/2) -- [solution](./day02)
- [Day 3: Mull It Over](https://adventofcode.com/2024/day3) -- [solution](./day03)
- [Day 4: Ceres Search](https://adventofcode.com/2024/day4) -- [solution](./day04)
- [Day 5: Print Queue](https://adventofcode.com/2024/day5) -- [solution](./day05)
- [Day 6: Guard Gallivant](https://adventofcode.com/2024/day6) -- [solution](./day06)
- [Day 7: Bridge Repair](https://adventofcode.com/2024/day7) -- [solution](./day07)
- [Day 8: Resonant Collinearity](https://adventofcode.com/2024/day8) -- [solution](./day08)
- [Day 9: Disk Fragmenter](https://adventofcode.com/2024/day9) -- [solution](./day09)
- [Day 10: Hoof It](https://adventofcode.com/2024/day10) -- [solution](./day10)
- [Day 11: ](https://adventofcode.com/2024/day11) -- [solution](./day11)

## NOTES

`day00` is a starter code template with a working Makefile. It provides setup with `getopt()`,
high resolution timing, and a wrapper for reading data. To start a new day from the template:

```
mkdir dayXX
cp day00/* dayXX
``` 


Options
    - default, runs part 1 and part 2 and does not check answers, just prints them
    - `-1 n` to enable part 1 test where `n` is the answer value to check
    - `-2 n` to enable part 2 test where `n` is the answer value to check

    - `n` = positive number will run part and check answer (exit code set appropriately)
    - `n` = `0` to disable check for part, e.g. `-1 0` will still run part 0 but not check it's answer
    - `n` = `-1` to disable run for part, e.g. `-1 -1` will disable part 1 completey


```
./dayXX input.txt           # runs part 1 and part 2 and prints answers (no check)
./dayXX -1 12 input.txt     # runs part 1 and part 2 prints answers and checks part 1 result
./dayXX -2 -1 input.txt     # disables part 2 from running
```

# Advent of Code 2024
## Stephen Houser

Contains my solutions to the [Advent of Code 2024](https://adventofcode.com/2024).

## Tools

- Python (no additional libraries)
- Visual Studio Code
- macOS / Linux
- git / GitHub

## Leaderboards


## Problems and Solutions


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

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

    List processing, Sorting. Sort two lists of numbers, find sum of differences, how many times does each number from first appear in second.

- [Day 2: Red-Nosed Reports](https://adventofcode.com/2024/day/2) -- [solution](./day02)

    List processing, check list for valid sequece (safe/unsafe); can tolerate single error (parity)?

- [Day 3: Mull It Over](https://adventofcode.com/2024/day3) -- [solution](./day03)

    List processing, Simulation, Regex. Simulated computer that mul(), do(), and don't()

- [Day 4: Ceres Search](https://adventofcode.com/2024/day4) -- [solution](./day04)

    Search. Search grid for XMAS; Search for diagoanl MAS.

- [Day 5: Print Queue](https://adventofcode.com/2024/day5) -- [solution](./day05)

    Topographical Sorting. Series of rules that numbers need to be arranged by find good and bad ones; reorder to be correct.

- [Day 6: Guard Gallivant](https://adventofcode.com/2024/day6) -- [solution](./day06)

    Pathfinding, Search. Guard moves around count spaces they walk on; find spaces to force guard into a loop.

- [Day 7: Bridge Repair](https://adventofcode.com/2024/day7) -- [solution](./day07)

    Math, List processing. Solve series of equations figure out where to * and where to +; adds concatenation operator.

- [Day 8: Resonant Collinearity](https://adventofcode.com/2024/day8) -- [solution](./day08)

    Geometry. Series of antennas, need to find resonant nodes; find multiple resonant nodes.

- [Day 9: Disk Fragmenter](https://adventofcode.com/2024/day9) -- [solution](./day09)

    Disk defragmentation by splitting files to earlier space; must move files as whole block, cannot split.

- [Day 10: Hoof It](https://adventofcode.com/2024/day10) -- [solution](./day10)

    Pathfinding. Find paths on map from 0 to 9 ascending numbers; find number of trails attached to each starting location.

- [Day 11: Plutonian Pebbles](https://adventofcode.com/2024/day11) -- [solution](./day11)

    Series of stones that change, multiply, and divide, how many do you have after 25 units; how many after 75?

- [Day 12: Garden Groups](https://adventofcode.com/2024/day12) -- [solution](./day12)

    Flood fill. Find regions (garden plots) on map, calculate perimiter * area; calculate number of unique walls.

- [Day 13: Claw Contraption](https://adventofcode.com/2024/day13) -- [solution](./day13)

    Math. Two buttons on a claw machine, find optimal press of each to get prize; add 10,000,000,000,000 to each prize location.

- [Day 14: Restroom Redoubt](https://adventofcode.com/2024/day14) -- [solution](./day14)

    Math, Simulation. Robots traveling on vectors, simulate for 100 seconds; simulate until you see a christmas tree.

- [Day 15: Warehouse Woes](https://adventofcode.com/2024/day15) -- [solution](./day15)

    Simulation. Robot moves around pushing boxes; boxes get 2x as big and you can push from one side.

- [Day 16: Reindeer Maze](https://adventofcode.com/2024/day16) -- [solution](./day16)

    Dijkstra's. find shortest path with penalty for turns; find all tiles on all shortest paths.

- [Day 17: Chronospatial Computer](https://adventofcode.com/2024/day17) -- [solution](./day17)

    Simulation, Pattern, Math. Simulate a simple computer determine output; find when output matches instruction set (repeating patterns). Refined second part with octal search.

- [Day 18: RAM Run](https://adventofcode.com/2024/day18) -- [solution](./day18)

    Dijkstra's and binary search; series of tiles added to map, after n steps is the map traversable (dijkstra); find the added tile that makes the map not traversable (binary search).

- [Day 19: ](https://adventofcode.com/2024/day19) -- [solution](./day19)


## NOTES

`day00` is a starter code template with a working Makefile. It provides setup with `getopt()`,
high resolution timing, and a wrapper for reading data. To start a new day from the template:

```
mkdir dayXX
cp day00/* dayXX
``` 


## Profiling

[`gprof`](https://www.math.utah.edu/docs/info/gprof_toc.html)
add `-pg` to compile



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

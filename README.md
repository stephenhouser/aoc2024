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

- [Day 19: Linen Layout](https://adventofcode.com/2024/day19) -- [solution](./day19)

    Regex, Combinations, Memoization; set of towels and possible arrangements, first part was are the arrangements valid or not. Used a slow regex which I knew would not work for a deeper problem. Second part was how many ways are each valid arrangement possible. Used memoization on a depth first search (DFS).

- [Day 20: Race Condition](https://adventofcode.com/2024/day20) -- [solution](./day20)

    Mapping, Dijkstra, and checking for shortcuts to the paths. First part allowed skipping one wall, second part let you jump anywhere in a 20 (manhatten) distance.

- [Day 21: Keypad Conundrum](https://adventofcode.com/2024/day21) -- [solution](./day21)

    Recursion, serious recursion. Took an extra day for this one; first was remote controlling a remote control that controled a robot. Second was 25 layers deep. Stuck finding the "best path" for quite a while. Had to evaluate all possible paths. Second part really was just adding cache/memoization to speed up.

- [Day 22: Monkey Market](https://adventofcode.com/2024/day22) -- [solution](./day22)

    Pattern matching; this was a reading comprehension test. first part was straightforward math and manipulations; second part was an exhaustive search of the patterns in previous results. Matching for best pattern/sale price of banannas.

- [Day 23: LAN Party](https://adventofcode.com/2024/day23) -- [solution](./day23)

    Graph, Cliques; first part was to find triples that contained a node starting with "t". Second part was to find the largest clique (an NP-Hard problem) Used Bron Kerbosch from Wikipedia.

- [Day 24: Crossed Wires](https://adventofcode.com/2024/day24) -- [solution](./day24)

    Logic simulation and graph viewing. Today was a tough one. First part reading graph, building simulated circuit and simulating it to get result. Second, adder circuit is broken need to find wires to swap. Considered just trying evey pair randomly and hoping. Reverted to setting one bit on y (in x+y=z) to triangulate where broken parts might be. Was fruitful, found which bit failed, looked at graph in that area and manually found wires to patch. Rinse, repeat. No code for the "solve".

- [Day 25: Code Chronicle](https://adventofcode.com/2024/day25) -- [solution](./day25)

    Brute force. Straightforward, read maps of keys and locks, check if they fit together.

## For Next Year

- Develop `charmap`, `point`, `vector`, and new `graph` classes to fit together.
- Add transpose to charmap (could have used on last day)
- Add ant/traveler to charmap and graph to 'walk the path'
- Flesh out `cache` or memoize classes
- Make charmap convert to graph and graph to charmap
- graph class with edges to nodes and nodes to edges to represent either way and flip between them.
- graph and charmap to .dot file for viewing.
- output functions, points, vectors, graphs to iostream
- add some graph algos to it (dijkstra, a*, bron Kerbosh and others)
- parsing routines; split input on `\n\n`, split ints, split tokens, common graph layouts (`a OR n -> x`, `a : b`, etc.)
- Get debugger working remotely in vscode
- switch to c++20 with ranges and functional programming

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

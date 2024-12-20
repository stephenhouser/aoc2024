from enum import Enum
from functools import cache, cached_property
from collections import defaultdict
from math import inf
from argparse import ArgumentParser, FileType


class Direction(Enum):
    '''A cardinal direction.'''
    UP = 0
    LEFT = 1
    DOWN = 2
    RIGHT = 3

    @property
    def horisontal(self):
        return self in (Direction.LEFT, Direction.RIGHT)

    @cached_property
    def delta(self):
        return ((0, -1), (1, 0), (0, 1), (-1, 0))[self.value]

    def move(self, x, y):
        '''Translate the coordinates a distance of 1 in this direction.'''
        dx, dy = self.delta
        return x + dx, y + dy


def parse(src):
    return [line for line in src.splitlines() if line != '']


def find_tile(grid, tile):
    for y, line in enumerate(grid):
        for x, char in enumerate(line):
            if char == tile:
                return x, y

    raise ValueError(f'Could not find "{tile}" in grid')


def ScoreMap(mapping):
    return defaultdict(lambda: inf, mapping)


def on_grid(x, y, grid):
    return 0 <= y < len(grid) and 0 <= x < len(grid[0])


def at(grid, x, y):
    return grid[y][x]


@cache
def taxicab_circle(x, y, r):
    for offset in range(r):
        inv_offset = r - offset
        yield x + offset, y + inv_offset
        yield x + inv_offset, y - offset
        yield x - offset, y - inv_offset
        yield x - inv_offset, y + offset


def open_space(grid):
    def check(coord):
        return on_grid(*coord, grid) and at(grid, *coord) != '#'
    return check


def get_cheats(x, y, grid, cheat_time):
    for radius in range(2, cheat_time + 1):
        valid_cheats = filter(open_space(grid), taxicab_circle(x, y, radius))
        yield from ((cheat, radius) for cheat in valid_cheats)


def make_graph(grid, cheat_time):
    valid_neighbour = open_space(grid)

    start = find_tile(grid, 'S')
    end = find_tile(grid, 'E')

    seen = {start, }
    edges = defaultdict(set)
    cheats = dict()  # a cheat is effectively a bonus edge

    todo = {start, }
    while len(todo) > 0:
        node = todo.pop()

        # find this node's neighbours
        for direction in Direction:
            neighbour = direction.move(*node)
            if valid_neighbour(neighbour):
                # valid neighbour
                edges[node].add(neighbour)

                if neighbour not in seen:
                    todo.add(neighbour)
                    seen.add(neighbour)

        # find the cheats for this node
        cheats[node] = set(get_cheats(*node, grid, cheat_time))

    return start, end, edges, cheats


def dijkstra(start, edges):
    discovered = {start, }
    distance = ScoreMap({start: 0})

    while len(discovered) > 0:
        current_node = min(discovered, key=lambda x: distance[x])
        discovered.remove(current_node)

        for neighbour in edges[current_node]:
            candidate_score = distance[current_node] + 1
            if candidate_score < distance[neighbour]:
                distance[neighbour] = candidate_score
                discovered.add(neighbour)

    return distance


def multi_items(mapping):
    for key, items in mapping.items():
        for item in items:
            yield key, *item


def main(grid, time_save, cheat_time, verbose):
    start, end, edges, cheats = make_graph(grid, cheat_time)
    distance_from_start = dijkstra(start, edges)
    distance_from_end = dijkstra(end, edges)
    target_distance = distance_from_start[end] - time_save

    cheat_count = defaultdict(int)
    total = 0
    for cheat_start, cheat_end, cheat_cost in multi_items(cheats):
        dist = distance_from_start[cheat_start] + cheat_cost \
             + distance_from_end[cheat_end]
        if dist <= target_distance:
            cheat_count[distance_from_start[end] - dist] += 1
            total += 1

    if verbose:
        for save, number in sorted(cheat_count.items(), key=lambda x: x[0]):
            if number == 1:
                print(f'There is one cheat that saves {save} picoseconds')
            else:
                print(
                    f'There are {number} cheats that save {save} picoseconds')

    return total


arg_parser = ArgumentParser()
arg_parser.add_argument('src', type=FileType('r'))
arg_parser.add_argument('time_save', nargs='?', type=int, default=100)
arg_parser.add_argument('cheat_time', nargs='?', type=int, default=20)
arg_parser.add_argument('-v', '--verbose', action='store_true')

if __name__ == '__main__':
    args = arg_parser.parse_args()
    grid = parse(args.src.read())
    print(main(grid, args.time_save, args.cheat_time, args.verbose))
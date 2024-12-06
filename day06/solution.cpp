#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <set>

#include "aoc2024.h"
#include "solution.h"


bool is_valid(const data_collection_t &map, long x, long y) {
	return 0 <= x && (size_t)x < map[0].size() && 0 <= y && (size_t)y < map.size();
}

char at(const data_collection_t &map, long x, long y) {
	return is_valid(map, x, y) ? map[(size_t)y][(size_t)x] : '\0';
}

char get(const data_collection_t &map, long unsigned int x, long unsigned int y) {
	return is_valid(map, (long)x, (long)y) ? map[(size_t)y][(size_t)x] : ' ';
}

char set(data_collection_t &map, long unsigned int x, long unsigned int y, char c) {
	if (is_valid(map, (long)x, (long)y)) {
		return map[(size_t)y][(size_t)x] = c;
	}

	return '\0';
}

std::tuple<long, long> find_start(const data_collection_t &map) {
	for (auto [y, row] : enumerate(map)) {
		for (auto [x, location] : enumerate(row)) {
			if (at(map, x, y) == '^') {
				return {x, y};
			}
		}
	}

	return {-1, -1};
}

void print_map(const data_collection_t &map) {

	for (auto row : map) {
		for (auto location : row) {
			std::cout << location;
		}
		std::cout << "\n";
	}
}

void turn_right(int &dx, int &dy) {
	if (dx == 0) {
		if (dy == -1) {	// 0,-1: U -> R
			dx = 1;
			dy = 0;
		} else {		// 0,1: D-> L
			dx = -1;
			dy = 0;

		}
	} else {			// dy == 0
		if (dx == -1) {	// -1,0: L -> U
			dx = 0;
			dy = -1;
		} else {		// 1,0: R -> D
			dx = 0;
			dy = 1;
		}
	}
}

std::string make_string(long x, long y, int dx, int dy) {
	char buffer[256];
	sprintf(buffer, "%ld,%ld,%d,%d", x, y, dx, dy);
	return std::string(buffer);
}

bool is_obstacle(const data_collection_t &map, long x, long y) {
	auto c = at(map, x, y);
	return c == '#' || c == '%';
}

// path, did_exit
std::tuple<std::vector<std::tuple<long, long>>, bool>
traverse(const data_collection_t &data1) {
	auto data = data1;
	std::vector<std::tuple<long, long>> path;
	std::set<std::string> visited; // "x,y,dx,dy" for loop checking

	auto [x, y] = find_start(data);
	int dx = 0, dy = -1;

	while (is_valid(data, x, y)) {
		auto node = make_string(x, y, dx, dy);
		if (visited.find(node) != visited.end()) {
			// if we visit the same location going the same direction
			// stop if we hit a loop
			break;
		}

		visited.insert(node);

		path.push_back({x, y});

		set(data, x, y, '+');
		while (is_obstacle(data, x+dx, y+dy)) {
			// keep turing until we find a path.
			// should always be able to go back the way we came.
			turn_right(dx, dy);
		}

		x += dx;
		y += dy;
	}

	return {path, !is_valid(data, x, y)};
}

long part1(const data_collection_t data) {
	auto [path, _] = traverse(data);
	std::set<std::tuple<long, long>> visited(path.begin(), path.end());
	return visited.size();
}

// 1922 too low
// 1934 too low
// 1940 to low (last)
// 1976 !!
// 2136 too high (106)
long part2(const data_collection_t data) {
	long solution = 0;

	std::set<std::string> visited; // we already tried here

	auto [path, _] = traverse(data);
	auto [start_x, start_y] = find_start(data);

	for (auto [x, y] : path) {
		if (x == start_x && y == start_y) {
			continue;
		}

		// only test locations we have not tested
		auto node = make_string(x, y, 0, 0);
		if (visited.find(node) == visited.end()) {
			visited.insert(node);

			data_collection_t test_map = data;
			set(test_map, x, y, '%');
			auto [_, escaped] = traverse(test_map);
			if (!escaped) {
				solution++;
			}
		}
	}

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	auto lines = read_lines(path);
    data_collection_t map;

    for (const auto& line : lines) {
		std::vector<char> map_row(line.begin(), line.end());
		map.push_back(map_row);
	}

    return map;
}


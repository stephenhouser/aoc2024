#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

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

long part1(const data_collection_t data) {
	long solution = 0;

	data_collection_t mapped = data;
	auto [x, y] = find_start(data);
	int dx = 0, dy = -1;

	while (is_valid(mapped, x, y)) {
		// std::cout << "(" << x << "," << y << ") (" << dx << "," << dy << ") " << solution << "\n";
		// print_map(mapped);

		// count current spot
		if (at(mapped, x, y) != 'X') {
			solution++;
		}

		set(mapped, x, y, 'X');

		// move to next location
		char next = at(mapped, x+dx, y+dy);
		if (next == '\0') {
			break;
		} else if (next == '#') {
			turn_right(dx, dy);
		}

		x += dx;
		y += dy;
		// std::cout << "\t(" << x << "," << y << ") (" << dx << "," << dy << ") " << std::endl;
		// std::cout << std::endl;
	}

	// TODO: part 1 code here

	return solution;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 2;

	// TODO: part 2 code here

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


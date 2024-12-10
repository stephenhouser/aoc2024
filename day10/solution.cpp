#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>

#include "aoc2024.h"
#include "solution.h"


/* Read data from path and return a vector for each line in the file. */
using charmap_t = std::vector<std::vector<char>>;

const charmap_t read_charmap(const std::string &path) {
	auto lines = read_lines(path);
    charmap_t charmap;

    for (const auto& line : lines) {
		std::vector<char> map_row(line.begin(), line.end());
		charmap.push_back(map_row);
	}

    return charmap;
}

bool is_valid(const charmap_t &charmap, long x, long y) {
	return 0 <= x && (size_t)x < charmap[0].size()
	 	&& 0 <= y && (size_t)y < charmap.size();
}

char get(const charmap_t &charmap, long x, long y) {
	return is_valid(charmap, x, y) ? charmap[(size_t)y][(size_t)x] : ' ';
}

char get(const charmap_t &charmap, size_t x, size_t y) {
	return get(charmap, (long)x, (long)y);
}

bool is_char(const charmap_t &charmap, long x, long y, char c) {
	return is_valid(charmap, x, y) && charmap[(size_t)y][(size_t)x] == c;
}

bool is_char(const charmap_t &charmap, size_t x, size_t y, char c) {
	return is_char(charmap, (long)x, (long)y, c);
}

std::tuple<size_t, size_t> size(const charmap_t &charmap) {
	size_t x = 0;
	size_t y = charmap.size();
	if (y != 0) {
		x = charmap[0].size();
	}

	return {x, y};
}

// cheesy "hash" of node location and direction
inline size_t cheese_hash(size_t x, size_t y) {
	size_t hash = (x  & 0xFFFF) << 16
				| (y  & 0xFFFF);

	return hash;
}

int trail_search(const charmap_t &map, size_t x, size_t y, std::unordered_set<size_t> &found) {
	int trails = 0;

	char here = get(map, x, y);
	if (here == '9') {
		found.insert(cheese_hash(x, y));
		return 1;
	}

	// check up
	if (0 < y && get(map, x, y-1) == here+1) {
		trails += trail_search(map, x, y-1, found);
	}

	// check down
	if (y < map.size()-1 && get(map, x, y+1) == here+1) {
		trails += trail_search(map, x, y+1, found);
	}

	// check left
	if (0 < x && get(map, x-1, y) == here+1) {
		trails += trail_search(map, x-1, y, found);
	}

	// check right
	if (x < map[0].size()-1 && get(map, x+1, y) == here+1) {
		trails += trail_search(map, x+1, y, found);
	}

	return trails;
}


long part1([[maybe_unused]]const data_collection_t map) {
	long solution = 0;

	auto [x_size, y_size] = size(map);
	for (size_t y = 0; y < y_size; y++) {
		for (size_t x = 0; x < x_size; x++) {
			if (is_char(map, x, y, '0')) {
				std::unordered_set<size_t> found;
				trail_search(map, x, y, found);
				solution += (long)found.size();
			}
		}
	}

	return solution;
}

long part2([[maybe_unused]] const data_collection_t map) {
	long solution = 0;

	auto [x_size, y_size] = size(map);
	for (size_t y = 0; y < y_size; y++) {
		for (size_t x = 0; x < x_size; x++) {
			if (is_char(map, x, y, '0')) {
				std::unordered_set<size_t> found;
				solution += trail_search(map, x, y, found);
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
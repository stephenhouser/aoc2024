#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

#include "aoc2024.h"
#include "solution.h"

using map_t = std::vector<std::string>;
using pin_t = std::vector<size_t>;
using lock_t = std::vector<pin_t>;

// returns number of hash marks in colums
pin_t count_hash(map_t &map) {
	std::vector<size_t> pins(map[0].size(), 0);

	for (auto &row : map) {
		for (size_t c = 0; c < row.size(); c++) {
			pins[c] += (row[c] == '#') ? 1ul : 0ul;
		}
	}

	for (auto &p : pins) {
		p--;
	}

	return pins;
}

std::tuple<lock_t, lock_t> parse(const data_collection_t &data) {
	lock_t locks;
	lock_t keys;

	map_t map;
	for (auto &line : data) {
		// std::cout << line << std::endl;
		// std::cout << map.size() << std::endl;

		if (line.empty()) {
			auto pins = count_hash(map);
			if (map[0][0] == '#') {
				// lock
				locks.push_back(pins);
			} else {
				// key
				keys.push_back(pins);
			}

			map.clear();
		} else {
			map.push_back(line);
		}
	}

	auto pins = count_hash(map);
	if (map[0][0] == '#') {
		// lock
		locks.push_back(pins);
	} else {
		// key
		keys.push_back(pins);
	}

	return {locks, keys};
}

bool key_fits(pin_t &key, pin_t &lock, size_t lock_size = 5) {
	for (size_t pin = 0; pin < key.size(); pin++) {
		if (key[pin] + lock[pin] > lock_size) {
			return false;
		}
	}

	return true;
}



long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 0;

	auto [locks, keys] = parse(data);

	for (auto p : locks) {
		std::cout << "lock: " << p << std::endl;
	}

	for (auto p : keys) {
		std::cout << " key: " << p << std::endl;
	}

	for (auto &lock : locks) {
		for (auto &key : keys) {
			solution += key_fits(key, lock, 5) ? 1 : 0;
		}
	}

	return solution;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 2;

	// TODO: part 2 code here

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return read_lines(path);
}


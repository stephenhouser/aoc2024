#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_map>
#include <cmath>

#include "aoc2024.h"
#include "solution.h"

using stone_map_t = std::unordered_map<size_t, size_t>;

int digits(size_t n) {
	// return (int)std::log10(n) + 1;

	int digits = 0;
	for (; n > 0; n /= 10) {
		++digits;
	}

	return digits;
}

std::tuple<size_t, size_t> split_number(size_t n) {
	size_t mult = 1;
	size_t first = n;
	int half = digits(n) / 2;

	while (half--) {
		first /= 10;
		mult *= 10;
	}

	size_t second = n - (first * mult);
	return {first, second};
}

stone_map_t blink(const stone_map_t &stones) {
	stone_map_t new_stones;

	for (auto const &[stone, count] : stones) {
		if (stone == 0) {
			new_stones[1] += count;
		} else if (digits(stone) % 2 == 0) {
			auto [first, second] = split_number(stone);
			new_stones[first] += count;
			new_stones[second] += count;
		} else {
			new_stones[stone * 2024] += count;
		}
	}

	return new_stones;
}

long count_stones(stone_map_t &stones) {
	long stone_count = 0;
	for (auto const &[stone, count] : stones) {
		stone_count += (long)count;
	}

	return stone_count;
}

stone_map_t map_stones(const data_collection_t &data) {
	stone_map_t stones;
	for (auto stone : data) {
		stones[stone] += 1;
	}

	return stones;
}

long part1(const data_collection_t data) {
	size_t blinks = 25;

	auto stones = map_stones(data);

	while (blinks--) {
		stones = blink(stones);
	}

	return count_stones(stones);

}

long part2([[maybe_unused]] const data_collection_t data) {
	size_t blinks = 75;

	auto stones = map_stones(data);

	while (blinks--) {
		stones = blink(stones);
	}

	return count_stones(stones);
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	data_collection_t numbers;
	for (auto line: read_lines(path)) {
		auto strs = split(line, " ");
		for (auto s : strs) {
			numbers.push_back((size_t)atol(s.c_str()));
		}
	}

	return numbers;
}


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


// recursive with memoization is slightly slower than iterative
bool recursive = false;

using stone_map_t = std::unordered_map<size_t, size_t>;

int digits(size_t n) {
	// std::log10() seems to be a lot slower...
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

/* *** Iterative Verion *** */

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

// double buffering version, trims about 4ms off of input.txt for 75 blinks
void fast_blink(stone_map_t &stones, stone_map_t &new_stones) {
	new_stones.clear();

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
}

/* *** Recursive Memoized Version *** */

// cheesy "hash" of node location and direction
inline size_t cheese_hash(size_t x, int y) {
	size_t hash = (x        ) << 8
				| (y  & 0xFF);

	return hash;
}

// stone << 8 | depth
std::unordered_map<size_t, long> stone_cache;

long cache_get(size_t stone, int depth) {
	size_t key = cheese_hash(stone, depth);
	if (stone_cache.find(key) != stone_cache.end()) {
		return stone_cache[key];
	}

	return -1;
}

void cache_add(size_t stone, int depth, long count) {
	size_t key = cheese_hash(stone, depth);
	stone_cache[key] = count;
}

// double buffering version, trims about 4ms off of input.txt for 75 blinks
long blink_stone(size_t stone, int depth) {
	long cached = cache_get(stone, depth);
	if (0 <= cached) {
		return cached;
	}

	if (depth == 0) {
		cached = 1;
	} else if (stone == 0) {
		cached = blink_stone(1, depth-1);
	} else if (digits(stone) % 2 == 0) {
		auto [first, second] = split_number(stone);
		cached = blink_stone(first, depth-1) + blink_stone(second, depth-1);
	} else {
		cached = blink_stone(stone*2024, depth-1);
	}

	cache_add(stone, depth, cached);
	return cached;
}

/* *** The Parts *** */

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
	long solution = 0;
	int blinks = 25;

	if (recursive) {
		for (auto stone : data) {
			solution += blink_stone(stone, blinks);
		}
	} else {
		auto stones = map_stones(data);

		while (blinks--) {
			stones = blink(stones);
		}

		solution = count_stones(stones);
	}

	return solution;
}

long part2(const data_collection_t data) {
	long solution = 0;
	int blinks = 75;

	if (recursive) {
		for (auto stone : data) {
			solution += blink_stone(stone, blinks);
		}
	} else {
		auto stones = map_stones(data);

		while (blinks--) {
			stones = blink(stones);
		}

		solution = count_stones(stones);
	}

	return solution;
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


#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

#include "aoc2024.h"
#include "solution.h"

int digits(size_t n) {
	//auto len = (int)std::log10(x) + 1;

	int digits = 0;
	for (; n > 0; n /= 10) {
		++digits;
	}

	return digits;
}

std::tuple<size_t, size_t> split_number(size_t n) {
	int half = digits(n) / 2;

	size_t mult = 1;
	size_t first = n;

	// std::cout << "split " << n << std::endl;
	while (half--) {
		first /= 10;
		mult *= 10;
	}

	size_t second = n - (first * mult);

	// std::cout << "split " << n << std::endl;
	return {first, second};
}

long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 1;

	size_t blinks = 25;

	data_collection_t stones = data;
	std::cout << stones << std::endl;
	while (blinks--) {
		data_collection_t new_stones;

		for (auto n : stones) {
			if (n == 0) {
				new_stones.push_back(1);
			} else if (digits(n) % 2 == 0) {
				auto [first, second] = split_number(n);
				new_stones.push_back(first);
				new_stones.push_back(second);
			} else {
				new_stones.push_back(n * 2024);
			}
		}

		stones = new_stones;
		// std::cout << stones << std::endl;
	}

	// std::cout << stones << std::endl;
	solution = (long)stones.size();
	return solution;
}

// stone_n, times
std::map<size_t, size_t> stone_map;

long part2([[maybe_unused]] const data_collection_t data) {
	size_t blinks = 75;

	std::map<size_t, size_t> stones;
	for (auto stone : data) {
		stones[stone] += 1;
	}

	// for (auto const &[stone, count] : stones) {
	// 	std::cout << "(" << stone << "," << count << ") ";
	// }
	// std::cout << std::endl;

	std::map<size_t, size_t> new_stones;
	while (blinks--) {		
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

		// for (auto const &[stone, count] : new_stones) {
		// 	std::cout << "(" << stone << "," << count << ") ";
		// }
		// std::cout << std::endl;
		stones = new_stones;
	}

	// std::cout << stones << std::endl;
	long solution = 0;
	for (auto const &[stone, count] : stones) {
		solution += (long)count;
	}
	return (long)solution;
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


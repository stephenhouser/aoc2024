#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>
#include <unordered_map>

#include "aoc2024.h"
#include "solution.h"

inline size_t mix(const size_t secret, const size_t next_secret) {
	return secret ^ next_secret;
}

inline size_t prune(const size_t secret) {
	return secret % 16777216;
}

size_t next_secret(size_t secret) {
	secret = prune(mix(secret, secret*64));
	secret = prune(mix(secret, secret/32));
	secret = prune(mix(secret, secret*2048));
	return secret;
}

long part1([[maybe_unused]]const data_collection_t data) {
	size_t solution = 0;
	size_t secret_iterations = 2000;

	// std::cout << data << std::endl;

	for (auto secret : data) {
		// std::cout << secret << ": ";
		for (size_t i = 0; i < secret_iterations; i++) {
			secret = next_secret(secret);
		}

		// std::cout << secret << std::endl;
		solution += secret;
	}

	// 37327623 for test
	// 12979353889 for input.txt
	return (long)solution;
}

// how many banannas would we get with pattern of changes
// Each buyer only wants to buy one hiding spot, so after the hiding spot is sold, the monkey will move on to the next buyer.
// so returns when found the first match of the pattern
size_t eval(const std::string &prices, std::string &changes, std::string &needle) {
	size_t sales = 0;
	size_t n_size = needle.size();
	size_t p_size = prices.size();

	size_t pos = changes.find(needle);
	while (pos != std::string::npos) {
		if (pos < p_size-n_size) {
			char price = prices[pos + n_size];	// price is the last digit in the sequence
			sales = static_cast<size_t>(price - '0');
			// don't accumulate, just return the sale price
			// std::cout << "\tfound pos=" << pos << " price= " << price << " : " << sales << std::endl;
			return sales;
		}

		pos = changes.find(needle, pos+1);
	}

	return sales;
}

// returns all the 4 char change patterns that end with price
std::unordered_set<std::string> find_patterns(const std::string &prices, std::string &changes, const int price) {
	size_t pattern_length = 4;

	// std::cout << "find patterns with " << price+'0' << std::endl;

	std::unordered_set<std::string> patterns;
	// patterns are 4 in length, so start at 4
	for (size_t i = pattern_length; i < prices.length(); i++) {
		if (prices[i] == (char)price+'0') {
			patterns.insert(changes.substr(i-4, 4));
		}
	}

	return patterns;
}

// 1442 too low
// 1449 just right
// 1450 too high
long part2(const data_collection_t data) {
	size_t secret_iterations = 2000;

	// std::cout << data << std::endl;
	// std::cout << std::endl;
	
		// banannas[pattern][buyer] = price
	std::unordered_map<std::string, std::unordered_map<size_t, int>> banannas;

	// for (auto secret : data) {
	for (size_t buyer = 0; buyer < data.size(); buyer++) {
		auto secret = data[buyer];

		std::string prices;
		std::string changes{' '};

		int last = static_cast<int>(secret % 10);
		prices += ((char)last + '0');
	
		for (size_t i = 0; i < secret_iterations; i++) {
			secret = next_secret(secret);

			int current = static_cast<int>(secret % 10);
			int change = current - last;

			prices += ((char)last + '0');
			changes += ((char)change + 'm');

			// if we are at least 4 chars in, start collecting patterns
			if (i >= 3) {
				std::string pattern{changes.substr(changes.size()-4, 4)};

				auto bit = banannas[pattern].find(buyer);
				if (bit == banannas[pattern].end()) {
					banannas[pattern][buyer] = current;
				}
			}

			last = current;
		}
	}

	// std::cout << "checking patterns..." << std::endl;

	size_t solution = 0;
	for (auto pattern : banannas) {
		auto buyers = pattern.second;
		size_t sold = 0;
		for (auto buyer : buyers) {
			sold += (size_t)buyer.second;
		}

		if (solution < sold) {
			// std::cout << "pattern=" << pattern.first << " sold=" << sold << std::endl;
			solution = sold;
		}
	}

	return (long)solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	data_collection_t data;
	for (auto &l : read_lines(path)) {
		data.push_back((size_t)atol(l.c_str()));
	}

	return data;
}


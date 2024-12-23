#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <cstdint>

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

size_t pat_hash(const std::string_view &pattern) {
	// assert(pattern.size() == 4);
	return (size_t)(pattern[0] & 0xFF) << 48
	     | (size_t)(pattern[1] & 0xFF) << 32
		 | (size_t)(pattern[2] & 0xFF) << 16
		 | (size_t)(pattern[3] & 0xFF);
}

// 1442 too low
// 1449 just right
// 1450 too high
long part2(const data_collection_t data) {
	size_t secret_iterations = 2000;

	// std::cout << data << std::endl;
	// std::cout << std::endl;

	// keep map of price by the pattern it was first associated with and the buyer	
	// banannas[pattern][buyer] = price
	// then we can look up later, each pattern and the total purchased

	// keep just a set of the buyers who have a price for the pattern
	// so we only count their first
	std::unordered_map<size_t, std::unordered_set<size_t>> banannas;
	// keep the total cost for this pattern
	std::unordered_map<size_t, int> pattern_banannas;

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
			// collect patterns and their first cost as we iterate through 
			// the secrets. Then we don't have to do it later
			if (i >= 3) {
				auto hash = pat_hash(changes.substr(changes.size()-4, 4));
				auto bit = banannas[hash].find(buyer);
				if (bit == banannas[hash].end()) {
					banannas[hash].insert(buyer);
					pattern_banannas[hash] += current;
				}
			}

			last = current;
		}
	}

	// std::cout << "checking patterns..." << std::endl;

	// size_t solution = 0;
	// for (auto pattern : banannas) {
	// 	auto buyers = pattern.second;
	// 	size_t sold = 0;
	// 	for (auto buyer : buyers) {
	// 		sold += (size_t)buyer.second;
	// 	}

	// 	if (solution < sold) {
	// 		// std::cout << "pattern=" << pattern.first << " sold=" << sold << std::endl;
	// 		solution = sold;
	// 	}
	// }

	size_t solution = 0;
	// find max
	for (auto pattern : pattern_banannas) {
		if (solution < (size_t)pattern.second) {
			solution = (size_t)pattern.second;
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


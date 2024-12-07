#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <functional>

#include "aoc2024.h"
#include "solution.h"

/* combinations
 * Returns all combinations of chars with replacement into length n strings. 
 * e.g n=3, chars="01", returns ("000", "001", "010", "011", "100", ...)
 */
std::vector<std::string> combinations(size_t n, const std::string chars) {
    std::vector<std::string> result;
	std::string current;

	// inner recursive function
	std::function<void()> _generate = [&]() {
		if (current.size() == n) {
			result.push_back(current);
			return;
		}

		for (auto c : chars) {
			current.push_back(c);
			_generate();
			current.pop_back();
		}	
	};

	_generate();
	return result;
}

/* concatenate
 * Returns `a+b` as if they were strings concatenated together.
 * e.g. a=42, b=69, returns 4269.
 */
long concatenate(long a, long b) {
	for (long z = b; z > 0; z /= 10) {
		a *= 10;
	}
	return a + b;
}


long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 0;

	for (auto [answer, operands] : data) {
		auto combos = combinations(operands.size()-1, "+*");
		// std::cout << answer << ": " << operands << " -- " << combos << std::endl;

		for (const auto &operators : combos) {
			// std::cout << "\t: " << operands << " -- " << c;
			long total = operands[0];
			for (size_t i = 1; i < operands.size(); i++) {
				if (operators[i-1] == '+') {
					total += operands[i];
				} else {
					total *= operands[i];
				}

				if (total > answer) {
					break;
				}
			}

			if (total == answer) {
				solution += answer;
				break;
			}
		}
	}

	return solution;
}


long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;
	std::map<std::string, long> result_cache;

	for (auto [answer, operands] : data) {
		auto combos = combinations(operands.size()-1, "|+*");
		// std::cout << answer << ": " << operands << " -- " << combos << std::endl;

		// check each combination of operators...
		for (const auto &combo : combos) {
			// std::cout << "\t: " << operands << " -- " << c << std::endl;

			long total = operands[0];
			for (size_t i = 1; i < operands.size(); i++) {
				if (combo[i-1] == '+') {
					total += operands[i];
				} else if (combo[i-1] == '|') {
					total = concatenate(total, operands[i]);
				} else {
					total *= operands[i];
				}

				// break early if we go over the answer amount
				// no need to do more on this path
				if (total > answer) {
					break;
				}
			}

			// break early if we found a solution
			// no need to test more combinations
			if (total == answer) {
				solution += answer;
				break;
			}
		}
	}

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	auto lines = read_lines(path);
	data_collection_t data;

	for (const auto &line : lines) {
		long answer = 0;
		std::vector<long> numbers;

		for (auto n : split(line, " ")) {
			if (!answer) {
				answer = strtol(n.c_str(), NULL, 10);
			} else {
				numbers.push_back(strtol(n.c_str(), NULL, 10));
			}
		}

		data.push_back({answer, numbers});
	}

	return data;
}


#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

#include "aoc2024.h"
#include "solution.h"


void _combinations(size_t n, std::string current, std::vector<std::string>& result) {
	// Base case: If the current string has reached the length n, add it to the result.
	if (current.size() == n) {
		result.push_back(current);
		return;
	}
	
	// Recurse with '*' added to the current string
	_combinations(n, current + "*", result);
	
	// Recurse with '+' added to the current string
	_combinations(n, current + "+", result);
}

std::vector<std::string> combinations(size_t n) {
    std::vector<std::string> result;
    _combinations(n, "", result);
    return result;
}


long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 0;

	for (auto [answer, operands] : data) {
		auto combos = combinations(operands.size()-1);
		// std::cout << answer << ": " << operands << " -- " << combos << std::endl;

		for (const auto &c : combos) {
			// std::cout << "\t: " << operands << " -- " << c;

			long total = operands[0];
			bool ok = true;
			for (size_t i = 1; i < operands.size(); i++) {
				if (c[i-1] == '+') {
					total += operands[i];
				} else {
					total *= operands[i];
				}

				if (total > answer) {
					ok = false;
					break;
				}
			}

			if (ok) {
			 	if (total == answer) {
					// std::cout << " OK\n";
					solution += answer;
					break;
				} else {
					// std::cout << " FAIL\n";
				}
			}
		}

	}

	return solution;
}

void _combinations2(size_t n, std::string current, std::vector<std::string>& result) {
	// Base case: If the current string has reached the length n, add it to the result.
	if (current.size() == n) {
		result.push_back(current);
		return;
	}
	
	_combinations2(n, current + "|", result);
	_combinations2(n, current + "*", result);
	_combinations2(n, current + "+", result);
}

std::vector<std::string> combinations2(size_t n) {
    std::vector<std::string> result;
    _combinations2(n, "", result);
    return result;
}

long new_operand(long a, long b) {
	char buffer[100];
	sprintf(buffer, "%ld%ld", a, b);
	return atol(buffer);
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;

	for (auto [answer, operands] : data) {
		auto combos = combinations2(operands.size()-1);
		// std::cout << answer << ": " << operands << " -- " << combos << std::endl;

		for (const auto &c : combos) {
			// std::cout << "\t: " << operands << " -- " << c << std::endl;

			long total = operands[0];
			bool ok = true;
			for (size_t i = 1; i < operands.size(); i++) {
				if (c[i-1] == '+') {
					total += operands[i];
				} else if (c[i-1] == '|') {
					total = new_operand(total, operands[i]);
				} else {
					total *= operands[i];
				}

				if (total > answer) {
					ok = false;
					break;
				}
			}

			if (ok) {
			 	if (total == answer) {
					// std::cout << " OK\n";
					solution += answer;
					break;
				} else {
					// std::cout << " FAIL\n";
				}
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
		std::vector<long> operands;
		std::vector<std::string> raw = split(line, " ");
		for (const auto &r : raw) {
			long n = atol(r.c_str());
			if (!answer) {
				answer = n;
			} else {
				operands.push_back(n);
			}
		}

		data.push_back({answer, operands});
	}

	return data;
}


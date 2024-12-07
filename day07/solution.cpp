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


std::string hash(const std::string operations, std::vector<long> operands) {
	std::string result(operations);

	for (auto n : operands) {
		result += std::to_string(n);
	}

	return result;
}

long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 0;

	// maps string, e.g **+12,5,6 to result 

	for (auto [answer, operands] : data) {
		auto combos = combinations(operands.size()-1, "+*");
		// std::cout << answer << ": " << operands << " -- " << combos << std::endl;

		for (const auto &c : combos) {
			// std::cout << "\t: " << operands << " -- " << c;
			bool ok = true;
			long total = operands[0];

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


long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;
	std::map<std::string, long> result_cache;

	for (auto [answer, operands] : data) {
		auto combos = combinations(operands.size()-1, "|+*");
		// std::cout << answer << ": " << operands << " -- " << combos << std::endl;

		for (const auto &c : combos) {
			// std::cout << "\t: " << operands << " -- " << c << std::endl;

			long total = operands[0];
			bool ok = true;

			// auto hvalue = hash(c, operands);
			// if (result_cache.find(hvalue) == result_cache.end()) {
				for (size_t i = 1; i < operands.size(); i++) {
					if (c[i-1] == '+') {
						total += operands[i];
					} else if (c[i-1] == '|') {
						total = concatenate(total, operands[i]);
					} else {
						total *= operands[i];
					}

					if (total > answer) {
						ok = false;
						break;
					}
				}
			// 	result_cache[hvalue] = total;
			// } else {
			// 	total = result_cache[hvalue];
			// }


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


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


/* */
long solver_p1(long answer, long current, std::vector<long> &nums, size_t idx) {
	if (idx == 0) {
		return solver_p1(answer, nums[idx], nums, 1);
	}

	if (idx == nums.size()) {
		return answer == current ? 1 : 0;
	}

	if (current > answer) {
		return 0;
	}

	return solver_p1(answer, current + nums[idx], nums, idx+1)
	     + solver_p1(answer, current * nums[idx], nums, idx+1);
}

long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 0;

	for (auto [answer, operands] : data) {
		if (solver_p1(answer, 0, operands, 0)) {
			solution += answer;
		}
	}

	return solution;
}

long solver_p2(const long answer, long current, std::vector<long> &nums, const size_t idx) {
	if (idx == 0) {
		return solver_p2(answer, nums[idx], nums, 1);
	} else if (idx == nums.size()) {
		return answer == current ? 1 : 0;
	} else if (current > answer) {
		return 0;
	}

	return solver_p2(answer, concatenate(current, nums[idx]), nums, idx+1)
		 + solver_p2(answer, current + nums[idx], nums, idx+1)
		 + solver_p2(answer, current * nums[idx], nums, idx+1);
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;

	for (auto [answer, operands] : data) {
		if (solver_p2(answer, operands[0], operands, 1)) {
			solution += answer;
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


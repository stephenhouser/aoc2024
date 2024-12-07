#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <functional>

#include "aoc2024.h"
#include "solution.h"

bool flag_backward = false;

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
int solver_p1_forward(long answer, long current, std::vector<long> &nums, size_t idx) {
	if (idx == 0) {
		return solver_p1_forward(answer, nums[idx], nums, 1);
	}

	if (idx == nums.size()) {
		return answer == current ? 1 : 0;
	}

	if (current > answer) {
		return 0;
	}

	return solver_p1_forward(answer, current + nums[idx], nums, idx+1)
	     + solver_p1_forward(answer, current * nums[idx], nums, idx+1);
}


/* faster solution to go backwards in the list... */
int solver_p1_backward(long answer, long current, std::vector<long> &nums, size_t idx) {
	if (idx == 0) {
		return current == nums[idx] ? 1 : 0;
	}

	if (current < nums[idx]) {
		return 0;
	}

	int solutions = solver_p1_backward(answer, current - nums[idx], nums, idx-1);
	if (current % nums[idx] == 0) {
		solutions += solver_p1_backward(answer, current / nums[idx], nums, idx-1);
	}

	return solutions;
}


int solver_p1(long answer, std::vector<long> &nums) {
	if (flag_backward || true) {
		return solver_p1_backward(answer, answer, nums, nums.size()-1);
	} else {
		return solver_p1_forward(answer, 0, nums, 0);
	}
}

long part1(const data_collection_t data) {
	long solution = 0;

	for (auto [answer, operands] : data) {
		if (solver_p1(answer, operands)) {
			solution += answer;
		}
	}

	return solution;
}

int solver_p2_forward(const long answer, long current, std::vector<long> &nums, const size_t idx) {
	if (idx == 0) {
		return solver_p2_forward(answer, nums[idx], nums, 1);
	} 
	
	if (idx == nums.size()) {
		return answer == current ? 1 : 0;
	}
	
	if (current > answer) {
		return 0;
	}

	return solver_p2_forward(answer, concatenate(current, nums[idx]), nums, idx+1)
		 + solver_p2_forward(answer, current + nums[idx], nums, idx+1)
		 + solver_p2_forward(answer, current * nums[idx], nums, idx+1);
}

int count_digits(long n) {
	int digits = 1;
	for (; n > 0; n /= 10) {
		digits *= 10;
	}

	return digits;
}

int solver_p2_backward(long answer, long current, std::vector<long> &nums, size_t idx) {
	if (idx == 0) {
		return current == nums[idx] ? 1 : 0;
	}

	if (current < nums[idx]) {
		return 0;
	}

	// deconstruct addition
	int solutions = solver_p2_backward(answer, current - nums[idx], nums, idx-1);

	// deconstruct multiplication, iff division results in whole number
	if (current % nums[idx] == 0) {
		solutions += solver_p2_backward(answer, current / nums[idx], nums, idx-1);
	}

	// deconstruct concatenation, iff we can trim off number from current
	int digits = count_digits(nums[idx]);
	if (current % digits == nums[idx]) {
		solutions += solver_p2_backward(answer, current / digits, nums, idx-1);
	}

	return solutions;
}

int solver_p2(long answer, std::vector<long> &nums) {
	if (flag_backward || true) {
		return solver_p2_backward(answer, answer, nums, nums.size()-1);
	} else {
		return solver_p2_forward(answer, nums[0], nums, 1);
	}
}


long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;

	for (auto [answer, operands] : data) {
		if (solver_p2(answer, operands)) {
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


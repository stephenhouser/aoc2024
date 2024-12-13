#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

#include "aoc2024.h"
#include "solution.h"


struct problem_t {
	long e1_x = 0;
	long e1_y = 0;
	long e2_x = 0;
	long e2_y = 0;
	long s_x = 0;
	long s_y = 0;
};

std::regex a_regex("Button A: X\\+(\\d+), Y\\+(\\d+)");
std::regex b_regex("Button B: X\\+(\\d+), Y\\+(\\d+)");
std::regex s_regex("Prize: X=(\\d+), Y=(\\d+)");

long solve(problem_t &problem) {
	int a_press = 3;
	int b_press = 1;

	for (int b = 0; b < 100; b++) {
		for (int a = 0; a < 100; a++) {
			auto sum_x = a * problem.e1_x + b * problem.e2_x;
			auto sum_y = a * problem.e1_y + b * problem.e2_y;

			// std::cout << "(" << a << "," << b << ")=> ";
			// //           << "(" << sum_x << "," << sum_y << ")\n";

			// std::cout << a << "*" << problem.e1_x << "+"
			//           << b << "*" << problem.e1_y << "=" << sum_x;

			// std::cout << a << "*" << problem.e2_x << "+"
			//           << b << "*" << problem.e2_y << "=" << sum_x;
			// std::cout << std::endl;

			// std::cout << "(" << a << "," << b << ")="
			//           << "(" << sum_x << "," << sum_y << ")\n";

			if (sum_x == problem.s_x && sum_y == problem.s_y) {
				return a_press * a + b_press * b;
			}

			if (sum_x >= problem.s_x || sum_y >= problem.s_y) {
				break;
			}
		}
	}

	return 0;
}


long part1(const data_collection_t data) {
	long solution = 0;

	std::vector<problem_t> problems;

	problem_t problem;
	std::cmatch match;
	for (auto line : data) {
		if (std::regex_match(line.c_str(), match, a_regex)) {
			problem.e1_x = atoi(match[1].str().c_str());
			problem.e1_y = atoi(match[2].str().c_str());
		}

		if (std::regex_match(line.c_str(), match, b_regex)) {
			problem.e2_x = atoi(match[1].str().c_str());
			problem.e2_y = atoi(match[2].str().c_str());
		}

		if (std::regex_match(line.c_str(), match, s_regex)) {
			problem.s_x = atoi(match[1].str().c_str());
			problem.s_y = atoi(match[2].str().c_str());
			problems.push_back(problem);
		}
	}

	for (auto problem : problems) {
		auto cost = solve(problem);

		solution += cost;

		std::cout << "(" << problem.e1_x << "," << problem.e1_y << ") "
				  << "(" << problem.e2_x << "," << problem.e2_y << ") "
				  << "(" << problem.s_x << "," << problem.s_y << ") "
				  << cost
				  << std::endl;
	}
	// TODO: part 1 code here

	return solution;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 2;

	// TODO: part 2 code here

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return read_lines(path);
}


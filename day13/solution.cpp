#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

#include "aoc2024.h"
#include "solution.h"

using value_t = long long;

struct problem_t {
	value_t ax = 0;
	value_t ay = 0;
	value_t bx = 0;
	value_t by = 0;
	value_t x = 0;
	value_t y = 0;
};

std::regex a_regex("Button A: X\\+(\\d+), Y\\+(\\d+)");
std::regex b_regex("Button B: X\\+(\\d+), Y\\+(\\d+)");
std::regex s_regex("Prize: X=(\\d+), Y=(\\d+)");

value_t solve_linear(problem_t &problem) {
	value_t a_cost = 3;	// cost to press a
	value_t b_cost = 1;	// cost to press b

	// https://byjus.com/jee/system-of-linear-equations-using-determinants/
	value_t d1 = problem.by * problem.x - problem.bx * problem.y;
	value_t d2 = -problem.ay * problem.x + problem.ax * problem.y;
	value_t d  = (problem.ax * problem.by) - (problem.ay * problem.bx);

	if (d == 0 || d1 % d != 0 || d2 % d != 0) {
		return 0;	// inconsistent system
	}

	return a_cost * (d1/d) + b_cost * (d2/d);
}


/* solve as system of linear equations */
value_t solve_linear_first(problem_t &problem) {
	value_t a_cost = 3;	// cost to press a
	value_t b_cost = 1;	// cost to press b

	// m*ax + n*bx = x
	// m*ay + n*by = y
	// m = (sx * by - sy * bx) / (ax * by - ay * bx)	
	// will have two possible solutions for n, choose n1 over n2
	// n = (sx - m * ax) / bx
	// n = (sy - m * ay) / by

	value_t ax = problem.ax;
	value_t ay = problem.ay;
	value_t bx = problem.bx;
	value_t by = problem.by;

	value_t m = ((problem.x  * by) - (problem.y  * bx)) 
	    	  / ((ax * by) - (ay * bx));

	// if m is 0; swap the points
	if (m == 0) {
		ax = problem.bx;
		ay = problem.by;
		bx = problem.ax;
		by = problem.ay;
		a_cost = 1;
		b_cost = 3;
		m = ((problem.x  * by) - (problem.y  * bx)) 
	      / ((ax * by) - (ay * bx));
	}

	// seems answer is always first solution
	value_t n1 = (problem.x - m * ax) / bx;
	// value_t n2 = (problem.y - m * ay) / by;

	// std::cout << "(" << problem.ax << "," << problem.ay << ") "
	// 			<< "(" << problem.bx << "," << problem.by << ") "
	// 			<< "(" << problem.x << "," << problem.y << ") ";

	// std::cout << " =" << m << "," << n1 << " or " << n2;
	// std::cout << std::endl;

	if (   m * ax + n1 * bx == problem.x 
		&& m * ay + n1 * by == problem.y) {
		// std::cout << "linear_a = " << a_press * m + b_press * n1 << "\n";

		return a_cost * m + b_cost * n1;	
	}

	// if (   m * ax + n2 * bx == problem.x 
	// 	&& m * ay + n2 * by == problem.y) {
	// 	std::cout << "linear_b = " << a_cost * m + b_cost * n2 << "\n";
	// 	return a_cost * m + b_cost * n2;
	// }

	// std::cout << "linear = 0\n";
	return 0;
}

/* solve using brute force -- expensive! */
value_t solve_brute_force(problem_t &problem) {
	value_t a_press = 3;
	value_t b_press = 1;

	for (value_t b = 0; b < 100; b++) {
		for (value_t a = 0; a < 100; a++) {
			auto sum_x = a * problem.ax + b * problem.bx;
			auto sum_y = a * problem.ay + b * problem.by;

			if (sum_x == problem.x && sum_y == problem.y) {

				// std::cout << "(" << problem.ax << "," << problem.ay << ") "
				// 			<< "(" << problem.bx << "," << problem.by << ") "
				// 			<< "(" << problem.x << "," << problem.y << ") ";

				// std::cout << " =" << a << "," << b;
				// std::cout << std::endl;
				// std::cout << "brute force = " << a_press * a + b_press * b << "\n";

				return a_press * a + b_press * b;
			}

			if (sum_x >= problem.x || sum_y >= problem.y) {
				break;
			}
		}
	}

	// std::cout << "brute force = 0\n";
	return 0;
}


long part1(const data_collection_t data) {
	value_t solution = 0;

	std::vector<problem_t> problems;

	problem_t problem;
	std::cmatch match;
	for (auto line : data) {
		if (std::regex_match(line.c_str(), match, a_regex)) {
			problem.ax = (value_t)atoi(match[1].str().c_str());
			problem.ay = (value_t)atoi(match[2].str().c_str());
		}

		if (std::regex_match(line.c_str(), match, b_regex)) {
			problem.bx = (value_t)atoi(match[1].str().c_str());
			problem.by = (value_t)atoi(match[2].str().c_str());
		}

		if (std::regex_match(line.c_str(), match, s_regex)) {
			problem.x = (value_t)atoi(match[1].str().c_str());
			problem.y = (value_t)atoi(match[2].str().c_str());
			problems.push_back(problem);
		}
	}

	for (auto problem : problems) {
		// value_t brute = solve_brute_force(problem);
		value_t linear = solve_linear(problem);

		solution += linear;

		// if (verbose > 1 || brute != linear) {
		// 	std::cout << "(" << problem.ax << "," << problem.ay << ") "
		// 			<< "(" << problem.bx << "," << problem.by << ") "
		// 			<< "(" << problem.x << "," << problem.y << ") "
		// 			<< brute << "," << linear
		// 			<< std::endl;
		// }

		// std::cout << std::endl;
	}

	return (long)solution;
}

/*
Button A: X+25, Y+11
Button B: X+42, Y+68
Prize: X=7881, Y=10751

Button A: X+45, Y+76
Button B: X+84, Y+14
Prize: X=9612, Y=4342
*/
// 297110000000000000 too high
// 875318608908 too low used wrong input
// 94955433618919

long part2(const data_collection_t data) {
	value_t solution = 0;
	std::vector<problem_t> problems;

	problem_t problem;
	std::cmatch match;
	for (auto line : data) {
		if (std::regex_match(line.c_str(), match, a_regex)) {
			problem.ax = (value_t)atoi(match[1].str().c_str());
			problem.ay = (value_t)atoi(match[2].str().c_str());
		}

		if (std::regex_match(line.c_str(), match, b_regex)) {
			problem.bx = (value_t)atoi(match[1].str().c_str());
			problem.by = (value_t)atoi(match[2].str().c_str());
		}

		if (std::regex_match(line.c_str(), match, s_regex)) {
			problem.x = (value_t)atoi(match[1].str().c_str()) + 10000000000000;
			problem.y = (value_t)atoi(match[2].str().c_str()) + 10000000000000;
			problems.push_back(problem);
		}
	}

	for (auto problem : problems) {
		// auto cost = solve_brute_force(problem);
		auto cost = solve_linear(problem);
		solution += cost;

		if (verbose > 1) {
			std::cout << "(" << problem.ax << "," << problem.ay << ") "
					<< "(" << problem.bx << "," << problem.by << ") "
					<< "(" << problem.x << "," << problem.y << ") "
					<< cost
					<< std::endl;
		}
	}

	return (long)solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return read_lines(path);
}


#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

#include "aoc2024.h"
#include "solution.h"

using value_t = unsigned long long;

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

value_t solve_linearX(problem_t &problem) {
	value_t a_press = 3;
	value_t b_press = 1;

	// m*ax + n*bx = x
	// m*ay + n*by = y
	// m = (sx * by - sy * bx) / (ax * by - ay * bx)	
	// n = (sx - m * ax) / bx or n = (sy - m * ay) / by
	double dm = ((problem.x  * problem.by) - (problem.y  * problem.bx)) 
	         / ((problem.ax * problem.by) - (problem.ay * problem.bx));

	double dn1 = (problem.x - dm * problem.ax) / problem.bx;
	double dn2 = (problem.y - dm * problem.ay) / problem.by;

	value_t m = (value_t)dm;
	value_t n1 = (value_t)dn1;
	value_t n2 = (value_t)dn2;

	std::cout << "(" << problem.ax << "," << problem.ay << ") "
				<< "(" << problem.bx << "," << problem.by << ") "
				<< "(" << problem.x << "," << problem.y << ") ";

	std::cout << " =" << m << "," << n1 << " or " << n2;
	std::cout << std::endl;

	if (   m * problem.ax + n1 * problem.bx == problem.x 
		&& m * problem.ay + n1 * problem.by == problem.y) {
		std::cout << "linear_a = " << a_press * m + b_press * n1 << "\n";

		return a_press * m + b_press * n1;	
	}

	if (   m * problem.ax + n2 * problem.bx == problem.x 
		&& m * problem.ay + n2 * problem.by == problem.y) {
		std::cout << "linear_b = " << a_press * m + b_press * n2 << "\n";
		return a_press * m + b_press * n2;
	}

	std::cout << "linear = 0\n";
	return 0;
}


value_t solve_linear(problem_t &problem) {
	value_t a_press = 3;
	value_t b_press = 1;

	// m*ax + n*bx = x
	// m*ay + n*by = y
	// m = (sx * by - sy * bx) / (ax * by - ay * bx)	
	// n = (sx - m * ax) / bx or n = (sy - m * ay) / by

	// (17,86) (84,37) (7870,6450)
	value_t ax = problem.ax;
	value_t ay = problem.ay;
	value_t bx = problem.bx;
	value_t by = problem.by;

	value_t m = ((problem.x  * by) - (problem.y  * bx)) 
	    	  / ((ax * by) - (ay * bx));

	if (m == 0) {
		ax = problem.bx;
		ay = problem.by;
		bx = problem.ax;
		by = problem.ay;
		a_press = 1;
		b_press = 3;
		m = ((problem.x  * by) - (problem.y  * bx)) 
	    	  / ((ax * by) - (ay * bx));
	}

	value_t n1 = (problem.x - m * ax) / bx;
	value_t n2 = (problem.y - m * ay) / by;


	// std::cout << "(" << problem.ax << "," << problem.ay << ") "
	// 			<< "(" << problem.bx << "," << problem.by << ") "
	// 			<< "(" << problem.x << "," << problem.y << ") ";

	// std::cout << " =" << m << "," << n1 << " or " << n2;
	// std::cout << std::endl;

	if (   m * ax + n1 * bx == problem.x 
		&& m * ay + n1 * by == problem.y) {
		// std::cout << "linear_a = " << a_press * m + b_press * n1 << "\n";

		return a_press * m + b_press * n1;	
	}

	if (   m * ax + n2 * bx == problem.x 
		&& m * ay + n2 * by == problem.y) {
		// std::cout << "linear_b = " << a_press * m + b_press * n2 << "\n";
		return a_press * m + b_press * n2;
	}

	// std::cout << "linear = 0\n";
	return 0;
}



/* Brute force */
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
		value_t brute = solve_brute_force(problem);
		value_t linear = solve_linear(problem);

		solution += brute;

		if (verbose > 1 || brute != linear) {
			std::cout << "(" << problem.ax << "," << problem.ay << ") "
					<< "(" << problem.bx << "," << problem.by << ") "
					<< "(" << problem.x << "," << problem.y << ") "
					<< brute << "," << linear
					<< std::endl;
		}

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

long part2(const data_collection_t data) {
	value_t solution = 0;
	std::vector<problem_t> problems;

	// problem_t p;
	// p.ax = 45;
	// p.ay = 76;
	// p.bx = 84;
	// p.by = 14;
	// p.x = 9612;
	// p.y = 4342;
	// solve_brute_force(p);
	// solve_linear(p);


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


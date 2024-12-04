#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

#include "aoc2024.h"
#include "solution.h"

bool is_letter(const data_collection_t &data, int x, int y, char c) {
	return 0 <= x && x < (int)data[0].size() && 
	       0 <= y && y < (int)data.size() &&
		   data[(std::size_t)y][(std::size_t)x] == c;
}

bool is_xmas(const data_collection_t &data, int x, int y, int dx, int dy) {
	return  is_letter(data, x+(  dx), y+(  dy), 'M') &&
	    	is_letter(data, x+(2*dx), y+(2*dy), 'A') &&
			is_letter(data, x+(3*dx), y+(3*dy), 'S');
}

int xmas_count(const data_collection_t &data, int x, int y) {
	int xmas_count = 0;

	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			if (!(dx == 0 && dy == 0)) {
				xmas_count += is_xmas(data, x, y, dx, dy) ? 1 : 0;
			}
		}
	}

	return xmas_count;
}

long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 0;

	int x_max = (int)data[0].size();
	int y_max = (int)data.size();

	for (int y = 0; y < y_max; y++) {
		for (int x = 0; x < x_max; x++) {
			if (is_letter(data, x, y, 'X')) {
				solution += xmas_count(data, x, y);
			}
		}
	}

	return solution;
}

bool is_x_mas(const data_collection_t &data, int x, int y) {
	return ((is_letter(data, x-1, y-1, 'M') && is_letter(data, x+1, y+1, 'S')) ||
			(is_letter(data, x-1, y-1, 'S') && is_letter(data, x+1, y+1, 'M'))) &&
		   ((is_letter(data, x+1, y-1, 'M') && is_letter(data, x-1, y+1, 'S')) ||
			(is_letter(data, x+1, y-1, 'S') && is_letter(data, x-1, y+1, 'M')));
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;

	int x_max = (int)data[0].size();
	int y_max = (int)data.size();

	// don't need to check first and last rows and columns
	// start at 1 end at size() - 1
	for (int y = 1; y < y_max - 1; y++) {
		for (int x = 1; x < x_max - 1; x++) {
			if (is_letter(data, x, y, 'A')) {
				solution += is_x_mas(data, x, y) ? 1 : 0;
			}
		}
	}

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	data_collection_t data;

	auto lines = read_lines(path);
	for (auto line : lines) {
		std::vector<char> line_v;
		for (auto c : line) {
			line_v.push_back(c);
		}

		data.push_back(line_v);
	}

	return data;
}


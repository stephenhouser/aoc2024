#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_map>
#include <array>

#include "aoc2024.h"
#include "solution.h"


using index_t = int;
using value_t = size_t;

struct point_t {
	index_t x = 0;
	index_t y = 0;
	value_t value = 0;

	point_t(index_t x, index_t y, value_t v = 0) {
		this->x = x;
		this->y = y;
		this->value = v;
	}

	/* closest to 0 with preference towards closer x */
	bool operator<(const point_t &other) const {		
		if (this->y < other.y) {
			return true;
		}

		return this->x < other.x;
	}

	bool operator==(const point_t &other) const {
		return this->x == other.x && this->y == other.y;
	}
};

std::ostream& operator<<(std::ostream& os, const point_t &p) {
	std::ostringstream buffer;
	os << "(" << p.x << "," << p.y << ")";
	return os;
}


/* hash function so can be put in unordered_map or set */
template <>
struct std::hash<point_t> {
	size_t operator()(const point_t &p) const {
		return std::hash<size_t>()((((size_t)p.x & 0xFFFFFFFF) << 32) | ((size_t)p.y & 0xFFFFFFFF));
	}
};

struct vector_t {
	point_t p = {0, 0};
	point_t v = {0, 0};
};

		
std::ostream& operator<<(std::ostream& os, const vector_t &vec) {
	std::ostringstream buffer;
	os << vec.p << "," << vec.v;
	return os;
}

bool show_guards(std::vector<vector_t> &guards, index_t x_size, index_t y_size) {
	std::unordered_map<point_t, value_t> map;
	for (const auto &guard : guards) {
		point_t p(guard.p.x, guard.p.y);
		map[p] += 1;
	}

	bool show_tree = false;
	for (int y = 0; y < y_size; y++) {
		int max_run = 0;
		int last_x = -2;

		for (int x = 0; x < x_size; x++) {
			point_t p(x, y);
			if (map[p] > 0) {
				if (last_x+1 == x) {
					max_run++;
					if (max_run > 5) {
						show_tree = true;
						break;
					}
				} else {
					max_run = 0;
				}
			last_x = x;
			}
		}
	}
			
	if (show_tree) {
		for (int y = 0; y < y_size/2; y++) {
			for (int x = 0; x < x_size; x++) {
				point_t p(x, y);
				if (map[p] > 0) {
					std::cout << '*';
				} else {
					std::cout << ' ';
				}
			}
			std::cout << std::endl;
		}
	return true;
	}

	return false;
}


// p=0,4 v=3,-3
std::regex vector_regex("p=(\\d+),(\\d+).+v=(-?\\d+),(-?\\d+)");
std::vector<vector_t> load_guards(const data_collection_t data) {

	std::vector<vector_t> guards;

	std::cmatch match;
	for (auto line : data) {
		vector_t guard;
		// std::cout << line << std::endl;
		if (std::regex_match(line.c_str(), match, vector_regex)) {
			guard.p.x = (value_t)atoi(match[1].str().c_str());
			guard.p.y = (value_t)atoi(match[2].str().c_str());
			guard.v.x = (value_t)atoi(match[3].str().c_str());
			guard.v.y = (value_t)atoi(match[4].str().c_str());
			guards.push_back(guard);
		}
	}

	return guards;
}


// 220942007 too high
// 216027840

long part1(const data_collection_t data) {

	// index_t max_x = 10;
	// index_t max_y = 6;
	index_t max_x = 100;
	index_t max_y = 102;

	std::vector<vector_t> guards = load_guards(data);
	// for (auto guard : guards) {
	// 	std::cout << guard << " ";
	// }
	// std::cout << std::endl;
	
	for (int i = 0; i < 100; i++) {
		for (auto &guard : guards) {
			guard.p.x = guard.p.x + guard.v.x;
			if (guard.p.x < 0) {
				guard.p.x += max_x +1;
			} else if (guard.p.x > max_x) {
				guard.p.x -= max_x +1;
			}
			guard.p.y = guard.p.y + guard.v.y;
			if (guard.p.y < 0) {
				guard.p.y += max_y +1;
			} else if (guard.p.y > max_y) {
				guard.p.y -= max_y +1;
			}
		}
	}


	index_t mid_x = max_x / 2;
	index_t mid_y = max_y / 2;
	std::vector<value_t> quads = {0, 0, 0, 0};

	for (auto guard : guards) {
		if (guard.p.x < mid_x) {
			if (guard.p.y < mid_y) {
				quads[0] += 1;
			} else if (guard.p.y > mid_y) {
				quads[1] += 1;
			}
		} else if (guard.p.x > mid_x) {
			if (guard.p.y < mid_y) {
				quads[2] += 1;
			} else if (guard.p.y > mid_y) {
				quads[3] += 1;
			}
		}
	}

	long solution = 1;
	for (auto &q : quads) {
		// std::cout << q << ",";
		solution *= (long)q;
	}
	// std::cout << std::endl;

	return solution;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;

	index_t max_x = 100;
	index_t max_y = 102;

	std::vector<vector_t> guards = load_guards(data);
	// for (auto guard : guards) {
	// 	std::cout << guard << " ";
	// }
	// std::cout << std::endl;
	
	for (int i = 0; i < 100000; i++) {
		for (auto &guard : guards) {
			guard.p.x = guard.p.x + guard.v.x;
			if (guard.p.x < 0) {
				guard.p.x += max_x +1;
			} else if (guard.p.x > max_x) {
				guard.p.x -= max_x +1;
			}
			guard.p.y = guard.p.y + guard.v.y;
			if (guard.p.y < 0) {
				guard.p.y += max_y +1;
			} else if (guard.p.y > max_y) {
				guard.p.y -= max_y +1;
			}
		}

		if (show_guards(guards, max_x, max_y)) {
			std::cout << std::endl;
			std::cout << i << "-----------------------------------------------"
					<< i << "-----------------------------------------------";
			std::cout << std::endl;
		}
	}


	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return read_lines(path);
}


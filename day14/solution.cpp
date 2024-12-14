#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_map>
#include <set>
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

void show_guards(const std::vector<vector_t> &guards, const point_t &world) {
	std::unordered_map<point_t, value_t> map;
	for (const auto &guard : guards) {
		point_t p(guard.p.x, guard.p.y);
		map[p] += 1;
	}

	for (int y = 0; y < world.y; y++) {
		for (int x = 0; x < world.x; x++) {
			point_t p(x, y);
			if (map[p] > 0) {
				std::cout << '*';
			} else {
				std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}
}

/* Load guard positions and velocities from input file.
 * stored as a vector or vector_t
 */
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


/* Modulus that works in positive and negative direction. 
 * Works to "wrap" a world in a x and y directions.
 */
index_t mod_wrap(index_t value, int maxval) {
	long long s_value = static_cast<long long>(value);
	if (s_value > 0) {
		return (index_t)s_value % maxval;
	}
	
	return (maxval - (static_cast<index_t>(-s_value) % maxval)) % maxval;
}

/* Simulate guard movement, fast method.
 * Adjusts the guard positions based on their velocity over the given time.
 * Uses the above mod_wrap() to compute the new location at the future time
 * in one calculation rather than having to step through it.
 */
void simulate_fast(std::vector<vector_t> &guards, const point_t &world, index_t time) {
	for (auto &guard : guards) {
		guard.p.x = mod_wrap(guard.p.x + (time * guard.v.x), world.x);
		guard.p.y = mod_wrap(guard.p.y + (time * guard.v.y), world.y);
	}
}

/* Simulate guard movement, slow method.
 * Adjusts the guard positions based on their velocity over the given time.
 * Computes for each unit of time, that's why it's the slow version.
 */
void simulate(std::vector<vector_t> &guards, const point_t &world, index_t time) {
	while (time--) {
		for (auto &guard : guards) {
			guard.p.x = (guard.p.x + guard.v.x) % world.x;
			if (guard.p.x < 0) {
				guard.p.x += world.x;
			}

			guard.p.y = (guard.p.y + guard.v.y) % world.y;
			if (guard.p.y < 0) {
				guard.p.y += world.y;
			}
		}
	}
}

// 220942007 too high
// 216027840
long part1(const data_collection_t data) {
	point_t world(101, 103);
	index_t simulation_time = 100;

	std::vector<vector_t> guards = load_guards(data);

	if (verbose > 1) {
		for (auto guard : guards) {
			std::cout << guard << " ";
		}
		std::cout << std::endl;
	}
	
	// simulate guard movement... for 100 seconds
	simulate_fast(guards, world, simulation_time);

	// count the guards in each quadrant
	point_t world_middle(world.x/2, world.y/2);
	std::vector<value_t> quadrants = {0, 0, 0, 0};
	
	for (auto guard : guards) {
		if (guard.p.x < world_middle.x) {
			if (guard.p.y < world_middle.y) {
				quadrants[0] += 1;
			} else if (guard.p.y > world_middle.y) {
				quadrants[1] += 1;
			}
		} else if (guard.p.x > world_middle.x) {
			if (guard.p.y < world_middle.y) {
				quadrants[2] += 1;
			} else if (guard.p.y > world_middle.y) {
				quadrants[3] += 1;
			}
		}
	}

	long solution = std::accumulate(quadrants.begin(), quadrants.end(), 1, std::multiplies<int>());
	// long solution = 1;
	// for (auto &quadrant : quadrants) {
	// 	solution *= (long)quadrant;
	// }

	return solution;
}

/* Slow version. Faster version below.
 * Return true if we think this arrangement is a christmas tree.
 * This is guessed by having a single row with guard_check (30) adjacent
 * guards all on the same row. This works out to be the border of the tree.
 */
bool is_tree_slow(const std::vector<vector_t> &guards, [[maybe_unused]] const point_t &world) {
	std::unordered_map<point_t, value_t> map;
	for (const auto &guard : guards) {
		point_t p(guard.p.x, guard.p.y);
		map[p] += 1;
	}

	for (int y = 0; y < world.y; y++) {
		int max_run = 0;
		int last_x = -2;

		for (int x = 0; x < world.x; x++) {
			point_t p(x, y);
			if (map[p] > 0) {
				if (last_x+1 == x) {
					max_run++;
					if (max_run > 5) {
						return true;
					}
				} else {
					max_run = 0;
				}
			last_x = x;
			}
		}
	}

	return false;
}

/* Return the size of the largest run of adjacent guards on the row y. */
value_t adjacent_guards(index_t y, const std::vector<vector_t> &guards) {
	std::set<index_t> guard_positions;
	for (const vector_t &guard : guards) {
		if (guard.p.y == y) {
			guard_positions.insert(guard.p.x);
		}
	}

	value_t max_adjacent = 0;
	value_t adjacent = 0;
	index_t last = 0;
	for (auto x : guard_positions) {
		if (x == last+1) {
			if (++adjacent > max_adjacent) {
				max_adjacent = adjacent;
			}
		} else {
			adjacent = 0;
		}
		last = x;
	}

	return max_adjacent;
}

/* Return true if we think this arrangement is a christmas tree.
 * This is guessed by having a single row with guard_check (30) adjacent
 * guards all on the same row. This works out to be the border of the tree.
 */
bool is_tree(const std::vector<vector_t> &guards) {
	value_t guard_check = 30;

	// do a quick check first to see if there are more than guard_check
	// guards in any row. If there are, then do a more detailed check
	// for a set of adjacent guards.
	std::unordered_map<index_t, value_t> row_count;
	for (const auto &guard : guards) {
		row_count[guard.p.y] += 1;
		if (row_count[guard.p.y] >= guard_check) {
			return adjacent_guards(guard.p.y, guards) >= guard_check;
		}
	}

	return false;
}

// 6875 off by one
// 6876
long part2([[maybe_unused]] const data_collection_t data) {
	long simulated_time = 0;
	point_t world(101, 103);

	std::vector<vector_t> guards = load_guards(data);
	
	while (!is_tree(guards)) {
		simulated_time++;
		simulate_fast(guards, world, 1);
	}

	if (verbose > 1) {
		show_guards(guards, world);
	}

	return simulated_time;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return read_lines(path);
}


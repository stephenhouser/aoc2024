#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <set>

#include <thread>
#include <mutex>

#include "aoc2024.h"
#include "solution.h"

bool thread_ripper = true;

/* Read data from path and return a vector for each line in the file. */
using charmap_t = std::vector<std::vector<char>>;

const charmap_t read_charmap(const std::string &path) {
	auto lines = read_lines(path);
    charmap_t charmap;

    for (const auto& line : lines) {
		std::vector<char> map_row(line.begin(), line.end());
		charmap.push_back(map_row);
	}

    return charmap;
}

bool is_valid(const charmap_t &charmap, long x, long y) {
	return 0 <= x && (size_t)x < charmap[0].size() 
	 	&& 0 <= y && (size_t)y < charmap.size();
}

char get(const charmap_t &charmap, long x, long y) {
	return is_valid(charmap, x, y) ? charmap[(size_t)y][(size_t)x] : ' ';
}

char get(const charmap_t &charmap, size_t x, size_t y) {
	return get(charmap, (long)x, (long)y);
}


bool is_char(const charmap_t &charmap, long x, long y, char c) {
	return is_valid(charmap, x, y) && charmap[(size_t)y][(size_t)x] == c;
}

bool is_char(const charmap_t &charmap, size_t x, size_t y, char c) {
	return is_char(charmap, (long)x, (long)y, c);
}

char set(charmap_t &charmap, long x, long y, char c) {
	if (is_valid(charmap, (long)x, (long)y)) {
		return charmap[(size_t)y][(size_t)x] = c;
	}

	return '\0';
}

char set(charmap_t &charmap, size_t x, size_t y, char c) {
	return set(charmap, (long)x, (long)y, c);
}

std::tuple<size_t, size_t> size(const charmap_t &charmap) {
	size_t x = 0;
	size_t y = charmap.size();
	if (y != 0) {
		x = charmap[0].size();
	}

	return {x, y};
}

std::ostream& operator<<(std::ostream& os, const charmap_t &charmap) {
	std::ostringstream buffer;

	for (auto row : charmap) {
		for (auto location : row) {
			os << location;
		}
		os << "\n";
	}
	os << "\n";
	return os;
}



std::tuple<long, long, int, int> find_start(const charmap_t &charmap, char c = '^') {
	for (auto [y, row] : enumerate(charmap)) {
		for (auto [x, location] : enumerate(row)) {
			if (is_char(charmap, x, y, c)) {
				return {x, y, 0, -1};
			}
		}
	}

	return {-1, -1, 0, 0};
}

// cheesy "hash" of node location and direction
inline int cheese_hash(long x, long y, int dx, int dy) {
	int hash = ((int)x  & 0xFF) << 24
			 | ((int)y  & 0xFF) << 16
			 | (     dx & 0xFF) << 8
			 | (     dy & 0xFF);

	return hash;
}

// path, did_escape (e.g. did not loop)
// the path consists of <x, y, dx, dy> nodes
using path_node_t = std::tuple<long, long, int, int>;
std::tuple<std::vector<path_node_t>, bool> traverse(const charmap_t &charmap, const path_node_t &start) {
	auto [x, y, dx, dy] = start;

	std::set<int> visited;
	std::vector<path_node_t> path;

	while (is_valid(charmap, x, y)) {
		// also stop if we end up in a previously visited location
		// going in the same direction. That would be a loop.
		int hash = cheese_hash(x, y, dx, dy);
		if (visited.find(hash) != visited.end()) {
			break;
		}
		visited.insert(hash);

		// record current location in path
		path.push_back({x, y, dx, dy});
		// set(data, x, y, '+');

		// move to next location
		while (is_char(charmap, x+dx, y+dy, '#')) {
			// keep turing until we find a path.
			// should always be able to go back the way we came.
			int temp = dx;
			dx = -dy;
			dy = temp;
		}

		x += dx;
		y += dy;
	}

	return {path, !is_valid(charmap, x, y)};
}

long part1(const charmap_t charmap) {
	auto [x, y, dx, dy] = find_start(charmap);
 	auto [path, _] = traverse(charmap, {x, y, dx, dy});

	// put all the path locations into a set and then just count
	std::set<std::tuple<long, long>> visited;
	for (auto p : path) {
		auto [x, y, dx, dy] = p;
		visited.insert({x, y});
	}

	return (long)visited.size();
}

// 1922 too low
// 1934 too low
// 1940 to low (last)
// 1976 !!
// 2136 too high (106)
long part2(const data_collection_t charmap) {
	long solution = 0;

	std::vector<std::thread> threads;
	std::mutex solution_lock;

	std::set<int> visited; // we already tried here

	auto [start_x, start_y, start_dx, start_dy] = find_start(charmap);
	auto [path, _] = traverse(charmap, {start_x, start_y, start_dx, start_dy});

	auto prev_x = start_x;
	auto prev_y = start_y;
	auto prev_dx = start_dx;
	auto prev_dy = start_dy;

	for (auto [x, y, dx, dy] : path) {
		if (x == start_x && y == start_y) {
			continue;
		}

		// only test locations we have not tested
		int hash = cheese_hash(x, y, 0, 0);
		if (visited.find(hash) == visited.end()) {
			visited.insert(hash);

			if (thread_ripper) {
				// Spin off another thread to do the work. Join to it at the end
				auto t_func = [&charmap, x, y, prev_x, prev_y, prev_dx, prev_dy, &solution, &solution_lock]{
					charmap_t copy = charmap;
					set(copy, x, y, '#');
					auto [_, escaped] = traverse(copy, {prev_x, prev_y, prev_dx, prev_dy});

					if (!escaped) {
						solution_lock.lock();
						solution++;
						solution_lock.unlock();
					}
				};

				threads.push_back(std::thread(t_func));
			} else {
				data_collection_t test_map = charmap;
				set(test_map, x, y, '#');
				auto [_, escaped] = traverse(test_map, {prev_x, prev_y, prev_dx, prev_dy});
				if (!escaped) {
					solution++;
				}
			}
		}

		// track previous position so we can restart from here.
		// No reason to start way back at the beginning. We already
		// checked all those locations.
		prev_x = x;
		prev_y = y;
		prev_dx = dx;
		prev_dy = dy;
	}

	if (thread_ripper) {
		for (auto &t : threads) {
			t.join();
		}
	}

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	auto lines = read_lines(path);
    data_collection_t map;

    for (const auto& line : lines) {
		std::vector<char> map_row(line.begin(), line.end());
		map.push_back(map_row);
	}

    return map;
}


#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>
#include <unordered_map>

#include "aoc2024.h"
#include "solution.h"


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

bool is_valid(const charmap_t &charmap, size_t x, size_t y) {
	return is_valid(charmap, (long)x, (long)y);
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

bool set(charmap_t &charmap, long x, long y, char c) {
	if (is_valid(charmap, (long)x, (long)y)) {
		charmap[(size_t)y][(size_t)x] = c;
		return true;
	}

	return false;
}

bool set(charmap_t &charmap, size_t x, size_t y, char c) {
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


typedef struct point_t_ {
	char c;
	int x;
	int y;
} point_t;


point_t distance(const point_t &a, const point_t &b) {
	return {'.', a.x-b.x, a.y-b.y};
}

void pp(const point_t &p) {
	std::cout << "(" << p.x << "," << p.y << ")";
}

point_t mirror(const point_t &a, const point_t &b) {
	auto d = distance(a, b);
	return {a.c, a.x+d.x, a.y+d.y};
}

long part1([[maybe_unused]]const data_collection_t data) {
	std::unordered_set<size_t> antinodes;
	std::unordered_map<char, std::vector<point_t>> graph;

	for (const auto [y, row] : enumerate(data)) {
		for (const auto [x, c] : enumerate(row)) {
			if (c != '.') {
				point_t p = {c, (int)x, (int)y};
				graph[c].push_back(p);
			}
		}
	}

	// for each pair of the same letter
	for (const auto &m : graph) {
		const std::vector<point_t> &points = m.second;

		for (size_t i = 0; i < points.size()-1; i++) {
			for (size_t j = i+1; j < points.size(); j++) {
				// pair is points[i] and points[j]
				const point_t &m1 = mirror(points[i], points[j]);
				if (is_valid(data, (size_t)m1.x, (size_t)m1.y)) {
					antinodes.insert((size_t)m1.x << 32 | (size_t)m1.y);
				}

				const point_t &m2 = mirror(points[j], points[i]);
				if (is_valid(data, (size_t)m2.x, (size_t)m2.y)) {
					antinodes.insert((size_t)m2.x << 32 | (size_t)m2.y);
				}
			}
		}
	}

	return static_cast<long>(antinodes.size());
}

long part2([[maybe_unused]] const data_collection_t data) {
	std::unordered_set<size_t> antinodes;
	std::unordered_map<char, std::vector<point_t>> graph;

	for (const auto [y, row] : enumerate(data)) {
		for (const auto [x, c] : enumerate(row)) {
			if (c != '.') {
				point_t p = {c, (int)x, (int)y};
				graph[c].push_back(p);
			}
		}
	}

	// for each pair of the same letter
	for (const auto &m : graph) {
		const std::vector<point_t> &points = m.second;

		for (size_t i = 0; i < points.size()-1; i++) {
			for (size_t j = i+1; j < points.size(); j++) {
				int x, y;
				antinodes.emplace((size_t)points[i].x << 32 | (size_t)points[i].y);
				antinodes.emplace((size_t)points[j].x << 32 | (size_t)points[j].y);

				// pair is points[i] and points[j]
				const point_t &d1 = distance(points[i], points[j]);
				x = points[i].x+d1.x;
				y = points[i].y+d1.y;
				while (is_valid(data, (long)x, (long)y)) {
					antinodes.emplace((size_t)x << 32 | (size_t)y);
					x += d1.x;
					y += d1.y;
				}

				const point_t &d2 = distance(points[j], points[i]);
				x = points[i].x+d2.x;
				y = points[i].y+d2.y;
				while (is_valid(data, (long)x, (long)y)) {
					antinodes.emplace((size_t)x << 32 | (size_t)y);
					x += d2.x;
					y += d2.y;
				}
			}
		}
	}

	return static_cast<long>(antinodes.size());
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


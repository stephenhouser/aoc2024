#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>

#include "aoc2024.h"
#include "solution.h"

using point_t = size_t;

// cheesy "hash" of node location and direction
point_t make_point(long x, long y) {
	point_t hash = ((size_t)x  & 0xFFFFFFFF) << 32
				 | ((size_t)y  & 0xFFFFFFFF);

	return hash;
}

std::tuple<long, long>unmake_point(point_t p) {
	return {
		(p >> 32) & 0xFFFFFFFF,
		(p      ) & 0xFFFFFFFF
	};
}

struct region_t {
	char crop = '\0';
	int area = 0;
	int perimeter = 0;
	std::unordered_set<point_t> points{};

	void add(long x, long y, int boundaries) {
		perimeter += boundaries;
		area += 1;
		points.insert(make_point(x, y));
	}

	bool remove(long x, long y) {
		auto point_it = points.find(make_point(x, y));
		if (point_it != points.end()) {
			points.erase(point_it);
			area -= 1;
			return true;
		}

		return false;
	}

	std::tuple<long, long> pop() {
		auto point = unmake_point(points.extract(points.begin()).value());
		return point;
	}

	bool contains(long x, long y) {
		return points.find(make_point(x, y)) != points.end();
	}
};


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

std::ostream& operator<<(std::ostream& os, const charmap_t &map) {
	std::ostringstream buffer;

	for (const auto &row : map) {
		os << row << "\n";
	}

	return os;
}


bool is_valid(const charmap_t &charmap, long x, long y) {
	return 0 <= x && (size_t)x < charmap[0].size()
	 	&& 0 <= y && (size_t)y < charmap.size();
}

char get(const charmap_t &charmap, long x, long y) {
	return is_valid(charmap, x, y) ? charmap[(size_t)y][(size_t)x] : ' ';
}

bool is_char(const charmap_t &charmap, long x, long y, char c) {
	return is_valid(charmap, x, y) && charmap[(size_t)y][(size_t)x] == c;
}

std::tuple<size_t, size_t> size(const charmap_t &charmap) {
	size_t x = 0;
	size_t y = charmap.size();
	if (y != 0) {
		x = charmap[0].size();
	}

	return {x, y};
}

bool same_region(long x, long y, const region_t &region, const charmap_t &map) {
	if (!is_valid(map, x, y)) {
		return false;
	}

	char crop = get(map, x, y);
	if (crop == region.crop) {
		return true;
	}

	return false;
}


long part1([[maybe_unused]]const data_collection_t map) {
	long solution = 0;
	std::vector<region_t> regions;

	std::cout << map;

	// make a region map from the charmap
	region_t all;
	auto [size_x, size_y] = size(map);
	for (long y = 0; y < (long)size_y; ++y) {
		for (long x = 0; x < (long)size_x; ++x) {
			all.add(x, y, 0);
		}
	}

	while (!all.points.empty()) {
		region_t region;
		// extract point that will start a new region
		auto [rx, ry] = all.pop();		
		region.crop = get(map, rx, ry);
		
		std::cout << "REGION:" << rx << "," << ry << "="<< region.crop << std::endl;
		
		// start tentative list with this point...
		region_t tentnative;
		tentnative.add(rx, ry, 0);

		while (!tentnative.points.empty()) {
			auto [x, y] = tentnative.pop();
			int perimeter = 0;

			if (region.contains(x, y)) {
				continue;
			}

			// std::cout << "TENT:" << tentnative.points.size() << ":" << x << "," << y << "="<< region.crop << std::endl;

			std::vector<std::tuple<long, long>> dirs = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
			for (auto [dx, dy] : dirs) {
				auto c_x = x + dx;
				auto c_y = y + dy;
				if (same_region(c_x, c_y, region, map)) {
					all.remove(c_x, c_y);
					tentnative.add(c_x, c_y, 0);
				} else {
					perimeter += 1;
				}
			}

			region.add(x, y, perimeter);
		}

		regions.push_back(region);
	}
	
	for (const auto &r : regions) {
		auto s = r.area * r.perimeter;
		solution += s;
		std::cout << "crop=" << r.crop 
		          << " area=" << r.area 
		          << " perimeter=" << r.perimeter 
				  << " size=" << s << std::endl;
	}

	return solution;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 2;

	// TODO: part 2 code here

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


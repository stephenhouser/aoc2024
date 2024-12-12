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

	void add(long x, long y) {
		points.insert(make_point(x, y));
	}

	bool has(long x, long y) {
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

	// TODO: part 1 code here
	std::cout << map;

	std::vector<region_t> regions;

	region_t all;
	auto [size_x, size_y] = size(map);
	for (long y = 0; y < (long)size_y; ++y) {
		for (long x = 0; x < (long)size_x; ++x) {
			all.add(x, y);
		}
	}

	while (!all.points.empty()) {
		// extract point that will start a new region
		auto point_h = all.points.extract(all.points.begin());
		auto [x, y] = unmake_point(point_h.value());
		
		region_t region;
		region.crop = get(map, x, y);
		region.add(x, y);
		region.area = 0;
		region.perimeter = 0;
		
		std::cout << "REGION:" << x << "," << y << "="<< region.crop << std::endl;
		
		region_t tentnative;
		tentnative.add(x, y);

		while (!tentnative.points.empty()) {
			auto point_h = tentnative.points.extract(tentnative.points.begin());
			int perimeter = 0;
			auto [x, y] = unmake_point(point_h.value());

			std::cout << "TENT:" << tentnative.points.size() << ":" << x << "," << y << "="<< region.crop << std::endl;

			// check up
			long c_x = x;
			long c_y = y-1;
			point_t c_p = make_point(c_x, c_y);
			
			if (!region.has(c_x, c_y)) {
					if (same_region(c_x, c_y, region, map)) {
					// std::cout << "UP   " << std::endl;
					if (all.has(c_x, c_y)) {
						all.points.erase(all.points.find(c_p));
					}				
					tentnative.add(c_x, c_y);
				} else {
					perimeter += 1;
				}
			}
			//check left
			c_x = x-1;
			c_y = y;
			c_p = make_point(c_x, c_y);

			if (!region.has(c_x, c_y)) {
					if (same_region(c_x, c_y, region, map)) {
					// std::cout << "UP   " << std::endl;
					if (all.has(c_x, c_y)) {
						all.points.erase(all.points.find(c_p));
					}				
					tentnative.add(c_x, c_y);
				} else {
					perimeter += 1;
				}
			}

			// check down
			c_x = x;
			c_y = y+1;
			c_p = make_point(c_x, c_y);

			if (!region.has(c_x, c_y)) {
					if (same_region(c_x, c_y, region, map)) {
					// std::cout << "UP   " << std::endl;
					if (all.has(c_x, c_y)) {
						all.points.erase(all.points.find(c_p));
					}				
					tentnative.add(c_x, c_y);
				} else {
					perimeter += 1;
				}
			}

			// check right
			c_x = x+1;
			c_y = y;
			c_p = make_point(c_x, c_y);

			if (!region.has(c_x, c_y)) {
					if (same_region(c_x, c_y, region, map)) {
					// std::cout << "UP   " << std::endl;
					if (all.has(c_x, c_y)) {
						all.points.erase(all.points.find(c_p));
					}				
					tentnative.add(c_x, c_y);
				} else {
					perimeter += 1;
				}
			}

			region.perimeter += perimeter;
			region.area += 1;
			region.add(x, y);
		}

		regions.push_back(region);
	}

	
	for (const auto &r : regions) {
		auto s = r.area * r.perimeter;
		solution += s;
		std::cout << r.crop << ":" << r.area << "," << r.perimeter << "=" << s << std::endl;
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


#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>
#include <unordered_map>

#include "charmap.h"
#include "aoc2024.h"
#include "solution.h"


using index_t = long;
using value_t = size_t;
using point_t = size_t;

struct pt_t {
	index_t x;
	index_t y;
	index_t z;
	value_t value;

	/* closest to 0 with preference towards closer x */
	bool operator<(const pt_t &other) {		
		// if (this->z < other.z) {
		// 	return true;
		// }

		if (this->y < other.y) {
			return true;
		}

		return this->x < other.x;
	}

	bool operator==(const pt_t &other) {
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}
};

template <>
struct std::hash<pt_t> {
	size_t operator()(const pt_t &p) const {
		return std::hash<size_t>()((((size_t)p.x & 0xFFFFFFFF) << 32) | ((size_t)p.y & 0xFFFFFFFF));
	}
};



// cheesy "hash" of node location and direction
point_t make_point(int x, int y) {
	point_t hash = ((size_t)x  & 0xFFFFFFFF) << 32
				 | ((size_t)y  & 0xFFFFFFFF);

	return hash;
}

std::tuple<int, int>unmake_point(point_t p) {
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
	std::unordered_map<point_t, long> neighbors{};

	void add(int x, int y, int boundaries) {
		perimeter += boundaries;
		area += 1;
		points.insert(make_point(x, y));
		neighbors[make_point(x, y)] = boundaries;
	}

	bool remove(int x, int y) {
		auto point_it = points.find(make_point(x, y));
		if (point_it != points.end()) {
			points.erase(point_it);
			area -= 1;
			return true;
		}

		return false;
	}

	std::tuple<int, int> pop() {
		auto point = unmake_point(points.extract(points.begin()).value());
		return point;
	}

	std::tuple<int, int> first() {
		auto point = unmake_point(*points.begin());
		return point;
	}

	bool contains(int x, int y) {
		return points.find(make_point(x, y)) != points.end();
	}
};


bool same_region(int x, int y, const region_t &region, const charmap_t &map) {
	if (!map.is_valid(x, y)) {
		return false;
	}

	char crop = map.get(x, y);
	if (crop == region.crop) {
		return true;
	}

	return false;
}


long part1(const data_collection_t map) {
	long solution = 0;
	std::vector<region_t> regions;

	if (verbose > 1) {
		std::cout << std::endl << map;
	}

	// make a region map from the charmap
	region_t all;
	for (int y = 0; y < map.size_y; ++y) {
		for (int x = 0; x < map.size_x; ++x) {
			all.add(x, y, 0);
		}
	}

	while (!all.points.empty()) {
		region_t region;
		// extract point that will start a new region
		auto [rx, ry] = all.pop();		
		region.crop = map.get(rx, ry);
		
		// std::cout << "REGION:" << rx << "," << ry << "="<< region.crop << std::endl;
		
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

			std::vector<std::tuple<int, int>> dirs = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
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
		if (verbose > 2) {
			std::cout << "crop=" << r.crop 
					<< " area=" << r.area 
					<< " perimeter=" << r.perimeter 
					<< " size=" << s << std::endl;
		}
	}

	return solution;
}

int os_corners(int x, int y, region_t &region) {
	//NE, SE, SW, NW
	int corners = 0;
	if (!region.contains(x, y-1) && !region.contains(x+1, y)) {
		corners++;
	}
	if (!region.contains(x+1, y) && !region.contains(x, y+1)) {
		corners++;
	}
	if (!region.contains(x, y+1) && !region.contains(x-1, y)) {
		corners++;
	}
	if (!region.contains(x-1, y) && !region.contains(x, y-1)) {
		corners++;
	}

	return corners;
}

int is_corners(int x, int y, region_t &region) {
	//NE, SE, SW, NW
	int corners = 0;
	if (region.contains(x, y-1) && region.contains(x+1, y) && !region.contains(x+1, y-1)) {
		corners++;
	}
	if (region.contains(x+1, y) && region.contains(x, y+1) && !region.contains(x+1, y+1)) {
		corners++;
	}
	if (region.contains(x, y+1) && region.contains(x-1, y) && !region.contains(x-1, y+1)) {
		corners++;
	}
	if (region.contains(x-1, y) && region.contains(x, y-1) && !region.contains(x-1, y-1)) {
		corners++;
	}

	return corners;
}


long part2([[maybe_unused]] const data_collection_t map) {
	long solution = 0;

	std::vector<region_t> regions;
	
	if (verbose > 1) {
		std::cout << std::endl << map;
	}

	// make a region map from the charmap
	region_t all;
	for (int y = 0; y < map.size_y; ++y) {
		for (int x = 0; x < map.size_x; ++x) {
			all.add(x, y, 0);
		}
	}

	while (!all.points.empty()) {
		region_t region;
		// extract point that will start a new region
		auto [rx, ry] = all.pop();		
		region.crop = map.get(rx, ry);
		
		// std::cout << "REGION:" << rx << "," << ry << "="<< region.crop << std::endl;
		
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

			std::vector<std::tuple<int, int>> dirs = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
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

	for (auto &r : regions) {
		long corn = 0;
		// std::cout << "REGION " << r.crop << std::endl;

		for (auto point : r.points) {
			auto [x, y] = unmake_point(point);

			corn += os_corners(x, y, r);
			corn += is_corners(x, y, r);

			if (verbose > 2) {
				std::cout << "\t" << x << "," << y << "=" 
						<< corn << std::endl;
			}
		}

		solution += corn * r.area;
	}

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	charmap_t charmap(read_lines(path));
    return charmap;
}


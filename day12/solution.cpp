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


using index_t = int;
using value_t = size_t;
using point_t = size_t;

struct pt_t {
	index_t x = 0;
	index_t y = 0;
	value_t value = 0;

	pt_t(index_t x, index_t y, value_t v = 0) {
		this->x = x;
		this->y = y;
		this->value = v;
	}

	/* closest to 0 with preference towards closer x */
	bool operator<(const pt_t &other) const {		
		if (this->y < other.y) {
			return true;
		}

		return this->x < other.x;
	}

	bool operator==(const pt_t &other) const {
		return this->x == other.x && this->y == other.y;
	}
};

/* hash function so can be put in unordered_map or set */
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
	value_t area = 0;
	value_t perimeter = 0;
	std::unordered_set<pt_t> points{};

	void add(pt_t &p, value_t boundaries) {
		perimeter += boundaries;
		area += 1;
		points.insert(p);
	}

	void add(index_t x, index_t y, value_t value = 0) {
		pt_t p(x, y, value);
		this->add(p, value);
	}

	bool remove(pt_t &p) {
		auto point_it = points.find(p);
		if (point_it != points.end()) {
			points.erase(point_it);
			area -= 1;
			return true;
		}

		return false;
	}

	pt_t pop() {
		return points.extract(points.begin()).value();
	}

	pt_t first() {
		return *points.begin();
	}

	bool contains(pt_t &p) {
		return points.find(p) != points.end();
	}

	bool contains(index_t x, index_t y) {
		pt_t p(x, y);
		return this->contains(p);
	}

};


bool same_region(pt_t &p, const region_t &region, const charmap_t &map) {
	if (!map.is_valid(p.x, p.y)) {
		return false;
	}

	char crop = map.get(p.x, p.y);
	if (crop == region.crop) {
		return true;
	}

	return false;
}


size_t part1(const data_collection_t map) {
	size_t solution = 0;
	std::vector<region_t> regions;

	if (verbose > 1) {
		std::cout << std::endl << map;
	}

	// make a region map from the charmap
	region_t all;
	for (index_t y = 0; y < map.size_y; ++y) {
		for (index_t x = 0; x < map.size_x; ++x) {
			all.add(x, y, 0);
		}
	}

	while (!all.points.empty()) {
		region_t region;
		// extract point that will start a new region
		pt_t start = all.pop();		
		region.crop = map.get(start.x, start.y);
		
		// std::cout << "REGION:" << rx << "," << ry << "="<< region.crop << std::endl;
		
		// start tentative list with this point...
		region_t tentnative;
		tentnative.add(start, 0);

		while (!tentnative.points.empty()) {
			pt_t p = tentnative.pop();
			value_t perimeter = 0;

			if (region.contains(p)) {
				continue;
			}

			std::vector<std::tuple<int, int>> dirs = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
			for (auto [dx, dy] : dirs) {
				pt_t neighbor(p.x + dx, p.y + dy);
				if (same_region(neighbor, region, map)) {
					all.remove(neighbor);
					tentnative.add(neighbor, 0);
				} else {
					perimeter += 1;
				}
			}

			region.add(p, perimeter);
		}

		regions.push_back(region);
	}
	
	// count perimeters...
	for (const auto &r : regions) {
		size_t s = r.area * r.perimeter;
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


// perhaps could use this and pre-make a 256 member map?
// int check = north << 7 
// 		  | neast << 6
// 		  | east  << 5
// 		  | seast << 4
// 		  | south << 3
// 		  | swest << 2
// 		  | west  << 1
// 		  | nwest;
value_t corners(pt_t &p, region_t &region) {
	value_t corners = 0;

	bool north = region.contains(p.x  , p.y-1);
	bool neast = region.contains(p.x+1, p.y-1);
	bool east  = region.contains(p.x+1, p.y  );
	bool seast = region.contains(p.x+1, p.y+1);
	bool south = region.contains(p.x  , p.y+1);
	bool swest = region.contains(p.x-1, p.y+1);
	bool west  = region.contains(p.x-1, p.y  );
	bool nwest = region.contains(p.x-1, p.y-1);

	// outside corners
	if (!north && !east) {
		corners++;
	}
	if (!east && !south) {
		corners++;
	}
	if (!south && !west) {
		corners++;
	}
	if (!west && !north) {
		corners++;
	}

	// inside corners
	if (north && east && !neast) {
		corners++;
	}
	if (east && south && !seast) {
		corners++;
	}
	if (south && west && !swest) {
		corners++;
	}
	if (west && north && !nwest) {
		corners++;
	}

	return corners;
}

size_t part2(const data_collection_t map) {
	size_t solution = 0;
	std::vector<region_t> regions;

	if (verbose > 1) {
		std::cout << std::endl << map;
	}

	// make a region map from the charmap
	region_t all;
	for (index_t y = 0; y < map.size_y; ++y) {
		for (index_t x = 0; x < map.size_x; ++x) {
			all.add(x, y, 0);
		}
	}

	while (!all.points.empty()) {
		region_t region;
		// extract point that will start a new region
		pt_t start = all.pop();		
		region.crop = map.get(start.x, start.y);
		
		// start tentative list with this point...
		region_t tentnative;
		tentnative.add(start, 0);

		while (!tentnative.points.empty()) {
			pt_t p = tentnative.pop();
			value_t perimeter = 0;

			if (region.contains(p)) {
				continue;
			}

			std::vector<std::tuple<int, int>> dirs = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
			for (auto [dx, dy] : dirs) {
				pt_t neighbor(p.x + dx, p.y + dy);
				if (same_region(neighbor, region, map)) {
					all.remove(neighbor);
					tentnative.add(neighbor, 0);
				} else {
					perimeter += 1;
				}
			}

			region.add(p, perimeter);
		}

		regions.push_back(region);
	}

	// count corners
	for (auto &region : regions) {
		value_t corn = 0;
		for (auto p : region.points) {
			corn += corners(p, region);
		}

		solution += corn * region.area;
	}

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	charmap_t charmap(read_lines(path));
    return charmap;
}


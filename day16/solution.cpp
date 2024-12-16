#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>
#include <unordered_map>
#include <optional>
#include <queue>

#include "point.h"
#include "charmap.h"
#include "aoc2024.h"
#include "solution.h"

class vec_compare {
	public:
		bool operator() (vector_t &a, vector_t &b) {
			return a.p.value > b.p.value;
		}
};

class point_compare {
	public:
		bool operator() (point_t &a, point_t &b) {
			return a.value > b.value;
		}
};


void show_queue(std::priority_queue<vector_t, std::vector<vector_t>, vec_compare> q) {
	std::cout << "Q:";
	for (; !q.empty(); q.pop()) {
		vector_t i = q.top();
		std::cout << i.p << "-" << i.dir << "  ";
	}
	std::cout << std::endl;
}

std::vector<vector_t> neighbors(const charmap_t &map, const vector_t &v) {
	std::vector<vector_t> neighbors;

	auto &p = v.p;
	auto &d = v.dir;

	if (!map.is_char(p.x+d.x, p.y+d.y, '#')) {
		neighbors.push_back(vector_t(p.x+d.x, p.y+d.y, d.x, d.y, 1, 1));
	}

	if (d.x == 0) {
		// going n/s check east and west
		if (!map.is_char(p.x-1, p.y, '#')) {
			// std::cout << " west ";
			neighbors.push_back(vector_t(p.x-1, p.y, -1, 0, 1001, 1));
		}

		if (!map.is_char(p.x+1, p.y, '#')) {
			// std::cout << " east ";
			neighbors.push_back(vector_t(p.x+1, p.y, 1, 0, 1001, 1));
		}
	} else {
		// going e/w check north and south
		if (!map.is_char(p.x, p.y-1, '#')) {
			// std::cout << " north ";
			neighbors.push_back(vector_t(p.x, p.y-1, 0, -1, 1001, 1));
		}

		if (!map.is_char(p.x, p.y+1, '#')) {
			// std::cout << " south ";
			neighbors.push_back(vector_t(p.x, p.y+1, 0, 1, 1001, 1));
		}
	}

	return neighbors;
}

/*
// struct node_t {
// 	point_t p = {0, 0};
// 	std::vector<node_t> neighbors;
// };

// std::vector<node_t> load_nodes(charmap_t &map) {
// 	for (index_t y = 0; y < size_y; y++) {
// 		for (index_t x = 0; x < size_x; x++) {
// 			dist[y*size_x + x] = 100000;
// 			prev[y*size_x + x] = vector_t(-1,-1,0,0,0,0);
// 		}
// 	}
// }

value_t dijkstra(const charmap_t &map, point_t &start) {
	index_t size_x = (index_t)map.size_x;
	index_t size_y = (index_t)map.size_y;

	std::vector<value_t> dist;
	dist.reserve(size_x * size_y);

	std::vector<vector_t> prev;
	prev.reserve(size_x * size_y);

	std::priority_queue<vector_t, std::vector<vector_t>, vec_compare> q;

	for (index_t y = 0; y < size_y; y++) {
		for (index_t x = 0; x < size_x; x++) {
			dist[y*size_x + x] = 100000;
			prev[y*size_x + x] = vector_t(-1,-1,0,0,0,0);
		}
	}

	dist[start.y * size_x + start.x] = 0;
	q.push(vector_t(start.x, start.y, 1, 0, 0, 0));

	std::cout << "start" << std::endl;
	while (!q.empty()) {
		auto u = q.top();
		q.pop();
		std::cout << "eval " << u.p << " going " << u.dir << " cost " << u.p.value << std::endl; 

		for (auto v : neighbors(map, u)) {
			std::cout << "\tneigh: " << v.p << std::endl;
			value_t alt = dist[v.p.y * size_x + v.p.x] + v.p.value;
			if (alt < dist[u.p.y * size_x + u.p.x]) {
				dist[v.p.y * size_x + v.p.x] = alt;
				prev[v.p.y * size_x + v.p.x] = u;
			}
			q.push(v);
		}

		std::cout << "\tdone: " << u.p << std::endl;
	}

	std::vector<point_t> S;
	point_t u(map.find_char('E'));
	while (!map.is_char(u.x, u.y, 'S')) {
		S.push_back(u);

		auto p = prev[u.y * size_x + u.x];
		u.x = p.p.x;
		u.y = p.p.y;
	}

	std::cout << "path ";
	for (auto p : S) {
		std::cout << p << " -> ";
	}
	std::cout << std::endl;

	return 0;
}

value_t find_all(const charmap_t &map, point_t &p, point_t &dir, 
					value_t cost, std::vector<point_t> path,
					std::unordered_set<vector_t> &seen,
					std::vector<std::vector<point_t>> paths) {

	if (map.is_char(p.x, p.y, 'E')) {

		path.push_back(p)
		return cost;
	}

}
*/

value_t find_path(const charmap_t &map, point_t &start, point_t &start_dir, point_t &end) {
	std::priority_queue<vector_t, std::vector<vector_t>, vec_compare> tentative;
	std::unordered_set<vector_t> confirmed;
	std::vector<point_t> path;

	// start at start heading east
	tentative.push(vector_t(start, start_dir));
	
	for (; !tentative.empty(); ) {
		// pop lowest cost node
		auto current = tentative.top();
		tentative.pop();

		auto p = current.p;
		auto cost = p.value;
		auto d  = current.dir;
		auto steps = d.value;

		if (p == end) {
			path.push_back(p);
			continue;
			// return cost;
		}

		// if not on confirmed list
		if (confirmed.find(current) == confirmed.end()) {
			confirmed.insert(current);

			// go straight if we can
			if (!map.is_char(p.x+d.x, p.y+d.y, '#')) {
				tentative.push(vector_t(p.x+d.x, p.y+d.y, d.x, d.y, cost+1, steps+1));
			}

			if (d.x == 0) {
				// going n/s check east and west
				if (!map.is_char(p.x-1, p.y, '#')) {
					tentative.push(vector_t(p.x-1, p.y, -1, 0, cost+1001, steps+1));
				}

				if (!map.is_char(p.x+1, p.y, '#')) {
					tentative.push(vector_t(p.x+1, p.y, 1, 0, cost+1001, steps+1));
				}
			} else {
				// going e/w check north and south
				if (!map.is_char(p.x, p.y-1, '#')) {
					tentative.push(vector_t(p.x, p.y-1, 0, -1, cost+1001, steps+1));
				}

				if (!map.is_char(p.x, p.y+1, '#')) {
					tentative.push(vector_t(p.x, p.y+1, 0, 1, cost+1001, steps+1));
				}
			}
		}

		// show_queue(tentative);
	}

	std::sort(path.begin(), path.end(), [](point_t &a, point_t &b) {
		return a.value < b.value;
	});
	std::cout << "paths:" << path.size() << " ";
	for (auto p : path) {
		std::cout << p << "=" << p.value << std::endl;
	}
	std::cout << std::endl;


	return 0;
}



long part1(const data_collection_t map) {
	long solution = 0;

	// std::cout << map;
	point_t start(map.find_char('S'));
	point_t end(map.find_char('E'));
	point_t direction(1, 0);

	solution = (long)find_path(map, start, direction, end);

	return solution;
}

long part2(const data_collection_t map) {
	long solution = 2;

	// std::cout << map;
	point_t start(map.find_char('S'));
	point_t end(map.find_char('E'));
	point_t direction(1, 0);

	solution = (long)find_path(map, start, direction, end);

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return charmap_t(read_lines(path));
}


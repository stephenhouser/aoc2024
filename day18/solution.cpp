#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <queue>
#include <limits.h>

#include "point.h"
#include "charmap.h"
#include "aoc2024.h"
#include "solution.h"


std::vector<point_t> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

using dist_t = std::map<vector_t, int>;
using pred_t = std::map<vector_t, std::vector<vector_t>>;
std::tuple<size_t, dist_t, pred_t> dijkstra(const charmap_t &map, const point_t &start, const point_t &direction, const point_t &end);

void populate_map(charmap_t &map, const std::vector<point_t> &points, size_t max) {
	for (size_t p = 0; p < max; p++) {
		auto pt = points[p];
		map.set(pt.x, pt.y, '#');
	}
}


long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 1;

	point_t start(0, 0);
	// point_t end(6, 6);
	point_t end(70, 70);

	// std::cout << data << std::endl;
	charmap_t map((size_t)end.x+1, (size_t)end.y+1);
	populate_map(map, data, 1024);

	auto [distance, dist, pred] = dijkstra(map, start, start, end);

	std::cout << map;
	std::cout << distance << " steps" << std::endl;

	return solution;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 2;

	// TODO: part 2 code here

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	data_collection_t data;
	for (auto line : read_lines(path)) {
		data.push_back(split_int(line, ","));
	}

	return data;
}




// used to keep Q in cost order.
// this one works :-)
class compare_cost {
	public:
		bool operator() (vector_t &a, vector_t &b) {
			return b.p.z < a.p.z;
		}
};


std::tuple<size_t, dist_t, pred_t>
dijkstra(const charmap_t &map, 
		 const point_t &start, 
		 const point_t &direction, 
		 const point_t &end) {

	dist_t dist;
	pred_t pred;
	std::priority_queue<vector_t, std::vector<vector_t>, compare_cost> Q;

	// cost is stored in the vector/point's z value

	// vector_t(x, y, cost, dx, dy, _)
	Q.push({start, direction});

	// vector_t(x, y, _, dx, dy, _)
	dist[{start, direction}] = 0;

	while (!Q.empty()) {
		// u <= vertex in Q with min dist[u]
		vector_t u = Q.top();
		// remove u from Q
		Q.pop();

		auto cost = u.p.z;
		u.p.z = 0; // clear so we can insert into map properly

		if (u.p == end) {
			return {cost, dist, pred};
		}

		// for each neighbor v of u in Q
		for (auto direction : directions) {
			vector_t v(u.p+direction, direction);

			int neighbor_cost = cost + 1; // ((direction == u.dir) ? 1 : 1001);

			if (map.is_valid(v.p.x, v.p.y) && !map.is_char(v.p.x, v.p.y, '#')) {

				if (dist.find(v) == dist.end() || neighbor_cost < dist[v]) {
					dist[v] = neighbor_cost;

					pred[v].clear();
					pred[v].push_back(u);

					v.p.z = neighbor_cost;
					Q.push(v);

				} else if (neighbor_cost == dist[v]) {

					pred[v].push_back(u);
				}
			}
		}
	}

	return {INT_MAX, dist, pred};
}

#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <optional>
#include <queue>
#include <limits.h>
#include <cstdint>
#include <tuple>

#include "point.h"
#include "charmap.h"
#include "aoc2024.h"
#include "solution.h"


// void show_queue(std::priority_queue<vector_t, std::vector<vector_t>, vec_compare> q) {
// 	std::cout << "Q:";
// 	for (; !q.empty(); q.pop()) {
// 		vector_t i = q.top();
// 		std::cout << i.p << "-" << i.dir << "  ";
// 	}
// 	std::cout << std::endl;
// }


// this isn't quite working right. wrote it in hopes of eliminating the
// u.p.z = 0; line in dijkstra, hoping I could search without consulting
// the z values. Oh well. it doesnt hurt and shows how you can use
// a custom comparator for map. only used on pred[] list
class compare_vector {
	public:
		bool operator() (const vector_t &a, const vector_t &b) const {
			if (a.p.y == b.p.y) {
				if (a.p.x == b.p.x) {
					if (a.dir.y == b.dir.y) {
						return a.dir.x < b.dir.x;		
					}

					return a.dir.y < b.dir.y;
				}

				return a.p.x < b.p.x;
			}

			return a.p.y < b.p.y;
		}
};


// used to keep Q in cost order.
// this one works :-)
class compare_cost {
	public:
		bool operator() (vector_t &a, vector_t &b) {
			return b.p.z < a.p.z;
		}
};


std::vector<point_t> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

using dist_t = std::map<vector_t, int, compare_vector>;
using pred_t = std::map<vector_t, std::vector<vector_t>>;

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

			int neighbor_cost = cost + ((direction == u.dir) ? 1 : 1001);

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

long find_unique(point_t &end, pred_t &pred) {
	std::set<point_t> unique_tiles;
	std::queue<vector_t> Q;

	// look backwards in all directions
	for (auto direction : directions) {
		Q.push({end, direction});
	}

	// run backwards looking for all the tiles we hit.
	while (!Q.empty()) {
		auto vertex = Q.front();
		Q.pop();

		unique_tiles.insert(vertex.p);
		for (const auto &predecessor : pred[vertex]) {
			Q.push(predecessor);
		}
	}

	return (long)unique_tiles.size();
}

long part1(const data_collection_t map) {

	// std::cout << map;
	point_t start(map.find_char('S'));
	point_t end(map.find_char('E'));
	point_t direction(1, 0);

	auto [cost, dist, pred] = dijkstra(map, start, direction, end);

	return cost;
}

long part2(const data_collection_t map) {
	long solution = 0;

	// std::cout << map;
	point_t start(map.find_char('S'));
	point_t end(map.find_char('E'));
	point_t direction(1, 0);

	auto [cost, dist, pred] = dijkstra(map, start, direction, end);
	solution = find_unique(end, pred);

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return charmap_t(read_lines(path));
}


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

// dijkstra types, code below
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

long part1(const data_collection_t data) {
	point_t start(0, 0);
	point_t end(70, 70);
	size_t time_steps = 1024;

	// heuristic to determine test vs live data.
	if (data.size() < 100) {
		end = {6, 6};
		time_steps = 12;
	}

	charmap_t map((size_t)end.x+1, (size_t)end.y+1);
	populate_map(map, data, time_steps);

	auto [solution, dist, pred] = dijkstra(map, start, start, end);

	if (verbose > 1) {
		std::cout << map;
		std::cout << solution << " steps" << std::endl;
	}

	return (long)solution;
}

// binary search of across time.
// will need to run dijkstra at most ceil(log2(tile_count)) times (12 in our case)
long part2(const data_collection_t data) {
	point_t start(0, 0);
	point_t end(70, 70);

	// heuristic to determine test vs live data.
	if (data.size() < 100) {
		end = {6, 6};
	}

	size_t success_time = 0;
	size_t fail_time = data.size();
	size_t check_time = success_time + (fail_time - success_time) / 2;

	// std::cout << std::endl;
	while (fail_time > success_time + 1) {
		charmap_t map((size_t)end.x+1, (size_t)end.y+1);

		check_time = success_time + (fail_time - success_time) / 2;
		populate_map(map, data, check_time);

		auto [distance, dist, pred] = dijkstra(map, start, start, end);
		if (distance == INT_MAX) {
			// std::cout << check_time << " FAIL (" << success_time << "," << fail_time << ")"<< std::endl;
			fail_time = check_time;
		} else {
			// std::cout << check_time << "   OK (" << success_time << "," << fail_time << ")"<< std::endl;
			success_time = check_time;
		}
	}

	// std::cout << check_time << " DONE (" << success_time << "," << fail_time << ")"<< std::endl;

	if (verbose > 1) {
		std::cout << "at time " << success_time << " block " << data[success_time] << std::endl;
		std::cout << "at time " << fail_time << " block " << data[fail_time] << std::endl;
	}

	return data[success_time].x * 100 + data[success_time].y;
}

// this works but is slow (few seconds)
// faster would be to only recalculate if dropped tile lands on existing shortest path
// then re-dijkstra
// even better, binary search
long part2_slow(const data_collection_t data) {
	size_t solution_time = 0;

	point_t start(0, 0);
	point_t end(70, 70);

	if (test_file) {
		end = {6, 6};
	}

	charmap_t map((size_t)end.x+1, (size_t)end.y+1);
	for (size_t time = 0; time < data.size(); time++) {
		auto pt = data[time];
		map.set(pt.x, pt.y, '#');

		if (time > 1024) {
			auto [distance, dist, pred] = dijkstra(map, start, start, end);
			if (distance == INT_MAX) {
				solution_time = time;
				break;
			}
		}
	}

	if (verbose > 1) {
		std::cout << map;
		std::cout << "at time " << solution_time << " block " << data[solution_time] << std::endl;
	}

	if (solution_time) {		
		return data[solution_time].x * 100 + data[solution_time].y;
	}

	return 0;
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

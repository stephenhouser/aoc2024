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
#include <algorithm>

#include "point.h"
#include "charmap.h"
#include "aoc2024.h"
#include "solution.h"

// dijkstra types, code below
std::vector<point_t> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
using dist_t = std::map<vector_t, int>;
using pred_t = std::map<vector_t, std::vector<vector_t>>;
std::tuple<size_t, dist_t, pred_t> dijkstra(const charmap_t &map, const point_t &start, const point_t &direction, const point_t &end);

int dijkstra_distance(const charmap_t &map, const dist_t &dist, const point_t &p);
std::vector<point_t> dijkstra_path(const point_t &end, const pred_t &pred);
void show_dijkstra_distances(const charmap_t &map, const dist_t &dist);


size_t manhatten_distance(const point_t &p1, const point_t &p2) {
	return (size_t)abs(p1.y - p2.y) + (size_t)abs(p1.x - p2.x);
}

long count_cheats(std::map<vector_t, size_t> cheats, size_t saved_time) {
	return std::accumulate(cheats.begin(), cheats.end(), 0, 
		[saved_time](size_t a, std::pair<vector_t, size_t> b) {
			return (saved_time <= b.second) ? a + 1 : a;
		});
}

// part 1 6943 to high -- doh! greater than 100!
long part1(const data_collection_t map) {
	point_t start{map.find_char('S')};
	point_t end{map.find_char('E')};

	// heuristic for test vs live data
	const bool test_data = map.size_x < 100;

	const auto [distance, dist, pred] = dijkstra(map, start, {1,0}, end);
	auto path{dijkstra_path(end, pred)};
	// show_distances(map, dist);

	std::map<size_t, std::set<vector_t>> cheats;

	for (auto &p : path) {
		size_t current_distance = (size_t)dijkstra_distance(map, dist, p);

		for (auto d : directions) {
			point_t p1{p + d};
			point_t p2{p + d + d};

			if (map.is_char(p1.x, p1.y, '#') && map.is_not_char(p2.x, p2.y, '#')) {
				size_t distance = (size_t)dijkstra_distance(map, dist, p2);
				if (distance != INT_MAX && distance > current_distance) {
					size_t saved = distance-current_distance - 2;
					cheats[saved].insert({p.x, p.y, p2.x, p2.y});
				}
			}
		}
	}

	long solution = std::accumulate(cheats.begin(), cheats.end(), 0, 
		[test_data](size_t a, std::pair<size_t, std::set<vector_t>> b) {
			if (test_data) {
				return a + b.second.size();
			}

			return (b.first >= 100) ? a + b.second.size() : a;
		});

	return solution;
}


std::vector<point_t> possible_cheats(const std::vector<point_t> &path, const point_t &start, size_t max_distance) {
	std::vector<point_t> cheats;

	for (auto end : path) {
		if (start != end && manhatten_distance(start, end) < max_distance) {
			cheats.push_back(end);
		}
	}

	return cheats;
}

// 5932164 too high
// 24678 too low
// 908314 too low
// 1032257 finally!
long part2(const data_collection_t map) {
	point_t start{map.find_char('S')};
	point_t end{map.find_char('E')};

	// heuristic for test vs live data
	const bool test_data = map.size_x < 100;
	const int cheat_distance = 21;

	const auto [distance, dist, pred] = dijkstra(map, start, {1,0}, end);
	auto path{dijkstra_path(end, pred)};
	// show_distances(map, dist);

	// map cheat to max saved time for the cheat.
	// in case we get two cheats that make the same jump, we want the largest savings
	std::map<vector_t, size_t> cheats;

	for (auto &p : path) {
		size_t current_distance = (size_t)dijkstra_distance(map, dist, p);

		for (auto &jump : possible_cheats(path, p, cheat_distance)) {
			size_t m_distance = manhatten_distance(p, jump);
			size_t distance = (size_t)dijkstra_distance(map, dist, jump);

			if (distance != INT_MAX && (current_distance+m_distance) < distance) {
				vector_t cheat{p, jump};
				auto cheat_it = cheats.find(cheat);
				if (cheat_it == cheats.end() || (*cheat_it).second > distance) {
					cheats[cheat] = distance - (current_distance+m_distance);
				}
			}
		}
	}

	return count_cheats(cheats, test_data ? 50 : 100);
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return charmap_t(read_lines(path));
}


// used to keep Q in cstd::coutt order.
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

/* Return the distance of point p from the start using the precomputed dist[]. */
int dijkstra_distance(const charmap_t &map, const dist_t &dist, const point_t &p) {
	if (map.is_char(p.x, p.y, 'S')) {
		return 0;
	}

	int distance = INT_MAX;
	for (auto dir : directions) {
		vector_t v{p.x, p.y, dir.x, dir.y};

		auto dit = dist.find(v);
		if (dit != dist.end()) {
			int d = dit->second;
			if (d && d < distance) {
				distance = d;
			}
		}

	}
	return distance;
}

/* Return the path from start to end using precomputed pred. */
std::vector<point_t> dijkstra_path(const point_t &end, const pred_t &pred) {
	std::vector<point_t> path;
	std::queue<vector_t> Q;

	// look backwards in all directions
	for (auto direction : directions) {
		Q.push({end, direction});
	}

	// run backwards looking for all the tiles we hit.
	while (!Q.empty()) {
		auto vertex = Q.front();
		Q.pop();

		path.push_back(vertex.p);

		auto pit = pred.find(vertex);
		if (pit != pred.end()) {
			std::vector<vector_t> p = pit->second;
			for (const auto &predecessor : p) {
				Q.push(predecessor);
			}
		}
	}

	return path;
}

void show_dijkstra_distances(const charmap_t &map, [[maybe_unused]]const dist_t &dist) {
	int x_width = 3;

	std::cout << "   ";
	for (int x = 0; x < map.size_x; ++x) {
		std::cout << std::setw(x_width) << x;
	}
	std::cout << "\n";

	std::cout << "-+-";
	for (int x = 0; x < map.size_x; ++x) {
		std::cout << "---";
	}
	std::cout << "-+-\n";


	int y = 0;
	for (auto yit = map.data.begin(); yit != map.data.end(); ++yit) {
		std::cout << y % 10 << "| ";

		int x = 0;
		for (auto xit = (*yit).begin(); xit != (*yit).end(); ++xit) {
			int distance = dijkstra_distance(map, dist, {x, y});
			if (distance < INT_MAX) {
				std::cout << std::setw(x_width) << dijkstra_distance(map, dist, {x, y});
			} else {
				std::cout << std::setw(x_width) << " ";
			}

			x++;
		}
		std::cout << " |" << std::setw(x_width) << y << "\n";
		// std::cout << " |" << "\n";
		++y;
	}

	std::cout << "-+-";
	for (size_t x = 0; x < (size_t)map.size_x; ++x) {
		std::cout << "---";
	}
	std::cout << "-+-\n   ";
	for (size_t x = 0; x < (size_t)map.size_x; ++x) {
		std::cout << std::setw(x_width) << x;
	}
	std::cout << "\n";

}
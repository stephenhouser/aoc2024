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

bool test_data = false;



// dijkstra types, code below
std::vector<point_t> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
using dist_t = std::map<vector_t, int>;
using pred_t = std::map<vector_t, std::vector<vector_t>>;
std::tuple<size_t, dist_t, pred_t> dijkstra(const charmap_t &map, const point_t &start, const point_t &direction, const point_t &end);

void show_distances(const charmap_t &map, const dist_t &dist);


int distance_to(const charmap_t &map, const dist_t &dist, const point_t &p) {
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

std::vector<point_t> get_path(const point_t &end, const pred_t &pred) {
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

bool can_move(const charmap_t &map, int x, int y) {
	return map.is_valid(x, y) && !map.is_char(x, y, '#');
}


size_t manhatten(const point_t &p1, const point_t &p2) {
	return (size_t)abs(p1.y - p2.y) + (size_t)abs(p1.x - p2.x);
}


// part 1 6943 to high -- doh! greater than 100!
long part1(const data_collection_t map) {
	point_t start{map.find_char('S')};
	point_t end{map.find_char('E')};

	const auto [distance, dist, pred] = dijkstra(map, start, {1,0}, end);

	// show_distances(map, dist);

	std::map<size_t, std::set<vector_t>> cheats;
	for (auto &p : get_path(end, pred)) {

		size_t current_distance = (size_t)distance_to(map, dist, p);

		for (auto d : directions) {
			point_t p1{p + d};
			point_t p2{p + d + d};

			if (map.is_char(p1.x, p1.y, '#') && map.is_not_char(p2.x, p2.y, '#')) {
				size_t distance = (size_t)distance_to(map, dist, p2);
				if (distance != INT_MAX && distance > current_distance) {
					size_t saved = distance-current_distance - 2;
					cheats[saved].insert({p.x, p.y, p2.x, p2.y});
				}
			}
		}
	}

	// std::cout << "part 1" << std::endl;
	// for (auto &cheat : cheats) {
	// 	std::cout << cheat.first << ": ";
	// 	for (auto &v : cheat.second) {
	// 		std::cout << v << ", ";
	// 	}
	// 	std::cout << std::endl;
	// }

	long solution = std::accumulate(cheats.begin(), cheats.end(), 0, 
		[](size_t a, std::pair<size_t, std::set<vector_t>> b) {
			if (test_data) {
				return a + b.second.size();
			}

			return (b.first >= 100) ? a + b.second.size() : a;
		});

	return solution;
}

std::unordered_set<point_t> possible_cheats(const charmap_t &map, const point_t &p, int distance) {
	std::unordered_set<point_t> pts;

	// std::cout << "possible_cheats ";

	for (int my = -distance; my <= distance; my++) {
		for (int mx = -distance; mx <= distance; mx++) {
			// std::cout << p.x+mx << ", " << p.y+my << ":";

			int m_distance = abs(mx) + abs(my);
			if (m_distance <= distance && map.is_not_char(p.x+mx, p.y+my, '#')) {
				if (mx == 0 && my == 0) {
					continue;
				}
				// std::cout << "insert";
				pts.insert({p.x+mx, p.y+my, m_distance});
			}
			// std::cout << std::endl;
		}
	}

	return pts;
}


// 5932164 too high
// 24678 too low
// 908314 too low
// 1032257
long part2(const data_collection_t map) {
	point_t start{map.find_char('S')};
	point_t end{map.find_char('E')};

	const auto [distance, dist, pred] = dijkstra(map, start, {1,0}, end);

	// show_distances(map, dist);

	std::map<vector_t, size_t> cheats;
	auto paths{get_path(end, pred)};

	for (auto &p : paths) {
		size_t current_distance = (size_t)distance_to(map, dist, p);

		for (auto &jump : paths) {		
			size_t m_distance = manhatten(p, jump);

			if (p != jump && m_distance < 21) {
				size_t distance = (size_t)distance_to(map, dist, jump);
				if (distance != INT_MAX && (current_distance+m_distance) < distance) {
					vector_t cheat{p, jump};
					auto cheat_it = cheats.find(cheat);
					if (cheat_it == cheats.end() || (*cheat_it).second > distance) {
						cheats[cheat] = distance - (current_distance+m_distance);
					}
				}
			}
		}
	}

	long solution = 0;
	std::cout << "part 2" << std::endl;
	for (auto &cheat : cheats) {
		if (test_data && cheat.second >= 50) {
			// std::cout << cheat.second << " " << cheat.first << std::endl;
			solution++;
		} else if (cheat.second >= 100) {
			// std::cout << cheat.second << " " << cheat.first << std::endl;
			solution++;
		}
	}

	// long solution = std::accumulate(cheats.begin(), cheats.end(), 0, 
	// 	[](size_t a, std::pair<size_t, std::set<vector_t>> b) {
	// 		if (test_data) {
	// 			return a + b.second.size();
	// 		}

	// 		return (b.first >= 100) ? a + b.second.size() : a;
	// 	});

	return solution;
}



long part2_xxx(const data_collection_t map) {
	point_t start{map.find_char('S')};
	point_t end{map.find_char('E')};

	const auto [distance, dist, pred] = dijkstra(map, start, {1,0}, end);

	std::map<size_t, std::set<vector_t>> cheats;
	std::map<vector_t, size_t> cheat_saves;

	for (auto &p : get_path(end, pred)) {
		// const point_t p{7,2}; {

		size_t current_distance = (size_t)distance_to(map, dist, p);

		// std::cout << p << "(" << current_distance << ") " << std::endl;

		int len = 3; {
		// for (int len = 2; len < 21; len++) {
			for (auto d : directions) {
				// point_t d{1, 0}; {

				point_t cheat_start{p + d};
				if (map.is_char(cheat_start.x, cheat_start.y, '#')) {

					// std::cout << "\tstart " << cheat_start << std::endl;
					
					for (auto cheat_end : possible_cheats(map, cheat_start, len-1)) {
						size_t distance = (size_t)distance_to(map, dist, cheat_end);

						// std::cout << "\t\tto " << cheat_end << "(" << distance << ") ";

						if (distance > (current_distance + (size_t)len)) {
							// std::cout << " saves " << distance-current_distance-(size_t)len;

							if (distance != INT_MAX && distance > current_distance) {
								size_t saved = (distance - current_distance) + (size_t)len;
								cheat_end.z = (index_t)saved;
								vector_t v{p, cheat_end};

								auto cached = cheat_saves.find(v);
								if (cached == cheat_saves.end() || (*cached).second < saved) {
									cheats[saved].insert({p, cheat_end});
									cheat_saves[v] = saved;					
								}
							}
						}
						// std::cout << std::endl;
					}
				}
			}
		}
	}


	long solution = 0;
	std::cout << std::endl;
	for (auto &save : cheat_saves) {
		if (save.second >= 50) {
			std::cout << save.second << "\t" << save.first << std::endl;
			solution++;
		}
	}

	std::cout << cheat_saves.size() << std::endl;
	// long solution = (long)cheat_saves.size();
	// std::set<vector_t> uniq;

	// std::cout << "part 2" << std::endl;
	// for (auto &cheat : cheats) {
	// 	std::cout << cheat.first << "(" << cheat.second.size() << "): ";
	// 	for (auto &v : cheat.second) {
	// 		std::cout << v << ", ";
	// 		if (cheat.first >= 50) 
	// 			uniq.insert(v);
	// 	}
	// 	std::cout << std::endl;
	// }

	// std::cout << "uniq=" << uniq.size() << std::endl;

	// long solution = std::accumulate(cheats.begin(), cheats.end(), 0, 
	// 	[](size_t a, std::pair<size_t, std::set<vector_t>> b) {
	// 		if (test_data) {
	// 			return a + b.second.size();
	// 		}
			
	// 		return (b.first >= 100) ? a + b.second.size() : a;
	// 	});

	return solution;
}


/*
long part2(const data_collection_t map) {
	point_t start{map.find_char('S')};
	point_t end{map.find_char('E')};

	const auto [distance, dist, pred] = dijkstra(map, start, {1,0}, end);

	// show_distances(map, dist);
	// std::cout << std::endl;

	std::map<size_t, std::set<vector_t>> cheats;
	for (auto &p : get_path(end, pred)) {
		// const point_t p{1, 3}; {

		size_t current_distance = (size_t)distance_to(map, dist, p);

		for (auto dir : directions) {
			if (map.is_char(p.x+dir.x, p.y+dir.y, '#')) {
				point_t cheat_start{p.x+dir.x, p.y+dir.y};

				// std::cout << "check" << cheat_start << std::endl;

				for (int len = 2; len < 3; len++) {
					for (auto cheat_end : find_d(map, cheat_start, len)) {

						// std::cout << "\t" << len << " to " << cheat_end 
						// 		  << " is " << manhatten(cheat_start, cheat_end) << std::endl;

						size_t distance = (size_t)distance_to(map, dist, cheat_end);
						if (distance != INT_MAX && distance > current_distance) {
							size_t saved = distance - current_distance - (size_t)len;
								if (saved > 1000) {
									std::cout << "overflow " << p << " to " << cheat_end << std::endl;
								} else {
									cheats[saved].insert({p.x, p.y, cheat_end.x, cheat_end.y});
								}
						}
					}
				}
			}
		}
	}

	std::set<vector_t> unique_cheats;

	for (auto &cheat : cheats) {
		if (cheat.first >= 1) {
			std::cout << cheat.first << " " << cheat.second.size() << ": ";
			for (auto &v : cheat.second) {
				unique_cheats.insert(v);
				
				std::cout << v << ", ";
			}
		std::cout << std::endl;
		}
	}

	std::cout << "uniqie = " << unique_cheats.size() << std::endl;

	// long solution = std::accumulate(cheats.begin(), cheats.end(), 0, 
	// 	[](size_t a, std::pair<size_t, std::set<vector_t>> b) {
	// 		return (b.first >= 50) ? a + b.second.size() : a;
	// 	});

	return 10;
}
*/
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

	// cstd::coutt is stored in the vector/point's z value

	// vector_t(x, y, cstd::coutt, dx, dy, _)
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




void show_distances(const charmap_t &map, [[maybe_unused]]const dist_t &dist) {
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
			int distance = distance_to(map, dist, {x, y});
			if (distance < INT_MAX) {
				std::cout << std::setw(x_width) << distance_to(map, dist, {x, y});
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
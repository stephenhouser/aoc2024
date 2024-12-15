#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>
#include <unordered_map>
#include <optional>

#include "point.h"
#include "charmap.h"
#include "aoc2024.h"
#include "solution.h"

bool can_move(int x, int y, int dx, int dy, const charmap_t &map) {
	// off map
	if (!map.is_valid(x+dx, y+dy)) {
		return false;
	}

	// skip boxes
	while (map.is_char(x+dx, y+dy, 'O')) {
		x += dx;
		y += dy;
	}

	// wall
	return !map.is_char(x+dx, y+dy, '#');
}

void push_boxes(int x, int y, int dx, int dy, charmap_t &map) {
	// skip boxes
	int fill_x = x + dx;
	int fill_y = y + dy;
	while (map.is_char(fill_x, fill_y, 'O')) {
		fill_x += dx;
		fill_y += dy;
	}

 	map.set(fill_x, fill_y, 'O');
	map.set(x+dx, y+dy, '.');
}

std::unordered_map<char, std::tuple<int, int>> directions = {
	{'^', {  0, -1}},
	{'v', {  0,  1}},
	{'<', { -1,  0}},
	{'>', {  1,  0}},
};

void move_robot(std::pair<int, int> &robot, char dir_char, charmap_t &map) {
	auto [dx, dy] = directions[dir_char];

	if (can_move(robot.first, robot.second, dx, dy, map)) {
		push_boxes(robot.first, robot.second, dx, dy, map);

		map.set(robot.first, robot.second, '.');
		robot.first += dx;
		robot.second += dy;
		map.set(robot.first, robot.second, '@');
	}
}


long part1(data_collection_t data) {
	auto &[map, directions] = data;
	std::pair<int, int> robot = map.find_char('@');

	if (verbose > 2) {
		std::cout << robot.first << "," << robot.second << std::endl;
		std::cout << map;
	}

	for (auto direction : directions) {
		move_robot(robot, direction, map);
	}

	if (verbose > 1) {
		std::cout << robot.first << "," << robot.second << std::endl;
		std::cout << map;
	}

	long solution = 0;
	for (int y = 1; y < map.size_y - 1; y++) {
		for (int x = 1; x < map.size_x - 1; x++) {
			if (map.is_char(x, y, 'O')) {
				solution += (x) + (y)*100;
			}
		}
	}

	return solution;
}

charmap_t embiggen(const charmap_t &map) {
	charmap_t big;

	for (auto small_line : map.data) {
		std::vector<char> big_line;
		for (auto small_symbol : small_line) {
			switch (small_symbol) {
				case '@':
					big_line.push_back(small_symbol);
					big_line.push_back('.');
					break;
				case 'O':
					big_line.push_back('[');
					big_line.push_back(']');
					break;
				default:
					big_line.push_back(small_symbol);
					big_line.push_back(small_symbol);
					break;

			}
		}

		big.add_line(big_line);
	}

	return big;
}

void move_boxes(std::unordered_set<point_t> boxes, int dx, int dy, charmap_t &map) {
	std::vector<point_t> moves(boxes.begin(), boxes.end());

	if (dx == -1) {
		std::sort(moves.begin(), moves.end(), [](const point_t &a, const point_t &b) {
			return a.x < b.x;
		});
	} else if (dx == 1) {
		std::sort(moves.begin(), moves.end(), [](const point_t &a, const point_t &b) {
			return a.x > b.x;
		});
	} else if (dy == -1) {
		std::sort(moves.begin(), moves.end(), [](const point_t &a, const point_t &b) {
			return a.y < b.y;
		});
	} else {
		std::sort(moves.begin(), moves.end(), [](const point_t &a, const point_t &b) {
			return a.y > b.y;
		});
	}
	
	for (auto box : moves) {
		map.set(box.x, box.y, '.');
		map.set(box.x+1, box.y, '.');

		map.set(box.x+dx, box.y+dy, '[');
		map.set(box.x+dx+1, box.y+dy, ']');
	}
}

/* can we move what is in x,y to x+dx, x+dy*/
std::optional<std::unordered_set<point_t>>
will_move2(int x, int y, int dx, int dy, const charmap_t &map) {
	std::unordered_set<point_t> to_move;

	// std::cout << "check:" << x << "," << y << ":" << dx << "," << dy << std::endl;

	if (map.is_char(x+dx, y+dy, '#')) {
		return std::nullopt;
	}

	if (map.is_char(x+dx, y+dy, '.')) {
		return to_move;
	}

	if (dy == 0) {
		// horizontal
		while (map.is_char(x+dx, y, '[') || map.is_char(x+dx, y, ']')) {
			if (map.is_char(x+dx, y, '[')) {
				point_t p(x+dx, y);
				to_move.insert(p);
			}

			if (map.is_char(x+dx, y, '#')) {
				return std::nullopt;
			}

			x += dx;
		}
		return to_move;
	} else {
		// align x to left side of box
		if (map.is_char(x, y+dy, ']')) {
			x--;
		}

		if (map.is_char(x, y+dy, '#') || map.is_char(x+1, y+dy, '#')) {
 			return std::nullopt;
		}

		if (map.is_char(x, y, '[') || map.is_char(x, y+dy, ']')) {
			auto moves = will_move2(x, y+dy, dx, dy, map);
			if (!moves.has_value()) {
				return std::nullopt;
			}

			to_move.merge(moves.value());
		} 
		
		if (map.is_char(x+1, y, ']') || map.is_char(x+1, y+dy, '[')) {
			auto moves = will_move2(x+1, y+dy, dx, dy, map);
			if (!moves.has_value()) {
				return std::nullopt;
			}

			to_move.merge(moves.value());
		}
	}

	point_t p(x+dx, y+dy);
	to_move.insert(p);
	return to_move;
}


std::unordered_set<point_t> will_move(int x, int y, int dx, int dy, const charmap_t &map) {
	std::unordered_set<point_t> to_move;

	if (dy == 0) {
		// horizontal
		while (map.is_char(x, y, '[') || map.is_char(x, y, ']')) {
			if (map.is_char(x, y, '[')) {
				point_t p(x, y);
				to_move.insert(p);
			}

			x += dx;
		}
	} else if (!map.is_char(x, y, '.')) {
		// align x to left side of box
		if (map.is_char(x, y, ']')) {
			x--;
		}

		point_t p(x, y);
		to_move.insert(p);

		if (map.is_char(x, y+dy, '[') || map.is_char(x, y+dy, ']')) {
			to_move.merge(will_move(x, y+dy, dx, dy, map));
		}

		if (map.is_char(x+1, y+dy, '[') || map.is_char(x+1, y+dy, ']')) {
			to_move.merge(will_move(x+1, y+dy, dx, dy, map));
		}
	}

	return to_move;
}

/* can we push what is at x,y to x+dx, y+dy */
bool can_move_big(int x, int y, int dx, int dy, const charmap_t &map) {
	if (map.is_char(x+dx, y+dy, '.')) {
		return true;
	}

	if (map.is_char(x+dx, y+dy, '#')) {
		return false;
	}


	if (dy == 0) {
		// horizontal
		while (map.is_char(x+dx, y, '[') || map.is_char(x+dx, y, ']')) {
			x += dx;
		}
		return map.is_char(x+dx, y, '.');
	} else {
		// vertical

		// align x to left side of box
		if (map.is_char(x, y+dy, ']')) {
			x--;
		}

		return can_move_big(x, y+dy, dx, dy, map) && can_move_big(x+1, y+dy, dx, dy, map);
	}
}


void move_robot_big(point_t &robot, char dir_char, charmap_t &map) {
	auto [dx, dy] = directions[dir_char];

	if (can_move_big(robot.x, robot.y, dx, dy, map)) {
		std::unordered_set<point_t> mv = will_move(robot.x+dx, robot.y+dy, dx, dy, map);
	// auto moves = will_move2(robot.x, robot.y, dx, dy, map);
	// if (moves.has_value()) {
	// 	std::cout << "\tmoves[" << dir_char << "]";
	// 	for (auto m : moves.value()) {
	// 		std::cout << m << " ";
	// 	}
	// 	std::cout << std::endl;

		move_boxes(mv, dx, dy, map);

		map.set(robot.x, robot.y, '.');
		robot.x += dx;
		robot.y += dy;
		map.set(robot.x, robot.y, '@');
	}
}


long part2(const data_collection_t data) {

	auto &[small_map, directions] = data;
	charmap_t map(embiggen(small_map));

	point_t robot(map.find_char('@'));

	if (verbose > 2) {
		std::cout << robot << std::endl;
		std::cout << map;
	}

	for (auto direction : directions) {
		move_robot_big(robot, direction, map);

		if (verbose > 3) {
			std::cout << robot << std::endl;
			std::cout << map;
		}
	}

	if (verbose > 1) {
		std::cout << robot << std::endl;
		std::cout << map;
	}

	long solution = 0;
	for (int y = 1; y < map.size_y - 1; y++) {
		for (int x = 1; x < map.size_x - 1; x++) {
			if (map.is_char(x, y, '[')) {
				solution += (x) + (y)*100;
			}
		}
	}

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	auto lines = read_lines(path);

	charmap_t map;
	std::vector<char> directions;

	bool reading_directions = false;
	for (auto &line : lines) {
		if (line.size() > 0) {
			if (reading_directions) {
				std::vector<char> direction_row(line.begin(), line.end());
				directions.insert(directions.end(), direction_row.begin(), direction_row.end());
			} else {
 				map.add_line(line);
			}
		} else {
			reading_directions = true;
		}
	}

	return {map, directions};
}


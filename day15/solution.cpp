#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_set>

#include "charmap.h"
#include "aoc2024.h"
#include "solution.h"

bool show = false;

using index_t = int;
using value_t = size_t;

struct point_t {
	index_t x = 0;
	index_t y = 0;

	point_t(index_t x, index_t y) {
		this->x = x;
		this->y = y;
	}

	/* closest to 0 with preference towards closer x */
	bool operator<(const point_t &other) const {		
		if (this->y < other.y) {
			return true;
		}

		return this->x < other.x;
	}

	bool operator==(const point_t &other) const {
		return this->x == other.x && this->y == other.y;
	}
};

std::ostream& operator<<(std::ostream& os, const point_t &p) {
	std::ostringstream buffer;
	os << "(" << p.x << "," << p.y << ")";
	return os;
}

/* hash function so can be put in unordered_map or set */
template <>
struct std::hash<point_t> {
	size_t operator()(const point_t &p) const {
		return std::hash<size_t>()((((size_t)p.x & 0xFFFFFFFF) << 32) | ((size_t)p.y & 0xFFFFFFFF));
	}
};

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

void move_robot(std::pair<int, int> &robot, char dir_char, charmap_t &map) {
	int dx = 0;
	int dy = 0;
	switch (dir_char) {
		case '^':
			dx = 0;
			dy = -1;
			break;

		case '>':
			dx = 1;
			dy = 0;
			break;

		case '<':
			dx = -1;
			dy = 0;
			break;
		case 'v':
			dx = 0;
			dy = 1;
			break;
	}

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


bool can_move_big(int x, int y, int dx, int dy, const charmap_t &map) {
	if (show) 
		std::cout << "\tcan (" << x << "," << y << ")=" << map.get(x, y)
	    	      << " (" << dx << "," << dy << ")=" << map.get(x+dx, y+dy) ;

	if (dy == 0) {
		// horizontal
		while (map.is_char(x+dx, y, '[') || map.is_char(x+dx, y, ']')) {
			// std::cout << "\tcan (" << x << "," << y << ") dx " << dx << " = " << map.get(x+dx, y) << std::endl;
			x += dx;
		}
		bool can = map.is_char(x+dx, y, '.');
		if (show) std::cout << can << std::endl;
		return can;
	} else {
		// vertical
		if (map.is_char(x, y+dy, '.')) {
			if (show) std::cout << " TRUE" << std::endl;
			return true;
		}

		if (map.is_char(x, y+dy, '#')) {
			if (show) std::cout << " FALSE" << std::endl;
			return false;
		}

		// align x to left side of box
		if (map.is_char(x, y+dy, ']')) {
			x--;
		}

		bool can = can_move_big(x, y+dy, dx, dy, map) && can_move_big(x+1, y+dy, dx, dy, map);
		if (show) std::cout << can << std::endl;
		return can;
	}
}


void move_robot_big(std::pair<int, int> &robot, char dir_char, charmap_t &map) {
	int dx = 0;
	int dy = 0;
	switch (dir_char) {
		case '^':
			dx = 0;
			dy = -1;
			break;

		case '>':
			dx = 1;
			dy = 0;
			break;

		case '<':
			dx = -1;
			dy = 0;
			break;
		case 'v':
			dx = 0;
			dy = 1;
			break;
	}

	if (can_move_big(robot.first, robot.second, dx, dy, map)) {
		std::unordered_set<point_t> mv = will_move(robot.first+dx, robot.second+dy, dx, dy, map);
		move_boxes(mv, dx, dy, map);

		map.set(robot.first, robot.second, '.');
		robot.first += dx;
		robot.second += dy;
		map.set(robot.first, robot.second, '@');
	}
}


long part2([[maybe_unused]] const data_collection_t data) {

	auto &[small_map, directions] = data;
	charmap_t map(embiggen(small_map));

	std::pair<int, int> robot = map.find_char('@');

	if (verbose > 2) {
		std::cout << robot.first << "," << robot.second << std::endl;
		std::cout << map;
	}

	for (auto direction : directions) {
		move_robot_big(robot, direction, map);
		
		if (verbose > 3) {
			std::cout << " " << robot.first << "," << robot.second << std::endl;
			std::cout << map;
		}
	}

	if (verbose > 1) {
		std::cout << " " << robot.first << "," << robot.second << std::endl;
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


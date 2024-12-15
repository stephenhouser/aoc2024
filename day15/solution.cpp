#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

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
	// const charmap_t &map = data.first;
	// const std::vector<char> &directions = data.second;

	auto &[map, directions] = data;

	std::pair<int, int> robot = map.find_char('@');

	std::cout << robot.first << "," << robot.second << std::endl;
	std::cout << map;

	for (auto direction : directions) {
		move_robot(robot, direction, map);
	}

	//std::cout << direction << " " << robot.first << "," << robot.second << std::endl;
	std::cout << map;

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

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 2;

	// TODO: part 2 code here

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


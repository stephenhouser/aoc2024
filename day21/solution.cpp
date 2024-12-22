#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <map>
#include <set>
#include <limits.h>

#include "aoc2024.h"
#include "solution.h"


/*
+---+---+---+
| 7 | 8 | 9 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
    | 0 | A |
    +---+---+
*/
std::map<char, std::vector<std::vector<std::string>>> _numpad_codes = {
	{'0', {{""}, {"^<"}, {"^"}, {"^>",">^"}, {"^^<","^<^"}, {"^^"}, {">^^","^>^","^^>"}, {"^^^<","^^<^", "^<^^"}, {"^^^"}, {"^^^>","^^>^", "^>^^", ">^^^"}, {">"}}},
	{'1', {{">v"}, {""}, {">"}, {">>"}, {"^"}, {"^>",">^"}, {"^>>",">^>","^>>"}, {"^^"}, {">^^","^>^","^^>"}, {"^^>>","^>^>",">^>^","^^>>","^>>^",">^^>"}, {">>v",">v>"}}},
	{'2', {{"v"}, {"<"}, {""}, {">"}, {"<^","^<"}, {"^"}, {"^>",">^"}, {"<^^","^<^","^^<"}, {"^^"}, {">^^","^>^","^^>"}, {"v>",">v"}}},
	{'3', {{"<v","v<"}, {"<<"}, {"<"}, {""}, {"<<^","<^<","^<<"}, {"<^","^<"}, {"^"}, {"<<^^","<^<^","<^^<","^<<^","^<^<","^^<<"}, {"<^^","^<^","^^<"}, {"^^"}, {"v"}}},
	{'4', {{">vv","v>v"}, {"v"}, {"v>"}, {"v>>",">v>",">>v"}, {""}, {">"}, {">>"}, {"^"}, {"^>",">^"}, {"^>>",">^>",">>^"}, {"<<vv","<v<v","<vv<","v<<v","v<v<","vv<<"}}},
	{'5', {{"vv"}, {"<v","v<"}, {"v"}, {"v>",">v"}, {"<"}, {""}, {">"}, {"<^","^<"}, {"^"}, {"^>",">^"}, {">vv","v>v","vv>"}}},
	{'6', {{"<vv","v<v","vv<"}, {"<<v","<v<","v<<"}, {"<v","v<"}, {"v"}, {"<<"}, {"<"}, {""}, {"<<^","<^<","^<<"}, {"<^","^<"}, {"^"}, {"vv"}}},
	{'7', {{">vvv","v>vv","vv>v",">vvv"}, {"vv"}, {"vv>","v>v",">vv"}, {"vv>>","v>v>",">>vv", "v>>v", ">vv>", ">v>v"}, {"v"}, {"v>",">v"}, {"v>>",">v>",">>v"}, {""}, {">"}, {">>"}, 
			/* A */ {">>vvv",">v>vv",">vv>v",">vvv>","v>>vv","v>v>v","v>vv>","vv>>v","vv>v>"}}},
	{'8', {{"vvv"}, {"<vv","v<v","vv<"}, {"vv"}, {"vv>","v>v",">vv"}, {"<v","v<"}, {"v"}, {"v>",">v"}, {"<"}, {""}, {">"}, {">vvv","v>vv","vv>v","vvv>"}}},
	{'9', {{"<vvv","v<vv","vv<v","vvv<"}, {"<<vv","<v<v","<vv<","v<<v","v<v<","vv<<"}, {"<vv","v<v","vv<"}, {"vv"}, {"<<v","<v<","v<<"}, {"<v","v<"}, {"v"}, {"<<"}, {"<"}, {""}, {"vvv"}}},
	{'A', {{"<"}, {"^<<","<^<"}, {"<^","^<"}, {"^"}, {"^^<<","<^^<","<^<^","^<^<","^<<^"}, {"<^^","^<^","^^<"}, {"^^"}, 
			/* 7* */ {"<^<^^","<^^<^","<^^^<","^<<^^","^<^<^","^<^^<","^^<<^","^^<^<"}, 
					 {"<^^^","^<^^","^^<^","^^^<"}, {"^^^"}, {""}}}
};

// returns the number of steps to move from current to next on numeric keypad
std::vector<std::string> numpad_codes(const char current, const char next) {
	size_t next_index = (next == 'A') ? 10 : (size_t)(next-'0');
	return _numpad_codes[current][next_index];
}

/*
    +---+---+
    | ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+
*/
std::map<char, std::vector<std::vector<std::string>>> _dirpad_codes = {
		// ^       	 	v   	 		<    	 				>    			A
	{'^', {{""},   		{"v"},   		{"v<"},  				{"v>", ">v"}, 	{">"}   		}},
	{'v', {{"^"},  		{""},    		{"<"},   				{">"}, 			{"^>", ">^"}  	}},
	{'<', {{">^"}, 		{">"},   		{""},    				{">>"},			{">>^", ">^>"} 	}},
	{'>', {{"<^", "^<"},{"<"},   		{"<<"},  				{""},  			{"^"}	  		}},
	{'A', {{"<"}, 		{"<v", "v<"},  	{"v<<", "<v<", "v<<"}, 	{"v"}, 			{""}	 		}}
};
// returns the number of steps to move from current to next on movement keypad
std::vector<std::string> dirpad_codes(const char current, const char next) {
	std::vector moves_vec = _dirpad_codes[current];

	switch (next) {
		case '^':
			return moves_vec[0];
		case 'v':
			return moves_vec[1];
		case '<':
			return moves_vec[2];
		case '>':
			return moves_vec[3];
		case 'A':
			return moves_vec[4];
	}

	return {};
}

size_t robot(const char current, const char next, int depth, bool numpad) {
	if (depth == 0) {
		auto codes = dirpad_codes(current, next);
		return codes[0].length() + 1;
	}

	// std::string min_code;
	size_t min_cost = INT_MAX;

	// possible paths
	auto possible = numpad ? numpad_codes(current, next) : dirpad_codes(current, next);
	for (auto code : possible) {
		size_t cost = 0;
		char current = 'A';

		for (auto c : code+"A") {
			cost += robot(current, c, depth-1, false);
			current = c;
		}

		if (cost < min_cost) {
			min_cost = cost;
			// min_code = code;
		}
	}

	// std::cout << min_code;
	return min_cost;
}


// work code 938A --> 72 (d2), 32 (d1)
// v<<A>>^AAAvA^Av<<A>A^>AA<Av>A^A<vA<AA>>^AvA<^A>AAvA^A<vA^>Av<<A>>^AAA<Av>A^A
// 74
// 68
// 70
// 70
// 72
// 176452

// 177524 too high
// 175892 too low
// 176452
long part1(const data_collection_t data) {
	size_t solution = 0;
	int depth = 2;

	for (auto &code : data) {
		size_t cost = 0;

		char current = 'A';
		for (auto c : code) {
			cost += robot(current, c, depth, true);
			current = c;
		}

		if (verbose > 1) {
			std::cout << "Code " << code << " = " << cost << " * " << atol(code.c_str()) << std::endl;
		}
		solution += cost * (size_t)atol(code.c_str());
	}

	return (long)solution;
}


long part2(const data_collection_t data) {
	size_t solution = 0;
	int depth = 25;

	for (auto &code : data) {
		size_t cost = 0;

		char current = 'A';
		for (auto c : code) {
			cost += robot(current, c, depth, true);
			current = c;
		}

		if (verbose > 1) {
			std::cout << "Code " << code << " = " << cost << " * " << atol(code.c_str()) << std::endl;
		}
		solution += cost * (size_t)atol(code.c_str());
	}

	return (long)solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return read_lines(path);
}


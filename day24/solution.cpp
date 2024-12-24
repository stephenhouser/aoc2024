#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <cassert>


#include "aoc2024.h"
#include "solution.h"

// using wire_t = size_t;

// wire_t wire_number(std::string &str) {
// 	assert(str.length() == 3);

// 	return ((str[0] & 0xFF) << 16)
// 		 | ((str[1] & 0xFF) <<  8)
// 		 | ((str[2] & 0xFF));
// }

// struct wire_t {
// 	std::string name;
// 	int value;
// 	std::string in;
// 	std::string out;
// };

void print_node(const node_t &node) {
	std::cout << node.a;
	switch (node.op) {
		case AND:
			std::cout << " & ";
			break;
		case OR:
			std::cout << " | ";
			break;
		case XOR:
			std::cout << " ^ ";
			break;
		default:
			std::cout << " ? ";
			break;
	}
	std::cout << node.b << " => " << node.out;
}

std::vector<std::string> find_output_wires(const std::map<std::string, int> &wires) {
	std::vector<std::string> output_wires;

	for (const auto &wire_pair : wires) {
		if (wire_pair.first.find("z", 0) == 0) {
			output_wires.push_back(wire_pair.first);
		}
	}

	std::sort(output_wires.begin(), output_wires.end(), std::greater<std::string>());
	return output_wires;
}

bool output_ready(std::map<std::string, int> &wires) {
	std::vector<std::string> output_wires = find_output_wires(wires);
	for (auto &wire : output_wires) {
		if (wires[wire] < 0) {
			return false;
		}
	}

	return true;
}

size_t z_value(std::map<std::string, int> &wires) {
	std::vector<std::string> output_wires = find_output_wires(wires);

	size_t result = 0;
	for (auto &wire : output_wires) {
		// assert(wires[wire] >= 0);
		result = result << 1;

		if (wires[wire] >= 0) {
			result += (size_t)(wires[wire] & 0x01);
			std::cout << (wires[wire] & 0x01);
		} else {
			std::cout << ".";
		}

	}

	std::cout << std::endl;

	return result;
}

int node_value(const node_t &node, std::map<std::string, int> &wires) {
	if (wires[node.a] < 0 || wires[node.b] < 0) {
		return -1;
	}
	
	switch (node.op) {
		case AND:
			return (wires[node.a] & wires[node.b]) & 0x01;
		case OR:
			return (wires[node.a] | wires[node.b]) & 0x01;
		case XOR:
			return (wires[node.a] ^ wires[node.b]) & 0x01;
	}

	return -1;
}


// 0 not the right answer
// 59336987801432 correct answer, nodes affect wires immediately
long part1([[maybe_unused]] const data_collection_t data) {
	long solution = 1;

	// std::map<std::string, int>, std::vector<node_t>>
	auto [wires, nodes] = data;

	// for (auto wire : wires) {
	// 	std::cout << wire.first << " = " << wire.second << std::endl;
	// }
	// std::cout << std::endl;

	// for (auto node : nodes) {
	// 	print_node(node);
	// 	std::cout << std::endl;
	// }

	int i = 0;
	while (!output_ready(wires)) {
		std::cout << i++ << ": ";

		for (auto &node : nodes) {
			wires[node.out] = node_value(node, wires);
		}

		z_value(wires);
	}

	std::cout << std::endl;
	solution = (long)z_value(wires);
	return solution;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 2;

	// TODO: part 2 code here

	return solution;
}

bool contains(const std::map<std::string, int> &haystack, const std::string &needle) {
	return haystack.find(needle) != haystack.end();
}

void add_wire(std::map<std::string, int> wires, const std::string &name, int value) {
	wires[name] = value;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	std::map<std::string, int> wires;
	std::vector<node_t> nodes;

	bool read_wires = true;
	for (auto &line : read_lines(path)) {
		
		if (line.empty()) {
			read_wires = false;
			continue;
		}

		if (read_wires) {
			// y02: 0
			auto w = split(line, " ");
			w[0].pop_back();

			wires[w[0]] = atoi(w[1].c_str());
		} else {
			// x00 AND y00 -> z00
			auto n = split(line, " ");

			std::string &in_a = n[0];
			if (!contains(wires, in_a)) {
				wires[in_a] = -1;
			}

			std::string &in_b = n[2];
			if (!contains(wires, in_b)) {
				wires[in_b] = -1;
			}

			std::string &out  = n[4];
			if (!contains(wires, out)) {
				wires[out] = -1;
			}

			gate_t op;
			if (n[1] == "AND") {
				op = AND;
			} else if (n[1] == "OR") {
				op = OR;
			} else {
				op = XOR;
			}

			nodes.push_back({op, in_a, in_b, out});
		}
	}

	return {wires, nodes};
}


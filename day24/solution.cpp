#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <cassert>
#include <bitset>

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

std::vector<std::string> find_wires(const network_t &wires, const std::string &prefix) {
	std::vector<std::string> output_wires;

	for (const auto &wire_pair : wires) {
		if (wire_pair.first.find(prefix, 0) == 0) {
			output_wires.push_back(wire_pair.first);
		}
	}

	return output_wires;
}

bool bus_ready(network_t &wires, const std::string &prefix = "z") {
	std::vector<std::string> output_wires = find_wires(wires, prefix);
	for (auto &wire : output_wires) {
		if (wires[wire] < 0) {
			return false;
		}
	}

	return true;
}

size_t bus_value(network_t &wires, const std::string &prefix = "z") {
	std::vector<std::string> output_wires = find_wires(wires, prefix);
	std::sort(output_wires.begin(), output_wires.end(), std::greater<std::string>());

	size_t result = 0;
	for (auto &wire : output_wires) {
		assert(wires[wire] >= 0);
		result = (result << 1) + (size_t)(wires[wire] & 0x01);
	}

	return result;
}

void set_bus(network_t &wires, const std::string &prefix, size_t value) {
	std::vector<std::string> output_wires = find_wires(wires, prefix);
	std::sort(output_wires.begin(), output_wires.end(), std::less<std::string>());

	for (auto &wire : output_wires) {
		wires[wire] = value & 0x01;
		value = value >> 1;
	}
}

void bus_print(network_t &wires, const std::string &prefix = "z") {
	std::vector<std::string> output_wires = find_wires(wires, prefix);
	std::sort(output_wires.begin(), output_wires.end(), std::greater<std::string>());

	for (auto &wire : output_wires) {
		if (wires[wire] >= 0) {
			std::cout << (wires[wire] & 0x01);
		} else {
			std::cout << ".";
		}
	}
}

int node_eval(const node_t &node, network_t &wires) {
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


size_t simulate_bit_vec(network_t wires, std::vector<node_t> nodes) {
	std::bitset<64> z_ready;
	std::vector<std::string> z_wires = find_wires(wires, "z");
	for (size_t i = 0; i < z_wires.size(); i++) {
		z_ready.set(i);
	}

	while (z_ready != 0) {
		for (auto &node : nodes) {
			auto value = node_eval(node, wires);
			wires[node.out] = value;

			if (node.out.find("z", 0) == 0 && value >= 0) {
				int z_bit = atoi(node.out.substr(1).c_str());
				if (value < 0) {
					z_ready.set((size_t)z_bit, true);
				} else {
					z_ready.set((size_t)z_bit, false);
				}
			}
		}
	}

	return bus_value(wires);
}

size_t simulate_1(network_t wires, std::vector<node_t> nodes) {
	while (!bus_ready(wires)) {

		for (auto &node : nodes) {
			auto value = node_eval(node, wires);
			wires[node.out] = value;
		}
	}

	return bus_value(wires);
}

size_t simulate(network_t wires, std::vector<node_t> nodes) {
	// int i = 0;
	while (!bus_ready(wires, "z")) {
		// std::cout << std::setw(2) << i++ << ": ";

		for (auto &node : nodes) {
			wires[node.out] = node_eval(node, wires);
		}

		// bus_print(wires, "z");
		// std::cout << std::endl;
	}

	return bus_value(wires, "z");
}



// 0 not the right answer
// 59336987801432 correct answer, nodes affect wires immediately
long part1(const data_collection_t data) {
	long solution = 0;

	auto [wires, nodes] = data;

	// for (auto wire : wires) {
	// 	std::cout << wire.first << " = " << wire.second << std::endl;
	// }
	// std::cout << std::endl;

	// for (auto node : nodes) {
	// 	print_node(node);
	// 	std::cout << std::endl;
	// }
	// 2.2ms

	solution = (long)simulate_1(wires, nodes);
	return solution;
}


long part2(const data_collection_t data) {
	long solution = 0;

	auto [wires, nodes] = data;

	for (size_t x = 0; x < 10; x++) {
		for (size_t y = 0; y < 48; y++) {
			size_t fy = 1 << y;

			set_bus(wires, "x", x);
			set_bus(wires, "y", fy);
			auto z = simulate(wires, nodes);

			// bus_print(wires, "x");
			// std::cout << " + ";
			// bus_print(wires, "y");
			// std::cout << " = ";
			// bus_print(wires, "z");

			std::cout << x << " + " << fy << " = " << z;

			if (x + fy != z) {
				std::cout << " FAIL" << std::endl;
			} else {
				std::cout << " OK" << std::endl;
			}
		}
	}

	return solution;
}

bool contains(const std::map<std::string, int> &haystack, const std::string &needle) {
	return haystack.find(needle) != haystack.end();
}

// void add_wire(std::map<std::string, int> wires, const std::string &name, int value) {
// 	wires[name] = value;
// }

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


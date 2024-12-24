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

/* simulate the circuit */
size_t simulate_1(network_t wires, std::vector<node_t> nodes) {
	while (!bus_ready(wires)) {

		for (auto &node : nodes) {
			auto value = node_eval(node, wires);
			wires[node.out] = value;
		}
	}

	return bus_value(wires);
}

/* simulate the circuit */
size_t simulate(network_t wires, std::vector<node_t> nodes) {
	while (!bus_ready(wires, "z")) {
		for (auto &node : nodes) {
			wires[node.out] = node_eval(node, wires);
		}
	}

	return bus_value(wires, "z");
}

/* find the node with wire as an output */
int find_output(std::vector<node_t> &nodes, std::string wire) {
	for (size_t n = 0; n < nodes.size(); n++) {
		if (nodes[n].out == wire) {
			return (int)n;
		}
	}

	return -1;
}

/* generate a unique name for a node based on it's index, used for make_dot() */
std::string node_name(std::vector<node_t> &nodes, int node_n) {
	std::string op;
	switch (nodes[(size_t)node_n].op) {
		case AND:
			op = "AND_";
			break;
		case XOR:
			op = "XOR_";
			break;
		case OR:
			op = "OR_";
			break;
	}

	return op + std::to_string(node_n);
}

/* Makes a .dot file format of the nodes and edges for examination
 * ./solution > file.dot
 * // edit file.dot to remove any extra characters
 * dot -Tpdf file.dot > file.pdf
*/
void make_dot([[maybe_unused]]network_t wires, std::vector<node_t> nodes) {
	std::cout << "digraph network {" << std::endl;

		// out -> in
		for (size_t n = 0; n < nodes.size(); n++) {
			auto node = nodes[n];
			
			int in_a = find_output(nodes, node.a);
			if (in_a >= 0) {
				std::cout << node_name(nodes, in_a) << " -> " << node_name(nodes, (int)n) 
						  << " [label=\"" << node.a << "\"]" << std::endl;
			} else {
				std::cout << node.a << " -> " << node_name(nodes, (int)n)
						  << " [label=\"" << node.a << "\"]" << std::endl;
			}

			int in_b = find_output(nodes, node.b);
			if (in_b >= 0) {
				std::cout << node_name(nodes, in_b) << " -> " << node_name(nodes, (int)n)
						  << " [label=\"" << node.b << "\"]" << std::endl;
			} else {
				std::cout << node.b << " -> " << node_name(nodes, (int)n)
						  << " [label=\"" << node.b << "\"]" << std::endl;
			}

			if (node.out.find("z", 0) == 0) {
				std::cout << node_name(nodes, (int)n) << " -> " << node.out
						  << " [label=\"" << node.out << "\"]" << std::endl;
			}
		}

	std::cout << "}" << std::endl;
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

	solution = (long)simulate(wires, nodes);
	return solution;
}

void swap_wires(std::vector<node_t> &nodes, std::string w1, std::string w2) {
	for (auto &node : nodes) {
		if (node.out == w1) {
			// std::cout << "// swap out " << node.out << " => " << w2 << std::endl;
			node.out = w2;
		} else if (node.out == w2) {
			// std::cout << "// swap out " << node.out << " => " << w1 << std::endl;
			node.out = w1;
		}
	}
}

/* *** by looking at a lot of diagrams!
 * Simulate and find where the first bit failure occurs,
 * look at .dot file (converted to PDF) generated with make_dot()
 * Piece togethere what wires are crossed by comparing to previous
 * adder circuits, come up with guess as to wires to change.
 * add swap_wires() call to swap wires in the graph.
 * loop for 4 times.
*/
long part2(const data_collection_t data) {
	auto [wires, nodes] = data;

	// don't ron on sample data.
	if (wires.size() < 100) {
		std::cout << "Don't run on sample data." << std::endl;
		return 0;
	}

	// sorted: ctg,dmh,dvq,rpb,rpv,z11,z31,z38
	swap_wires(nodes, "z11", "rpv");	// bit 11
	swap_wires(nodes, "ctg", "rpb");	// bit 15
	swap_wires(nodes, "z31", "dmh");	// bit 31
	swap_wires(nodes, "z38", "dvq");	// bit 38
	// make_dot(wires, nodes);

	auto z_wires = find_wires(wires, "z");
	size_t max_bit = z_wires.size() - 1;

	size_t x = 10;	// a random value
	for (size_t y = 0; y < max_bit; y++) {
		size_t fy = 1ul << y;

		set_bus(wires, "x", x);
		set_bus(wires, "y", fy);
		auto z = simulate(wires, nodes);

		if (x + fy != z) {
			std::cout << " FAIL at bit " << y << ", "
					  << x << " + " << fy << " should be " << x + fy
					  << std::endl;
			std::cout << "x:" << std::bitset<64>(x) << " " << x << "\n"
					  << "y:"<< std::bitset<64>(fy) << " " << fy << "\n"
					  << "z:"<< std::bitset<64>(z) << " " << z << std::endl;
		}
	}

	return (long)string_hash("ctg,dmh,dvq,rpb,rpv,z11,z31,z38");
}

bool contains(const std::map<std::string, int> &haystack, const std::string &needle) {
	return haystack.find(needle) != haystack.end();
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


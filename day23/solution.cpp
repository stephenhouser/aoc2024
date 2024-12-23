#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <string_view>
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <unordered_map>
#include <unordered_set>
#include <set>

#include "aoc2024.h"
#include "solution.h"

void p_set(std::set<std::string> &s) {
	for (auto &p : s) {
		std::cout << p << ",";
	}
}

bool starts_with(const std::string &s, const std::string &prefix) {
	return s.find(prefix) == 0;
}

std::string lansert(const std::vector<std::string> &nodes) {
	std::string clique_name;
	std::vector<std::string> sorted_nodes{nodes};

	std::sort(sorted_nodes.begin(), sorted_nodes.end());
	for (auto n : sorted_nodes) {
		clique_name.append(n);
	}

	return clique_name;
}

std::string lansert(const std::string &n1, const std::string &n2, const std::string &n3) {
	return lansert({n1, n2, n3});
}


// 234 too low
// 1238 just right, dummy on the min and max calcs
long part1(const data_collection_t data) {
	long solution = 0;

	std::set<std::string> t_nodes;
	std::unordered_map<std::string, std::set<std::string>> nodes;
	for (auto edge : data) {
		// std::cout << "Edge: " << edge.first << "->" << edge.second;
		// std::cout << "  " << edge.second << "->" << edge.first << std::endl;
		nodes[edge.first].insert(edge.second);
		nodes[edge.second].insert(edge.first);

		if (starts_with(edge.first, "t")) {
			t_nodes.insert(edge.first);
		}
	}

	// for (auto node : nodes) {
	// 	std::cout << node.first << ":";
	// 	for (auto n : node.second) {
	// 		std::cout << n << ",";
	// 	}
	// 	std::cout << std::endl;
	// }

	std::unordered_set<std::string> triples;
	for (auto n1 : t_nodes) {
		auto n1_edges = nodes[n1];

		// all n2 are in n1
		for (auto n2 : n1_edges) {
			auto n2_edges = nodes[n2];

			// all n3 are in n2
			for (auto n3 : n2_edges) {
				auto n3_edges = nodes[n3];
				if (n3_edges.find(n1) != n3_edges.end()) {
					// this is a 3-connected set, add it to our list of triples
					triples.insert(lansert(n1, n2, n3));
				}
			}
		}
	}

	for (auto t : triples) {
		// std::cout << t << std::endl;
		solution++;
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
	data_collection_t data;

	for (auto line : read_lines(path)) {
		auto nodes = split(line, "-");

		data_t edge = {nodes[0], nodes[1]};
		data.push_back(edge);
	}

	return data;
}


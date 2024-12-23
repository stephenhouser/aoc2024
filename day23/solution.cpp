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

bool any_starts_with(const std::vector<std::string> &s, const std::string &prefix) {
	for (auto n : s) {
		if (n.find(prefix) == 0) {
			return true;
		}
	}
	return false;
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

					// insert them in a sorted order so set will filter for us.
					// std::cout << n1 << "," << n2 << "," << n3 << std::endl;
					auto max = std::max(n1, std::max(n2, n3));
					auto min = std::min(n1, std::min(n2, n3));

					std::string clique;
					if (n1 != max && n1 != min) {
						clique.append(min+n1+max);
						// std::cout << "\t" << min << "," << n1 << "," << max << std::endl;
					} else if (n2 != min && n2 != max) {
						clique.append(min+n2+max);
						// std::cout << "\t" <<  min << "," << n2 << "," << max << std::endl;
					} else {
						clique.append(min+n3+max);
						// std::cout << "\t" <<  min << "," << n3 << "," << max << std::endl;
					}

					triples.insert(clique);
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


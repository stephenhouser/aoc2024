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

	for (auto n1 : nodes) {
		std::set<std::string> n1_edges{n1.second};
		n1_edges.insert(n1.first);

		for (auto n2 : nodes) {
			if (n1 != n2) {
				std::set<std::string> n2_edges{n2.second};
				n2_edges.insert(n2.first);

				std::vector<std::string> n1n2_edges;
			    std::set_intersection(n1_edges.begin(), n1_edges.end(),
									  n2_edges.begin(), n2_edges.end(),
            			              std::back_inserter(n1n2_edges));

				for (auto n3_name : n1n2_edges) {
					if (n3_name != n1.first && n3_name != n2.first) {
						auto n3{nodes[n3_name]};

						std::set<std::string> n3_edges{n3};
						n3_edges.insert(n3_name);

						std::vector<std::string> n1n2n3_edges;
						std::set_intersection(n1n2_edges.begin(), n1n2_edges.end(),
											n3_edges.begin(), n3_edges.end(),
												std::back_inserter(n1n2n3_edges));

						if (n1n2n3_edges.size() > 3) {
							std::cout << n1.first << ":(";
							p_set(n1_edges);
							
							std::cout << ")\t" << n2.first << ":(";
							p_set(n2_edges);

							std::cout << ")\t" << n3_name << ":(";
							p_set(n3_edges);


							std::cout << ")\t(";
							for (auto n : n1n2n3_edges) {
								std::cout << n << ",";
							}

							std::cout << ")\t";

							// if (any_starts_with(n1n2n3_edges, "t")) {
							// 	std::cout << " *** ***";
							// 	solution++;
							// }

							std::cout << std::endl;
						}
					}
				}
			}
		}

	}

	// solution /= 3;

	// std::unordered_set<std::string> seen;
	// for (auto node : nodes) {
	// 	if (seen.find(node.first) == seen.end()) {
	// 		auto edges = node.second;
	// 		std::cout << "evaluate " << node.first << "->" << edges.size() << std::endl;
	// 		// if (edges.size() == 2) {
	// 			std::cout << "\ttriple:" << node.first << ",";
	// 			for (auto n : node.second) {
	// 				std::cout << n << ",";
	// 			}
	// 			std::cout << std::endl;
	// 		// }
	// 	}
	// }


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


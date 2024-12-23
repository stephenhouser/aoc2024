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
#include <algorithm>
#include <cassert>

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
		clique_name.append(",");
	}

	clique_name.pop_back();
	return clique_name;
}


// std::string lansert(const std::set<std::string> &nodes) {
// 	std::string clique_name;
// 	std::vector<std::string> sorted_nodes;

// 	std::copy(nodes.begin(), nodes.end(), std::back_inserter(sorted_nodes));

// 	std::sort(sorted_nodes.begin(), sorted_nodes.end());
// 	for (auto n : sorted_nodes) {
// 		clique_name.append(n);
// 	}

// 	return clique_name;
// }

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

using node_t = size_t;
using graph_t = std::unordered_map<node_t, std::unordered_set<node_t>>;

node_t hash(std::string &node) {
	assert(node.length() == 2);
	return (((node[0] & 0xFF) << 8) | (node[1] & 0xFF));
}

std::string unhash(node_t node_hash) {
	std::string node;
	node.push_back((char)((node_hash >> 8) & 0xFF));
	node.push_back((char)(node_hash & 0xFF));
	return node;
}

std::string lansert(const std::vector<node_t> &nodes) {
	std::string clique_name;
	std::vector<std::string> sorted_nodes;

	for (auto node : nodes) {
		sorted_nodes.push_back(unhash(node));
	}

	std::sort(sorted_nodes.begin(), sorted_nodes.end());
	for (auto n : sorted_nodes) {
		clique_name.append(n);
		clique_name.append(",");
	}

	clique_name.pop_back();
	return clique_name;
}
/*

The recursion is initiated by setting R and X to be the empty set and P to be the vertex set of the graph. Within each recursive call, the algorithm considers the vertices in P in turn; if there are no such vertices, it either reports R as a maximal clique (if X is empty), or backtracks. For each vertex v chosen from P, it makes a recursive call in which v is added to R and in which P and X are restricted to the neighbor set N(v) of v, which finds and reports all clique extensions of R that contain v. Then, it moves v from P to X to exclude it from consideration in future cliques and continues with the next vertex in P.

algorithm BronKerbosch1(R, P, X) is
    if P and X are both empty then
        report R as a maximal clique
    for each vertex v in P do
        BronKerbosch1(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
        P := P \ {v}
        X := X ⋃ {v}
*/
std::vector<node_t> largest_clique;

std::vector<node_t> keys(graph_t &graph) {
	std::vector<node_t> collect;
	for (auto n : graph) {
		collect.push_back(n.first);
	}
	return collect;
}

void bron_kerbosch(graph_t &R, graph_t &P, graph_t &X) {
	if (P.empty() && X.empty()) {
		// we found a clique, is it larger than the largest?
		auto clique = keys(R);
		if (largest_clique.size() < clique.size()) {
			largest_clique = clique;
		}

		return;
	}

	// for (auto p : P) {
	// 	auto v = p.first;
	// 	auto v_edges = p.second;

	while (!P.empty()) {
		auto nit = P.begin();
		auto v = (*nit).first;
		auto v_edges = (*nit).second;
		P.erase(nit);

		// R union {v}
		auto Rn{R};
		Rn[v] = v_edges;

		// P intersection Neighbors of(v)
		graph_t Pn;
		for (auto n : v_edges) {
			if (P.find(n) != P.end()) {
				Pn[n] = P[n];
			}
		}

		// X intersection Neighbors of (v)
		graph_t Xn;
		for (auto n : v_edges) {
			if (X.find(n) != X.end()) {
				Xn[n] = X[n];
			}
		}

		bron_kerbosch(Rn, Pn, Xn);

		// P := P \ {v} -- remove v
		auto pit2 = P.find(v);
		if (pit2 != P.end())
			P.erase(pit2);

        // X := X ⋃ {v}
		X[v] = v_edges;
	}
}

// pivot version not any faster
void bron_kerbosch_pivot(graph_t &R, graph_t &P, graph_t &X) {
	if (P.empty() && X.empty()) {
		// we found a clique, is it larger than the largest?
		auto clique = keys(R);
		if (largest_clique.size() < clique.size()) {
			largest_clique = clique;
		}

		return;
	}


	// choose a pivot vertex u in P ⋃ X
	std::unordered_set<node_t> u_neighbors;
	for (auto p : P) {
		if (X.find(p.first) != X.end()) {
			u_neighbors = p.second;
			break;
		}
	}

	graph_t Pu{P};
    // for each vertex v in P \ N(u) do
	for (auto n_n : u_neighbors) {
		auto pit = Pu.find(n_n);
		if (pit != Pu.end()) {
			Pu.erase(pit);
		}
	}
	
	for (auto p : Pu) {
		auto v = p.first;
		auto v_edges = p.second;

	// while (!P.empty()) {
	// 	auto nit = P.begin();
	// 	auto v = (*nit).first;
	// 	auto v_edges = (*nit).second;
	// 	P.erase(nit);

	// for each vertex v in P do
	// for (auto p : P) {
	// 	auto v = p.first;
	// 	auto v_edges = p.second;

		// R union {v}
		auto Rn{R};
		Rn[v] = v_edges;

		// P intersection Neighbors of(v)
		graph_t Pn;
		for (auto n : v_edges) {
			if (P.find(n) != P.end()) {
				Pn[n] = P[n];
			}
		}

		// X intersection Neighbors of (v)
		graph_t Xn;
		for (auto n : v_edges) {
			if (X.find(n) != X.end()) {
				Xn[n] = X[n];
			}
		}

		bron_kerbosch_pivot(Rn, Pn, Xn);

		// P := P \ {v} -- remove v
		auto pit2 = P.find(v);
		if (pit2 != P.end())
			P.erase(pit2);

        // X := X ⋃ {v}
		X[v] = v_edges;
	}
}

size_t number_string(std::string &str) {
	size_t n = 0;
	for (auto c : str) {
		if (c >= 'a' && c <= 'z') {
			n = n << 5 | (size_t)((c - 'a') & 0x1F);
		}
	}
	return n;
}


// adapted from https://cp-algorithms.com/string/string-hashing.html
size_t string_hash(const std::string &s) {
	const size_t p = 31;
	const size_t m = 1e9 + 9;
	size_t hash_value = 0;
	size_t p_pow = 1;

	for (size_t i = 0; i < s.length(); i++) {
		hash_value = (hash_value + ((size_t)s[i] - (size_t)'a' + (size_t)1) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}

	return hash_value;
}

// first try
// bg,bl,ch,fn,fv,gd,jn,kk,lk,pv,rr,tb,vw
long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;

	std::set<std::string> t_nodes;
	graph_t graph;
	for (auto edge : data) {
		// std::cout << "Edge: " << edge.first << "->" << edge.second;
		// std::cout << "  " << edge.second << "->" << edge.first << std::endl;
		auto n1 = hash(edge.first);
		auto n2 = hash(edge.second);

		graph[n1].insert(n2);
		graph[n2].insert(n1);
	}

	graph_t R;
	graph_t X;
	bron_kerbosch(R, graph, X);

	std::string passcode = lansert(largest_clique);
	if (verbose > 1) {
		std::cout << "found size " << largest_clique.size() << std::endl;
		std::cout << passcode << std::endl;
	}

	solution = (long)string_hash(passcode);
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


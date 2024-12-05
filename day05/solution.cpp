#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <map>
#include <set>

#include "aoc2024.h"
#include "solution.h"

void print_vec(const std::vector<int> &vec) {
	for (auto n : vec) {
		if (!(vec.front() == n)) {
			std::cout << ",";
		}
		std::cout << n;
	}
	std::cout << std::endl;
}


bool set_contains(const std::set<int> &s, int n) {
	return s.find(n) != s.end();
}

bool vec_contains(const std::vector<int> &vec, int n) {
	if (false) {
		for (auto x : vec) {
			if (x == n) {
				return true;
			}
		}

		return false;
	} else {
		for (unsigned long i = 0; i < vec.size(); i++) {
			if (vec[i] == n) {
				return true;
			}
		}
		return false;
	}
}

long vec_middle(std::vector<int> const &vec) {
	return *(vec.begin() + ((int)vec.size() / 2));
}



bool appears_before(int n, size_t loc, const std::vector<int> &vec) {
	for (auto [i, x] : enumerate(vec)) {
		if (i < loc && n == x) {
			return true;
		}

	}

	return false;
}

bool appears_after(int n, size_t loc, const std::vector<int> &vec) {
	for (auto [i, x] : enumerate(vec)) {
		if (i >= loc && n == x) {
			return true;
		}
	}

	return false;
}


bool check_rule(int page, const std::vector<int> &prev, const std::vector<int> &next, rules_t &rules) {
	const auto [before_rules, after_rules] = rules[page];

	for (auto p : after_rules) {
		if (vec_contains(next, p)) {
			// std::cout << "previous contains " << p << " ";
			return false;
		}
	}

	for (auto p : before_rules) {
		if (vec_contains(prev, p)) {
			// std::cout << "next contains " << p << " ";
			return false;
		}
	}

	return true;
}

bool check_sequence(const std::vector<int> &sequence,  rules_t &rules) {
	// std::cout << "\ncheck_sequence()";
	// print_vec(sequence);
	if (true) {
		std::vector<int> after = sequence;
		std::vector<int> before;

		for (auto page : sequence) {
			after.erase(after.begin());	// pop
			if (!check_rule(page, before, after, rules)) {
				return false;
			}

			before.push_back(page);
		}
	} else if (true) {
		for (auto [i, n] : enumerate(sequence)) {
			// std::cout << "\tconsider " << n << " @" << i;

			const auto [prerequsite, postrequisite] = rules[n];
			//check that there is nothing in our after rules
			for (auto p : postrequisite) {
				if (appears_after(p, i, sequence)) {
					// std::cout << " FAIL " << p << " appears after " << n << "\n";
					return false;
				}
			}

			for (auto p : prerequsite) {
				if (appears_before(p, i, sequence)) {
					// std::cout << " FAIL " << p << " appears before " << n << "\n";
					return false;
				}
			}
		}
		// std::cout << " OK\n";
	} else {
		for (auto [i, n] : enumerate(sequence)) {
			std::cout << "\tconsider " << n << " @" << i;

			const auto [prerequsite, postrequisite] = rules[n];

			for (auto p : postrequisite) {
				if (appears_after(p, i, sequence)) {
					// std::cout << " FAIL " << p << " appears after " << n << "\n";
					return false;
				}
			}

			// for (auto p : prerequsite) {
			// 	if (appears_before(p, i, sequence)) {
			// 		// std::cout << " FAIL " << p << " appears before " << n << "\n";
			// 		return false;
			// 	}
			// }
		}
		// std::cout << " OK\n";

	}
	return true;
}

long part1(const data_collection_t data) {
	long solution = 0;
	auto [rules, updates] = data;

	for (auto sequence : updates) {
		if (check_sequence(sequence, rules)) {
			solution += vec_middle(sequence);
		}
	}
	
	return solution;
}

// returns true if page can go at location
bool check_placement(int page, int location, const std::vector<int> &pages, rules_t &rules) {
	std::vector<int> before;
	std::copy(pages.begin(), pages.begin()+location, std::back_inserter(before));

	std::vector<int> after;
	std::copy(pages.begin()+location, pages.end(), std::back_inserter(after));

	return check_rule(page, before, after, rules);
}

std::vector<int> reorder_sequence(const std::vector<int> &sequence, rules_t &rules) {
	std::vector<int> fixed;

	for (auto page : sequence) {
		if (fixed.empty()) {
			fixed.insert(fixed.begin(), page);
		} else {
			int i = 0;
			for (; i < (int)fixed.size(); i++) {
				if (check_placement(page, i, fixed, rules)) {
					fixed.insert(fixed.begin()+i, page);
					break;					
				}
			}
			if (i == (int)fixed.size()) {
				fixed.insert(fixed.end(), page);
			}
			
		}
	}

	return fixed;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;
	auto [rules, updates] = data;

	for (auto sequence : updates) {
		if (!check_sequence(sequence, rules)) {
			auto fixed = reorder_sequence(sequence, rules);
			solution += vec_middle(fixed);
		}		
	}
	
	return solution;
}

void print_rules(rules_t &rules) {
	for (auto p : rules) {
		auto page_n = p.first;
		auto [before_rules, after_rules] = p.second;

		std::cout << page_n << "\tbef=";
		for (auto b : before_rules) {
			std::cout << b << ",";
		}

		std::cout << "\taft=";
		for (auto a : after_rules) {
			std::cout << a << ",";
		}

		std::cout << std::endl;
	}
}

void add_rules(int n_before, int n_after, rules_t &rules) {
	// ad rule that n_before must come before n_after
	std::set<int> &before_rules = std::get<0>(rules[n_before]);
	before_rules.insert(n_after);
	// before_rules.first.insert(n_after);
	
	// add rule that n_after must come after n_before
	std::set<int> &after_rules = std::get<1>(rules[n_after]);
	after_rules.insert(n_before);

}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	bool reading_rules = true;
	rules_t rules;
	updates_t updates;

	for (const auto &line : read_lines(path)) {
		if (line.empty()) {

			reading_rules = false;

		} else if (reading_rules) {

			auto parts = split(line, "|");
			int n_before = atoi(parts[0].c_str());
			int n_after = atoi(parts[1].c_str());

			add_rules(n_before, n_after, rules);

		} else {

			auto parts = split(line, ",");

			std::vector<int> parts_i;
			for (auto p : parts) {
				parts_i.push_back(atoi(p.c_str()));
			}
			updates.push_back(parts_i);

		}
	}

	// print_rules(rules);
	return {rules, updates};
}

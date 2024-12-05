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


// check_rule(page, prev, next, rules) {
// 	after_rules = rules[page]->after	// page comes after these
// 	before_rules = rules[page]->before	// page mist be before these

// 	// if set intersection is empty
// 	// after_rules and list of things we already saw
// 	for (p : after_rules) {
// 		if p in prev:
// 			return false;
// 	}

// 	for (p : before_rules) {
// 		if p in next
// 			return false;
// 	}

// 	return true
// }

bool vec_contains(const std::vector<int> &vec, int n) {
	for (auto x : vec) {
		if (x == n) {
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


bool check_update(const std::vector<int> pages,  rules_t &rules) {
	std::vector<int> after = pages;
	std::vector<int> before;

	for (auto page : pages) {
		after.erase(after.begin());	// pop
		if (!check_rule(page, before, after, rules)) {
			return false;
		}

		before.push_back(page);
	}

	return true;
}

long find_middle(std::vector<int> const &pages) {
	auto mid = pages.begin() + ((int)pages.size() / 2);
	return (long)*mid;
}

long part1(const data_collection_t data) {
	long solution = 0;
	auto [rules, updates] = data;

	for (auto update : updates) {
		if (check_update(update, rules)) {
			solution += find_middle(update);
		}
	}
	
	return solution;
}

// returns true if page can go at location
bool check_placement(int page, int location, const std::vector<int> &pages, rules_t &rules) {
	std::vector<int> before;
	std::copy(pages.begin(), pages.begin()+location, std::back_inserter(before));
	std::cout << "\tBEFORE ";
	print_vec(before);

	std::vector<int> after;
	std::copy(pages.begin()+location, pages.end(), std::back_inserter(after));
	std::cout << "\tAFTER ";
	print_vec(after);

	return check_rule(page, before, after, rules);
}

std::vector<int> reorder_update(const std::vector<int> &pages, rules_t &rules) {
	std::vector<int> fixed;

	for (auto page : pages) {
		std::cout << "\tPLACE " << page << "\n";
		if (fixed.empty()) {
			fixed.insert(fixed.begin(), page);
		} else {
			int i = 0;
			for (; i < (int)fixed.size(); i++) {
				std::cout << "." << i;
				if (check_placement(page, i, fixed, rules)) {
					std::cout << "@i" << i;
					fixed.insert(fixed.begin()+i, page);
					break;					
				}
			}
			if (i == (int)fixed.size()) {
				std::cout << "@END";
				fixed.insert(fixed.end(), page);
			}
			
		}

		std::cout << "\n";
		print_vec(fixed);
	}

	return fixed;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;
	auto [rules, updates] = data;

	for (auto update : updates) {
		if (!check_update(update, rules)) {
			std::cout << "\n\n";
			auto fixed = reorder_update(update, rules);
			solution += find_middle(fixed);
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

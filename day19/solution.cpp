#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <string_view>
#include <cstdint>

#include "aoc2024.h"
#include "solution.h"

// 374 
long part1([[maybe_unused]]const data_collection_t data) {
	long solution = 0;

	auto [towels, arrangements] = data;
	// std::cout << "towels =" << towels << std::endl;
	// std::cout << "arrangements=" << arrangements.size() 
	// 		  << " " << arrangements[0]
	// 			<< std::endl;

	// "[ | ]+"
	std::ostringstream buffer;
	buffer << "^(";
	for (auto &t : towels) {
        buffer << t;
		if (&t != &towels.back()) {
			buffer << "|";
		}
	}
	buffer << ")+$";
	// std::cout << buffer.str() << std::endl;

	std::regex reggie(buffer.str());

	for (auto a : arrangements) {
		std::smatch match;
		if (std::regex_match(a, match, reggie)) {
			// std::cout << a << " matches" << std::endl;
			solution++;
		}
	}

	return solution;
}

std::map<std::string, uint64_t> seen;
uint64_t can_arrange(std::vector<std::string> &towels, std::string &arrangement) {
	uint64_t count = 0;

	if (arrangement.size() == 0) {
		return 1;
	}

	if (seen.find(arrangement) != seen.end()) {
		return seen[arrangement];
	}

	for (std::string &t : towels) {
		if (arrangement.find(t, 0) == 0) {
			std::string ns = arrangement.substr(t.length());
			count += can_arrange(towels, ns);
		}
	}

	seen[arrangement] = count;
	return count;
}

// 35631290362 low
// 1100663950563322 correct
// use unsigned int!!
long part2([[maybe_unused]] const data_collection_t data) {
	uint64_t solution = 0;

	auto [towels, arrangements] = data;

	for (auto a : arrangements) {
		auto can = can_arrange(towels, a);
		solution += can;
		std::cout << can << "\t" << a << std::endl;
	}

	return (long)solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	auto lines = read_lines(path);
	auto towels = split(lines[0], ", ");
	std::vector<std::string> arrangements;
	std::copy(lines.begin()+2, lines.end(), std::back_inserter(arrangements));
	return {towels, arrangements};
}


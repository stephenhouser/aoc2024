#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <string_view>
#include <cstdint>
#include <cstring>

#include "aoc2024.h"
#include "solution.h"

// std::map<std::string, uint64_t> seen;
// uint64_t can_arrange_s(std::vector<std::string> &towels, std::string &arrangement) {
// 	uint64_t count = 0;

// 	if (arrangement.size() == 0) {
// 		return 1;
// 	}

// 	if (seen.find(arrangement) != seen.end()) {
// 		return seen[arrangement];
// 	}

// 	for (std::string &t : towels) {
// 		if (arrangement.find(t, 0) == 0) {
// 			std::string ns = arrangement.substr(t.length());
// 			count += can_arrange(towels, ns);
// 		}
// 	}

// 	seen[arrangement] = count;
// 	return count;
// }

uint64_t string_hash(const char *s) {
    const uint64_t p = 31;
    const uint64_t m = 1e9 + 9;
    uint64_t hash_value = 0;
    uint64_t p_pow = 1;
	for (size_t i = 0; i < strlen(s); i++) {
        hash_value = (hash_value + ((uint64_t)s[i] - (uint64_t)'a' + (uint64_t)1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

std::map<uint64_t, uint64_t> seen;
uint64_t can_arrange(std::vector<std::string> &towels, const char *arrangement) {
	uint64_t count = 0;

	if (strlen(arrangement) == 0) {
		return 1;
	}

	uint64_t h = string_hash(arrangement);
	if (seen.find(h) != seen.end()) {
		return seen[h];
	}

	for (std::string &t : towels) {
		size_t t_len = strlen(t.c_str());
		if (strncmp(t.c_str(), arrangement, t_len) == 0) {
			count += can_arrange(towels, arrangement + t_len);
		}
	}

	seen[h] = count;
	return count;
}


// 374 
long part1(const data_collection_t data) {
	uint64_t solution = 0;

	auto [towels, arrangements] = data;
	for (auto &a : arrangements) {
		if (can_arrange(towels, a.c_str())) {
			solution++;
		}
	}

	return (long)solution;
}


// 35631290362 low
// 1100663950563322 correct
// use unsigned int!!
long part2([[maybe_unused]] const data_collection_t data) {
	uint64_t solution = 0;

	auto [towels, arrangements] = data;

	for (auto a : arrangements) {
		auto can = can_arrange(towels, a.c_str());
		solution += can;
		// std::cout << can << "\t" << a << std::endl;
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


long part1_slow([[maybe_unused]]const data_collection_t data) {
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

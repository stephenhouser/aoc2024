#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <string>    
#include <ranges>
#include <string_view>
#include <getopt.h>
#include <charconv>
#include <tuple>
#include <cstring>
#include <algorithm>
#include <map>
#include <array>
#include <chrono>
#include <regex>

using data_t = std::string;
std::vector<data_t> read_data(const std::string &path);

#include "aoc2024.h"

/* Output verbosity level; 0 = nothing extra, 1 = more... Set by command line. */
int verbose = 0;

/* Read data from path and return a vector for each line in the file. */
std::vector<data_t> read_data(const std::string &path) {
    auto lines = read_lines(path);

    return lines;
}


int part1(const std::vector<data_t> data) {
    int accumulator = 0 ;

	for (auto line : data) {
		std::regex mul_regex("mul\\((\\d+),(\\d+)\\)");
		auto m_beg = std::sregex_iterator(line.begin(), line.end(), mul_regex);
		auto m_end = std::sregex_iterator();
		for (std::regex_iterator i = m_beg; i != m_end; ++i) {
			std::smatch match = *i;
			std::string instruction = match.str();

			int l = atoi(match[1].str().c_str());
			int r = atoi(match[2].str().c_str());

			accumulator += l * r;
			// std::cout << instruction << "\n";
		}
	}

    return accumulator;
}

int part2([[maybe_unused]] const std::vector<data_t> data) {
    int accumulator = 0 ;

    return accumulator;
;
 }


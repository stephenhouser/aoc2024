#include <getopt.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <ranges>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

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
	int accumulator = 0;

	for (auto line : data) {
		std::regex mul_regex("mul\\((\\d+),(\\d+)\\)");
		auto m_beg = std::sregex_iterator(line.begin(), line.end(), mul_regex);
		auto m_end = std::sregex_iterator();
		for (std::regex_iterator i = m_beg; i != m_end; ++i) {
			std::smatch match = *i;

			int l = atoi(match[1].str().c_str());
			int r = atoi(match[2].str().c_str());

			accumulator += l * r;
		}
	}

	return accumulator;
}

int part2([[maybe_unused]] const std::vector<data_t> data) {
	int accumulator = 0;

	bool enabled = true;
	for (auto line : data) {
		std::regex mul_regex(
			"mul\\((\\d+),(\\d+)\\)|do\\(\\)|don't\\(\\)");
		auto m_beg = std::sregex_iterator(line.begin(), line.end(), mul_regex);
		auto m_end = std::sregex_iterator();
		for (std::regex_iterator i = m_beg; i != m_end; ++i) {
			std::smatch match = *i;
			std::string instruction = match.str();

			if (instruction.find("mul(", 0) == 0 && enabled) {
				int l = atoi(match[1].str().c_str());
				int r = atoi(match[2].str().c_str());
				accumulator += l * r;
			}

			if (instruction.find("do(", 0) == 0) {
				enabled = true;
			}

			if (instruction.find("don't(", 0) == 0) {
				enabled = false;
			}
		}
	}

	return accumulator;
}

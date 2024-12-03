
#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions

#include "aoc2024.h"
#include "computer.h"

/* Read data from path and return a vector for each line in the file. */
std::vector<data_t> read_data(const std::string &path) {
	auto lines = read_lines(path);

	return lines;
}

long part1(const std::vector<data_t> data) {
	long accumulator = 0;

	for (auto line : data) {
		std::regex mul_regex("mul\\((\\d+),(\\d+)\\)");
		auto m_beg = std::sregex_iterator(line.begin(), line.end(), mul_regex);
		auto m_end = std::sregex_iterator();
		for (std::regex_iterator i = m_beg; i != m_end; ++i) {
			std::smatch match = *i;

			int l = stoi(match[1].str());
			int r = stoi(match[2].str());

			accumulator += l * r;
		}
	}

	return accumulator;
}

long part2([[maybe_unused]] const std::vector<data_t> data) {
	long accumulator = 0;

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
				int l = stoi(match[1].str());
				int r = stoi(match[2].str());
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

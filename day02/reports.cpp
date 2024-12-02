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

#include "aoc2024.h"

const int safe_level_change = 3;

// verbosity level, 0 = nothing extra, 1 = more...
int verbose = 0;

std::vector<std::vector<long>> read_data(const std::string &path) {
    auto lines { read_lines(path) };
    std::vector<std::vector<long>> reports;

    for (const auto& line : lines) {
	    std::vector<long> levels;

        for (auto level : split(line, " ")) {
			levels.push_back(atol(level.c_str()));
		}

		reports.push_back(levels);
	}

    return reports;
}

bool is_increasing(const std::vector<long> &report) {
	auto first = report.front();
	auto it = report.begin();
	while(it != report.end() && *it == first) {
    	it++;
	}

	return first < *it;
}

bool safe_change(long last_level, long level, bool increasing) {
	auto difference = level - last_level;
	if (!difference) {
		return false;
	}

	if (increasing && (difference < 0 || difference > safe_level_change)) {
		return false;
	}

	if (!increasing && (difference > 0 || difference < -safe_level_change)) {
		return false;
	}

	return true;
}

bool safe_level(const std::vector<long> &report) {
	auto last_level = 0l;
	bool increasing = is_increasing(report);

	for (auto level : report) {
		if (!last_level) {
			if (verbose > 1) {
				const char *inc = increasing ? "+ " : "- ";
				std::cout << inc << level;
			}

			last_level = level;
		} else {
			if (verbose > 1) {
				std::cout << " " << level;
			}

			if (!safe_change(last_level, level, increasing)) {
				if (verbose > 1) {
					std::cout << "\t\033[91;1;4mFAIL\033[0m\n";
				}
				return false;
			}
		}

		last_level = level;
	}

	if (verbose > 1) {
		std::cout << "\t\033[92;1;4m√\033[0m\n";
	}
	
	return true;
}

int part1(const std::string& path) {
    auto reports = read_data(path);
    long safe_reports  { 0 };

    for (const auto &report : reports) {
		if (safe_level(report)) {
			safe_reports++;
		}
    }

    return (int)safe_reports;
}

int part2(const std::string& path) {
    auto reports = read_data(path);
    long safe_reports  { 0 };

    return (int)safe_reports;
 }

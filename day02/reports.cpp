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
	// auto first = report.front();
	// auto it = report.begin();
	// while(it != report.end() && *it == first) {
    // 	it++;
	// }

	// return first < *it;
	long destiny = 0;
	long last_level = report[0];
	for (auto level : report) {
		destiny += level - last_level;
	}

	return destiny > 0;
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

// bool safe_distance(long last_level, long level, bool safe_distance) {
// 	return abs(level - last_level) <= safe_distance;
// }

void show_report(const std::vector<long> &report, bool increasing, std::size_t fail_index) {
	const char *inc = increasing ? "+ " : "- ";
	std::cout << inc;

	for (std::size_t i = 0; i < report.size(); i++) {
		auto level = report[i];
		if (i == fail_index) {
			std::cout << " \033[91;1;4m" << level << "\033[0m";
		} else {
			std::cout << " " << level;
		}
	}

	if (fail_index < report.size()) {
		std::cout << "\t\033[91;1;4mFAIL\033[0m\n";
	} else {
		std::cout << "\t\033[92;1;4m√\033[0m\n";
	}
}

// returns the offset we failed on
std::size_t safe_level(const std::vector<long> &report) {
	long last_level = report[0];
	bool increasing = is_increasing(report);
	std::size_t index;

	for (index = 1; index < report.size(); index++) {
		long level = report[index];
		bool safe = safe_change(last_level, level, increasing);

		if (!safe) {
			break;
		}

		last_level = level;
	}

	if (verbose > 1) {
		show_report(report, increasing, index);
	}

	return index;
}

int part1(const std::string& path) {
    auto reports = read_data(path);
    long safe_reports  { 0 };

    for (const auto &report : reports) {
		auto fail_index = safe_level(report);
		if (fail_index == report.size()) {
			safe_reports++;
		}
    }

    return (int)safe_reports;
}

// 554 too low
// 565 too low
// 566 is right answer
// 578 too high
int part2(const std::string& path) {
    auto reports = read_data(path);
    int safe_reports  { 0 };

    for (const auto &report : reports) {
		if (verbose > 1) {		
			std::cout << "- SIZE:\t\t" << report.size() << " ";
		}
		auto fail_index = safe_level(report);
		if (fail_index == report.size()) {
			safe_reports++;
			continue;
		}

		for (std::size_t i = 0; i < report.size(); i++) {
			auto try_again = report;
			try_again.erase(std::next(try_again.begin(), (long)i));
			if (verbose > 1) {		
				std::cout << "  " << i << " SIZE:\t" << try_again.size() << " ";
			}

			if (safe_level(try_again) == try_again.size()) {
				safe_reports++;
				break;
			}
		}

    }

    return safe_reports;
 }

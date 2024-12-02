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

std::vector<std::vector<long>> read_data(const std::string &path);

#include "aoc2024.h"

/* Maximum difference between two values to be a "safe" level change. */
const int min_safe_distance = 1;
const int max_safe_distance = 3;

/* Output verbosity level; 0 = nothing extra, 1 = more... Set by command line. */
int verbose = 0;

/* Read data from path and return a vector for each line in the file. */
std::vector<std::vector<long>> read_data(const std::string &path) {
    auto lines { read_lines(path) };
    std::vector<std::vector<long>> reports;

    for (const auto& line : lines) {
	    std::vector<long> levels;

		/* split numbers by spaces */
        for (auto level : split(line, " ")) {
			levels.push_back(atol(level.c_str()));
		}

		reports.push_back(levels);
	}

    return reports;
}

/* Returns if the numbers in the vector are increasing.
 *  - true generally increasing numbers
 *  - false generally decreasing numbers 
 */
bool is_increasing(const std::vector<long> &report) {
	int direction { 0 };
	long previous { report.front() };

	for (auto level : report) {
		direction += level - previous;
	}

	return direction > 0;
}

/* Return if the absolute difference is a safe change. */
bool safe_distance(long last_level, long level) {
	auto difference = abs(level - last_level);
 	return min_safe_distance <= difference && difference <= max_safe_distance;
}

/* Return if the change from last_level to level is safe.
 * Takes into account if we are on an increasing or decreasing path.
 */
bool safe_change(long last_level, long level, bool increasing) {
	auto difference = level - last_level;
	if (!safe_distance(last_level, level)) {
		return false;
	}

	return increasing ? difference > 0 : difference < 0;
}

/* Show the report highlighting where a failure was identified. */
void show_report(const std::vector<long> &report, bool increasing, std::size_t fail_index) {
	const char *inc = increasing ? "+" : "-";
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

/* Check a report to see if it is safe.
 * - return the index of where there was an unsafe change
 * - return the size of the report if all changes were safe (report.size())
 */
std::size_t safe_level(const std::vector<long> &report) {
	long last_level = report[0];
	bool increasing = is_increasing(report);
	std::size_t index;

	for (index = 1; index < report.size(); index++) {
		long level = report[index];

		if (!safe_change(last_level, level, increasing)) {
			break;
		}

		last_level = level;
	}

	return index;
}

int part1(const std::vector<std::vector<long>> reports) {
    long safe_reports  { 0 };

	if (verbose > 1) {
		std::cout << "\n";
	}

	for (std::size_t i = 0; i < reports.size(); i++) {
		auto &report = reports[i];
		auto fail_index = safe_level(report);

		if (verbose > 1) {
			std::cout << std::right << std::setfill('0') << std::setw(4) << i;
			show_report(report, is_increasing(report), fail_index);
		}

		if (fail_index == report.size()) {
			safe_reports++;
		}
    }

    return (int)safe_reports;
}

/* A little less brute-force solution for part 2. is it still O(n^2)?
 * - It runs relatively fast as the dataset is small.
 * - For each report it starts from the index where a problem occurs
 * - Then work backwards removing each element from the vector and see if it works
 * - 3976 (slow below) vs 2103 (fast here) comparisons on the input.txt
 */
int part2(const std::vector<std::vector<long>> reports) {
    int safe_reports = 0;

	if (verbose > 1) {
		std::cout << "\n";
	}

	for (std::size_t i = 0; i < reports.size(); i++) {
		auto &report = reports[i];
		auto fail_index = safe_level(report);

		if (verbose > 1) {
			std::cout << std::right << std::setw(4) << i;
			show_report(report, is_increasing(report), fail_index);
		}

		if (fail_index == report.size()) {
			safe_reports++;
			continue;
		}

		for (int j = (int)fail_index; j >= 0; j--) {
			auto damp_report = report;
			damp_report.erase(std::next(damp_report.begin(), (long)j));
			auto fail_index = safe_level(damp_report);

			if (verbose > 2) {
				std::cout << "    ";
				show_report(damp_report, is_increasing(damp_report), fail_index);
			}

			if (fail_index == damp_report.size()) {
				safe_reports++;
				break;
			}
		}
    }

	std::cout << std::setfill(' '); 
    return safe_reports;
 }

/* This is my original brute-force solution for part 2. is it O(n^2)?
 * - It runs relatively fast as the dataset is small.
 * - For each report it starts from index 0 and removes each element and tries 
 * 		the list again.
 */
int part2_slow(const std::vector<std::vector<long>> reports) {
    int safe_reports = 0;

	if (verbose > 1) {
		std::cout << "\n";
	}

	for (std::size_t i = 0; i < reports.size(); i++) {
		auto &report = reports[i];

		auto fail_index = safe_level(report);

		if (verbose > 1) {
			std::cout << std::right << std::setfill('0') << std::setw(4) << i;
			show_report(report, is_increasing(report), fail_index);
		}

		if (fail_index == report.size()) {
			safe_reports++;
			continue;
		}

		for (std::size_t j = 0; j < report.size(); j++) {
			auto damp_report = report;
			damp_report.erase(std::next(damp_report.begin(), (long)j));
			auto fail_index = safe_level(damp_report);

			if (verbose > 2) {
				std::cout << "    ";
				show_report(damp_report, is_increasing(damp_report), fail_index);
			}

			if (fail_index == damp_report.size()) {
				safe_reports++;
				break;
			}
		}
    }

	std::cout << std::setfill(' '); 
	return safe_reports;
}

// 554 too low
// 565 too low
// 566 is right answer
// 578 too high


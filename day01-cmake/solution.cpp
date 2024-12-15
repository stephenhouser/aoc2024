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
#include <unordered_map>
#include <map>
#include <array>
#include <chrono>

#include "aoc2024.h"
#include "solution.h"


const data_collection_t read_data(const std::string &path) {
    auto lines { read_lines(path) };
    std::vector<long> left;
    std::vector<long> right;

    for (const auto& line : lines) {
        auto locations = split(line, " ");
        left.push_back(std::atol(locations[0].c_str()));
        right.push_back( std::atol(locations[1].c_str()));
        }

    std::sort(left.begin(), left.end(), std::less<>());
    std::sort(right.begin(), right.end(), std::less<>());
    return {left, right};
}

long part1(const data_collection_t data) {
    auto [left, right] = data;
    auto locations { zip(left, right) };
    long distance  { 0 };

    for (const auto &[a, b] : locations) {
        distance += b - a;
    }

    return (int)distance;
}

long part2(const data_collection_t data) {
    auto [left, right] = data;
    std::unordered_map<long, long> matches;
    long similarity = 0;

    /* precompute number of items in right column in single pass.
     * unordered_map is slightly faster than regular std::map.
     */
    for (auto r : right) {
	    matches[r]++;
    }

    for (const auto key : left) {
	similarity += matches[key] * key;

	/* previous slower solution would go through right side for every
	 * unique number in the left side
         * C++20 has matches.contains(key), C++17 does not.
	 
        if (matches.find(key) == matches.end()) {
            matches[key] = std::count(right.begin(), right.end(), key);
	    lookups++;
        }
        similarity += key * matches[key];
	*/

    }

    return (int)similarity;
 }

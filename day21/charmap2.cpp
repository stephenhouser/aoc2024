
#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <sstream>		// std::ostringstream

#include "charmap2.h"

std::ostream& operator<<(std::ostream& os, const charmap2_t &map) {
	std::ostringstream buffer;

	os << "   ";
	for (int x = 0; x < map.size_x; ++x) {
		os << x % 10;
	}
	os << "\n";

	os << "-+-";
	for (int x = 0; x < map.size_x; ++x) {
		os << "-";
	}
	os << "-+-\n";


	for (auto y = 0; y <= map.size_y; y++) {
		os << y % 10 << "| ";

		for (auto x = 0; x < map.size_x; x++) {
			os << x;
		}
		os << " |" << y << "\n";
	}

	os << "-+-";
	for (size_t x = 0; x < (size_t)map.size_x; ++x) {
		os << "-";
	}
	os << "-+-\n   ";
	for (size_t x = 0; x < (size_t)map.size_x; ++x) {
		os << x % 10;
	}
	os << "\n";

	return os;
}
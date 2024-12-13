
#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output

#include "charmap.h"

std::ostream& operator<<(std::ostream& os, const charmap_t &map) {
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


	int y = 0;
	for (auto yit = map.data.begin(); yit != map.data.end(); ++yit) {
		os << y % 10 << "| ";

		for (auto xit = (*yit).begin(); xit != (*yit).end(); ++xit) {
			os << *xit;
		}
		os << " |" << y << "\n";
		++y;
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
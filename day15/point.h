#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string


using index_t = int;
using value_t = size_t;

struct point_t {
	index_t x = 0;
	index_t y = 0;

	point_t(index_t x, index_t y) {
		this->x = x;
		this->y = y;
	}

	point_t(const std::pair<int, int> &p) {
		this->x = p.first;
		this->y = p.second;
	}

	/* closest to 0 with preference towards closer x */
	bool operator<(const point_t &other) const {		
		if (this->y < other.y) {
			return true;
		}

		return this->x < other.x;
	}

	bool operator==(const point_t &other) const {
		return this->x == other.x && this->y == other.y;
	}
};

std::ostream& operator<<(std::ostream& os, const point_t &p) {
	std::ostringstream buffer;
	os << "(" << p.x << "," << p.y << ")";
	return os;
}

/* hash function so can be put in unordered_map or set */
template <>
struct std::hash<point_t> {
	size_t operator()(const point_t &p) const {
		return std::hash<size_t>()((((size_t)p.x & 0xFFFFFFFF) << 32) | ((size_t)p.y & 0xFFFFFFFF));
	}
};

#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string


using index_t = int;
using value_t = size_t;

struct point_t {
	index_t x = 0;
	index_t y = 0;
	value_t value = 0;

	point_t(index_t x, index_t y, value_t value = 0) {
		this->x = x;
		this->y = y;
		this->value = value;
	}

	point_t(const point_t &p) {
		this->x = p.x;
		this->y = p.y;
		this->value = p.value;
	}

	point_t(const point_t &p, value_t value) {
		this->x = p.x;
		this->y = p.y;
		this->value = value;
	}

	point_t(const std::pair<int, int> &p, value_t value = 0) {
		this->x = p.first;
		this->y = p.second;
		this->value = value;
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

	point_t &operator=(const point_t &other) {
		this->x = other.x;
		this->y = other.y;
		this->value = other.value;
		return *this;
	}


	void move(const point_t &direction) {
		this->x += direction.x;
		this->y += direction.y;
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


struct vector_t {
	point_t p = {0, 0};
	point_t dir = {0, 0};

	vector_t(const point_t &p, const point_t &dir) : p(p), dir(dir) {
		// this->p = p;
		// this->dir = dir;
	}

	vector_t(index_t x, index_t y, index_t dx, index_t dy, value_t value = 0) :
		p(x, y, value), dir(dx, dy, 0) {
		// this->p = point_t(x, y, value);
		// this->dir = point_t(dx, dy, 0);
	}

	vector_t(index_t x, index_t y, index_t dx, index_t dy, value_t v1 = 0, value_t v2 = 0) :
		p(x, y, v1), dir(dx, dy, v2) {
		// this->p = point_t(x, y, value);
		// this->dir = point_t(dx, dy, 0);
	}

	vector_t(const std::pair<index_t, index_t> &p, const point_t &dir) :
		p(p), dir(dir) {
		// this->p = p;
		// this->dir = dir;
	}


	/* closest to 0 with preference towards closer x */
	bool operator<(const vector_t &other) const {		
		return this->p.value < other.p.value;
	}

	bool operator==(const vector_t &other) const {
		return this->p == other.p && this->dir == other.dir;
	}

};


/* hash function so can be put in unordered_map or set */
template <>
struct std::hash<vector_t> {
	size_t operator()(const vector_t &v) const {
		return std::hash<size_t>()(
			  (((size_t)v.p.x & 0xFFFF) << 32)   | ((size_t)v.p.y & 0xFFFF) << 24
			| (((size_t)v.dir.x & 0xFFFF) << 16) | ((size_t)v.dir.y & 0xFFFF)
		);
	}
};


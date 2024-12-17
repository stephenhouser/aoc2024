#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string


using index_t = int;

struct point_t {
	index_t x = 0;
	index_t y = 0;
	index_t z = 0;

	point_t(index_t x, index_t y, index_t z = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	point_t(const point_t &p) {
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
	}

	point_t(const std::pair<int, int> &p) {
		this->x = p.first;
		this->y = p.second;
		this->z = 0;
	}

	bool operator<(const point_t &rhs) const {		
		// sorts by z, then y, then x 
		if (z == rhs.z) {
			if (y == rhs.y) {
				return x < rhs.x;
			}

			return y < rhs.y;
		}

		return z < rhs.z;
	}

	bool operator==(const point_t &other) const {
		return this->x == other.x
			&& this->y == other.y
			&& this->z == other.z;
	}

	bool operator!=(const point_t &other) const {
		return !(*this == other);
	}

	point_t &operator=(const point_t &other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	point_t &operator+=(const point_t& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
        return *this;
    }
 
    // friends defined inside class body are inline and are hidden from non-ADL lookup
    friend point_t operator+(point_t lhs, const point_t &rhs) {
        lhs += rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
	}
};

std::ostream& operator<<(std::ostream& os, const point_t &p) {
	std::ostringstream buffer;
	os << "(" << p.x << "," << p.y << "," << p.z << ")";
	return os;
}

/* hash function so can be put in unordered_map or set */
#if defined(POINT_T_2)
template <>
struct std::hash<point_t> {
	size_t operator()(const point_t &p) const {
		return std::hash<size_t>()((((size_t)p.x & 0xFFFFFFFF) << 32) 
								 | (((size_t)p.y & 0xFFFFFFFF)      ));
	}
};
#else
template <>
struct std::hash<point_t> {
	size_t operator()(const point_t &p) const {
		return std::hash<size_t>()((((size_t)p.z & 0xFFFF) << 32)
								 | (((size_t)p.x & 0xFFFF) << 24) 
								 | (((size_t)p.y & 0xFFFF)      ));
	}
};
#endif


struct vector_t {
	point_t p = {0, 0};
	point_t dir = {0, 0};

	vector_t(const point_t &p, const point_t &dir) 
		: p(p), dir(dir) {
	}

	vector_t(index_t x, index_t y, index_t dx, index_t dy)
		: p(x, y), dir(dx, dy, 0) {
	}

	vector_t(index_t x, index_t y, index_t z, index_t dx, index_t dy, index_t dz) :
		p(x, y, z), dir(dx, dy, dz) {
	}

	vector_t(const std::pair<index_t, index_t> &p, const std::pair<index_t, index_t> &dir) :
		p(p), dir(dir) {
	}

	bool operator<(const vector_t &rhs) const {
		if (p == rhs.p) {
			return dir < rhs.dir;
		}

		return p < rhs.p;
	}

	bool operator==(const vector_t &other) const {
		return this->p == other.p && this->dir == other.dir;
	}
};

template <>
struct std::hash<vector_t> {
	size_t operator()(const vector_t &v) const {
		return std::hash<size_t>()((((size_t)  v.p.x & 0xFFFF) << 32)   
								 | (((size_t)  v.p.y & 0xFFFF) << 24)
								 | (((size_t)v.dir.x & 0xFFFF) << 16)
								 | (((size_t)v.dir.y & 0xFFFF)      ));
	}
};

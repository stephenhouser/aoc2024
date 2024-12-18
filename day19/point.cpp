#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string

#include "point.h"

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


template <>
struct std::hash<vector_t> {
	size_t operator()(const vector_t &v) const {
		return std::hash<size_t>()((((size_t)  v.p.x & 0xFFFF) << 32)   
								 | (((size_t)  v.p.y & 0xFFFF) << 24)
								 | (((size_t)v.dir.x & 0xFFFF) << 16)
								 | (((size_t)v.dir.y & 0xFFFF)      ));
	}
};

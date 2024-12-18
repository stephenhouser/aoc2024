#include <string>		// std::string
#include <vector>		// std::vector

#include "point.h"

using data_t = point_t;
using data_collection_t = std::vector<point_t>;

extern long part1(data_collection_t data);
extern long part2(data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

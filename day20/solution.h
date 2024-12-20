#include <string>		// std::string
#include <vector>		// std::vector

#include "charmap.h"
using data_collection_t = charmap_t;

extern long part1(data_collection_t data);
extern long part2(data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

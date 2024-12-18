#include <string>		// std::string
#include <vector>		// std::vector
#include <cstdint>

using data_t = std::string;
using data_collection_t = std::vector<data_t>;

extern uint64_t part1(data_collection_t data);
extern uint64_t part2(data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

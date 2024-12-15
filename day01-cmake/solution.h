#include <string>		// std::string
#include <vector>		// std::vector
#include <tuple>		// std::tuple

using data_t = long;
using data_collection_t = std::tuple<std::vector<data_t>, std::vector<data_t>>;

extern long part1(const data_collection_t data);
extern long part2(const data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

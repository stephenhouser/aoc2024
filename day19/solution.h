#include <string>		// std::string
#include <vector>		// std::vector

using data_t = std::string;

// towels, towel arrangements
using data_collection_t = std::tuple<std::vector<std::string>, std::vector<std::string>>;

extern long part1(data_collection_t data);
extern long part2(data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

#include <string>		// std::string
#include <vector>		// std::vector

// using data_t = std::string;
using data_collection_t = std::pair<charmap_t, std::vector<char>>;

extern long part1(data_collection_t data);
extern long part2(data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

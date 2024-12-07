#include <string>		// std::string
#include <vector>		// std::vector

using data_collection_t = std::vector<std::tuple<long, std::vector<long>>>;

extern long part1(data_collection_t data);
extern long part2(data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

extern bool flag_backward;
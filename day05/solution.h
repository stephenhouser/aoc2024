#include <string>		// std::string
#include <vector>		// std::vector
#include <map>
#include <set>

// map number to tuple of <before , after> rules
using rule_t = std::tuple<std::set<int>, std::set<int>>;
using rules_t = std::map<int, rule_t>;
using updates_t = std::vector<std::vector<int>>;

using data_collection_t = std::tuple<rules_t, updates_t>;

extern long part1(data_collection_t data);
extern long part2(data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

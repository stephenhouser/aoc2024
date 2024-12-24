#if !defined(SOLUTION_H)
#define SOLUTION_H

#include <string>		// std::string
#include <vector>		// std::vector
#include <map>


enum gate_t {
	AND,
	OR,
	XOR
};

struct node_t {
	gate_t op;
	std::string a;
	std::string b;
	std::string out;
};

using network_t = std::map<std::string, int>;
using data_collection_t = std::tuple<network_t, std::vector<node_t>>;

extern long part1(data_collection_t data);
extern long part2(data_collection_t data);
extern const data_collection_t read_data(const std::string &path);

#endif
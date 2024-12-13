#if !defined(CHARMAP_T_H)
#define CHARMAP_T_H

#include <string>		// std::string
#include <vector>		// std::vector

// using index_t = long;
// using value_t = long;

struct charmap_t {
	long size_x = 0;
	long size_y = 0;

	std::vector<std::vector<char>> data = {};

	charmap_t(const std::vector<std::string> &lines) {
		for (const auto& line : lines) {
			std::vector<char> map_row(line.begin(), line.end());
			this->data.push_back(map_row);
		}

		size_y = (long)this->data.size();
		if (size_y) {
			size_x = (long)this->data[0].size();
		}
	}

	bool is_valid(long x, long y) const {
		return 0 <= x && x < this->size_x
			&& 0 <= y && y < this->size_y;
	}

	char get(long x, long y) const {
		return this->is_valid(x, y) ? data[(size_t)y][(size_t)x] : '\0';
	}

	bool is_char(long x, long y, char c) const {
		return is_valid(x, y) && data[(size_t)y][(size_t)x] == c;
	}

};

std::ostream& operator<<(std::ostream& os, const charmap_t &map);

#endif
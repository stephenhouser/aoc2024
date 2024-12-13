#if !defined(CHARMAP_T_H)
#define CHARMAP_T_H

#include <string>		// std::string
#include <vector>		// std::vector

struct charmap_t {
	int size_x = 0;
	int size_y = 0;

	std::vector<std::vector<char>> data = {};

	charmap_t(const std::vector<std::string> &lines) {
		for (const auto& line : lines) {
			std::vector<char> map_row(line.begin(), line.end());
			this->data.push_back(map_row);
		}

		this->size_y = static_cast<int>(this->data.size());
		if (this->size_y) {
			this->size_x = static_cast<int>(this->data[0].size());
		}
	}

	bool is_valid(int x, int y) const {
		return 0 <= x && x < this->size_x
			&& 0 <= y && y < this->size_y;
	}

	bool is_valid(size_t x, size_t y) const {
		return is_valid(static_cast<int>(x), static_cast<int>(y));
	}

	char get(int x, int y) const {
		return this->is_valid(x, y) ? data[(size_t)y][(size_t)x] : '\0';
	}

	char get(size_t x, size_t y) const {
		return get(static_cast<int>(y), static_cast<int>(x));
	}

	bool is_char(int x, int y, char c) const {
		return is_valid(x, y) && data[(size_t)y][(size_t)x] == c;
	}

	bool is_char(size_t x, size_t y, char c) const {
		return is_char(static_cast<int>(y), static_cast<int>(x), c);
	}

};

std::ostream& operator<<(std::ostream& os, const charmap_t &map);

#endif
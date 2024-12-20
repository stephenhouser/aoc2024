#if !defined(CHARMAP2_T_H)
#define CHARMAP2_T_H

#include <string>
#include <stdexcept>
#include <vector>

// Define maximum dimensions for the map
const int MAX_SIZE_X = 1000;
const int MAX_SIZE_Y = 1000;

struct charmap2_t {
    int size_x = 0;
    int size_y = 0;
    
    char data[MAX_SIZE_Y][MAX_SIZE_X];  // Fixed-size 2D array

    charmap2_t() {
        // Initialize with default values
        for (int y = 0; y < MAX_SIZE_Y; y++) {
            for (int x = 0; x < MAX_SIZE_X; x++) {
                data[y][x] = '.';
            }
        }
    }

    charmap2_t(size_t sx, size_t sy) {
        if (sx > MAX_SIZE_X || sy > MAX_SIZE_Y) {
            throw std::runtime_error("Map size exceeds maximum dimensions");
        }

        size_x = static_cast<int>(sx);
        size_y = static_cast<int>(sy);

        // Initialize with dots
        for (int y = 0; y < size_y; y++) {
            for (int x = 0; x < size_x; x++) {
                data[y][x] = '.';
            }
        }
    }

    charmap2_t(const std::vector<std::string> &lines) {
        if (lines.size() > MAX_SIZE_Y || (lines.size() > 0 && lines[0].size() > MAX_SIZE_X)) {
            throw std::runtime_error("Map size exceeds maximum dimensions");
        }

        size_y = static_cast<int>(lines.size());
        size_x = size_y > 0 ? static_cast<int>(lines[0].size()) : 0;

        // Copy data from strings
        for (int y = 0; y < size_y; y++) {
            const auto& line = lines[(size_t)y];
            for (int x = 0; x < size_x; x++) {
                data[y][x] = line[(size_t)x];
            }
        }
    }

    void add_line(const std::string &line) {
        if (size_y >= MAX_SIZE_Y || line.size() > MAX_SIZE_X) {
            throw std::runtime_error("Cannot add line: exceeds maximum dimensions");
        }

        if (size_y == 0) {
            size_x = static_cast<int>(line.size());
        } else if (line.size() != static_cast<size_t>(size_x)) {
            throw std::runtime_error("Line length doesn't match existing width");
        }

        for (int x = 0; x < size_x; x++) {
            data[size_y][x] = line[(size_t)x];
        }
        size_y++;
    }

    void add_line(const std::vector<char> &line) {
        if (size_y >= MAX_SIZE_Y || line.size() > MAX_SIZE_X) {
            throw std::runtime_error("Cannot add line: exceeds maximum dimensions");
        }

        if (size_y == 0) {
            size_x = static_cast<int>(line.size());
        } else if (line.size() != static_cast<size_t>(size_x)) {
            throw std::runtime_error("Line length doesn't match existing width");
        }

        for (int x = 0; x < size_x; x++) {
            data[size_y][x] = line[(size_t)x];
        }
        size_y++;
    }

    bool is_valid(int x, int y) const {
        return 0 <= x && x < this->size_x && 0 <= y && y < this->size_y;
    }

    bool is_valid(size_t x, size_t y) const {
        return is_valid(static_cast<int>(x), static_cast<int>(y));
    }

    char get(int x, int y) const {
        return this->is_valid(x, y) ? data[y][x] : '\0';
    }

    char get(size_t x, size_t y) const {
        return get(static_cast<int>(x), static_cast<int>(y));
    }

    void set(int x, int y, char c) {
        if (this->is_valid(x, y)) {
            data[y][x] = c;
        }
    }

    void set(size_t x, size_t y, char c) {
        set(static_cast<int>(x), static_cast<int>(y), c);
    }

    bool is_char(int x, int y, char c) const {
        return is_valid(x, y) && data[y][x] == c;
    }

    bool is_char(size_t x, size_t y, char c) const {
        return is_char(static_cast<int>(x), static_cast<int>(y), c);
    }

    std::pair<size_t, size_t> find_char(char c = '^') const {
        for (int y = 0; y < this->size_y; y++) {
            for (int x = 0; x < this->size_x; x++) {
                if (is_char(x, y, c)) {
                    return {x, y};
                }
            }
        }
        return {-1, -1};
    }
};

std::ostream& operator<<(std::ostream& os, const charmap2_t &map);

#endif


#include <string>		// std::string
#include <vector>		// std::vector

extern int verbose;

std::string read_file(const std::string &filename);
std::vector<std::string> read_lines(const std::string &filename);

std::vector<std::string> split(const std::string &str, const std::string &delim);

template <typename T, typename U>
std::vector<std::tuple<T, U>> zip(std::vector<T> left, std::vector<U> right) {
    std::vector<std::tuple<T, U>> zipped;

    for (std::size_t i = 0; i < left.size(); i++) {
        zipped.push_back({left[i], right[i]});
    }

    return zipped;
}
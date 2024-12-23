
#include <string>		// std::string
#include <vector>		// std::vector
#include <sstream>

extern int verbose;

const std::string read_file(const std::string &filename);
const std::vector<std::string> read_lines(const std::string &filename);

const std::vector<std::string> split(const std::string &str, const std::string &delim);
const std::vector<int> split_int(const std::string &str, const std::string &delim);

template <typename T, typename U>
const std::vector<std::tuple<T, U>> zip(std::vector<T> left, std::vector<U> right) {
    std::vector<std::tuple<T, U>> zipped;

    for (std::size_t i = 0; i < left.size(); i++) {
        zipped.push_back({left[i], right[i]});
    }

    return zipped;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T> &vec) {
	std::ostringstream buffer;
    const char delimiter = ',';

    bool first = true;
	for (auto n : vec) {
        if (!first) {
            os << delimiter;
        }

        os << n;
        first = false;
	}

	return os;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
template <typename T,
          typename TIter = decltype(std::begin(std::declval<T>())),
          typename = decltype(std::end(std::declval<T>()))>
constexpr auto enumerate(T && iterable) {
    struct iterator {
        size_t i;
        TIter iter;
        bool operator != (const iterator & other) const { return iter != other.iter; }
        void operator ++ () { ++i; ++iter; }
        auto operator * () const { return std::tie(i, *iter); }
    };

    struct iterable_wrapper {
        T iterable;
        auto begin() { return iterator{ 0, std::begin(iterable) }; }
        auto end() { return iterator{ 0, std::end(iterable) }; }
    };

    return iterable_wrapper{ std::forward<T>(iterable) };
}

// adapted from https://cp-algorithms.com/string/string-hashing.html
size_t string_hash(const std::string &s) {
	const size_t p = 31;
	const size_t m = 1e9 + 9;
	size_t hash_value = 0;
	size_t p_pow = 1;

	for (size_t i = 0; i < s.length(); i++) {
		hash_value = (hash_value + ((size_t)s[i] - (size_t)'a' + (size_t)1) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}

	return hash_value;
}
#pragma GCC diagnostic pop

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <string>    
#include <ranges>
#include <string_view>
#include <getopt.h>
#include <charconv>
#include <tuple>
#include <cstring>
#include <algorithm>
#include <map>

bool verbose = false;

using string_vector = std::vector<std::string>;
using ulong = unsigned long;

// std::string read_file(std::string_view path) {
//     std::ios::sync_with_stdio(false);

//     std::ifstream in{path.data(), std::ios::in | std::ios::binary};
//     in.exceptions(std::ifstream::badbit);

//     std::string data;
//     for (std::array<char, 4096> buffer; in;) {
//         in.read(buffer.data(), buffer.size());        
//         std::ranges::copy(buffer | std::views::take(in.gcount()), std::back_inserter(data));
//   }

//   return data;
// }

std::vector<std::string> split(std::string str, std::string delim) {
    std::vector<std::string> tokens;
    char *str_c = strdup(str.c_str()); 
    char* token = NULL;

    token = strtok(str_c, delim.c_str()); 
    while (token != NULL) { 
        tokens.push_back(std::string(token));  
        token = strtok(NULL, delim.c_str()); 
    }

    delete[] str_c;
    return tokens;
}

std::vector<std::tuple<long, long>> zip(std::vector<long> left, std::vector<long> right) {
    std::vector<std::tuple<long, long>> zipped;

    for (std::size_t i = 0; i < left.size(); i++) {
        zipped.push_back({left[i], right[i]});
    }

    return zipped;
}

string_vector read_lines(std::string const path) {
    string_vector lines;

    std::ifstream ifs(path);
    if (!ifs) {
        throw std::runtime_error("Error opening file.");
    }

    std::string line;
    while (std::getline(ifs, line)) {
		lines.push_back(line);
    }

    return lines;
}

std::tuple<std::vector<long>, std::vector<long>> read_data(std::string path) {
    auto lines = read_lines(path);

    std::vector<long> left;
    std::vector<long> right;

    for (const auto& line : lines) {
        auto locations = split(line, " ");
        left.push_back(std::atol(locations[0].c_str()));
        right.push_back( std::atol(locations[1].c_str()));
        }

    std::sort(left.begin(), left.end(), std::less<>());
    std::sort(right.begin(), right.end(), std::less<>());
    return {left, right};
}

int part2(std::vector<long> left, std::vector<long> right) {
    std::map<long, long> matches;

    long similarity { 0 };
    for (const auto key : left) {
        if (!matches.contains(key)) {
            matches[key] = std::count(right.begin(), right.end(), key);
        }
        similarity += key * matches[key];
    }
     //return std::reduce(elves.begin(), elves.begin() + 3, 0);
     return (int)similarity;
 }

int part1(std::vector<long> left, std::vector<long> right) {
    auto locations{ zip(left, right) };

    long distance = 0;
    for (const auto &[a, b] : locations) {
        distance += b - a;
    }

    return (int)distance;
}

void help(std::string_view program) {
    std::cout << program << "[-hv] <file...>" << "\n";
    std::cout << "\t-h\thelp" << "\n";
    std::cout << "\t-v\tverbose" << "\n";
}

int main(int argc, char **argv) {
    char *program_name = argv[0];

    int c;
    while ((c = getopt(argc, argv, "hv")) != -1) {
        switch (c) {
            case 'v': 
                verbose = true;
                break;
            default:
                help(program_name);
                return 0;
        }
    }
    argc -= optind; 
    argv += optind; 

    if (!argc) {
        std::cerr << "Error: No file specified.\n";
        help(program_name);
    }

    for (int arg = 0; arg < argc; arg++) {
        std::string path{argv[arg]};

        try {
            auto [left, right] = read_data(path);
            int p1 = part1(left, right);
            std::cout << "Part 1: Sum of Distances : " << p1 << std::endl;

            int p2 = part2(left, right);
            std::cout << "Part 2: Similarity Score : " << p2 << std::endl;

        } catch(std::exception const& e) {
            std::cerr << "Exception: " << e.what() << "\n";
            throw;
        } catch(...) {
            std::cerr << "Exception: Unknown\n";
            throw;
        }
    }

    return 0;   
}
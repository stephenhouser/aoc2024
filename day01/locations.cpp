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

bool verbose = false;

using string_vector = std::vector<std::string>;

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

std::vector<std::tuple<int, int>> zip(std::vector<int> a, std::vector<int> b) {
    std::vector<std::tuple<int, int>> zipped;

    for (unsigned int i = 0; i < a.size(); i++) {
        zipped.push_back({a[i], b[i]});
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

std::tuple<std::vector<int>, std::vector<int>>
read_data(std::string path) {
    auto lines = read_lines(path);

    std::vector<int> locations1;
    std::vector<int> locations2;

    for (const auto& line : lines) {
        auto locations = split(line, " ");
        locations1.push_back(std::atoi(locations[0].c_str()));
        locations2.push_back( std::atoi(locations[1].c_str()));

        // std::cout << std::atoi(locations[0].c_str()) << ":" << 
        //         std::atoi(locations[1].c_str()) << "\n";
        }

    std::sort(locations1.begin(), locations1.end(), std::less<>());
    std::sort(locations2.begin(), locations2.end(), std::less<>());

    return {locations1, locations2};
}

// int part2(std::vector<int> elves) {
//     //return std::reduce(elves.begin(), elves.begin() + 3, 0);
// }

int part1(std::vector<int> locations1, std::vector<int> locations2) {
    auto locations{ zip(locations1, locations2) };

    int distance = 0;
    for (const auto &[a, b] : locations) {
        distance += abs(a - b);
    }

    return distance;
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
            auto [l1, l2] = read_data(path);
            int p1 = part1(l1, l2);
            std::cout << "Part 1: Sum of Distances : " << p1 << std::endl;


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
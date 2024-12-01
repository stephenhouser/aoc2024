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
#include <array>

bool verbose = false;

std::vector<std::string> split(const std::string &str, const std::string &delim) {
    std::vector<std::string> tokens;
    char *str_c { strdup(str.c_str()) };
    char *token { NULL };

    token = strtok(str_c, delim.c_str()); 
    while (token != NULL) { 
        tokens.push_back(std::string(token));  
        token = strtok(NULL, delim.c_str()); 
    }

    free(str_c);
    return tokens;
}

template <typename T, typename U>
std::vector<std::tuple<T, U>> zip(std::vector<T> left, std::vector<U> right) {
    std::vector<std::tuple<T, U>> zipped;

    for (std::size_t i = 0; i < left.size(); i++) {
        zipped.push_back({left[i], right[i]});
    }

    return zipped;
}

// from https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string read_file(const std::string &filename) {
    // get file size to read entire file in one go.
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)     {
        std::string contents;

        in.seekg(0, std::ios::end);
        auto file_size = in.tellg();
        if (file_size > 0) {
            contents.resize((std::size_t)file_size);
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], file_size);
        }

        in.close();
        return contents;
    }

    throw(errno);
}

std::vector<std::string> read_lines(const std::string &path) {
    std::vector<std::string> lines;

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



std::tuple<std::vector<long>, std::vector<long>> read_data(std::string &path) {
    auto lines { read_lines(path) };
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

int part1(std::string& path) {
    auto [left, right] = read_data(path);
    auto locations { zip(left, right) };
    long distance  { 0 };

    for (const auto &[a, b] : locations) {
        distance += b - a;
    }

    return (int)distance;
}

int part2(std::string& path) {
    auto [left, right] = read_data(path);
    std::map<long, long> matches;
    long similarity { 0 };

    for (const auto key : left) {
        // C++20 has matches.contains(key), C++17 does not.
        if (matches.find(key) == matches.end()) {
            matches[key] = std::count(right.begin(), right.end(), key);
        }
        similarity += key * matches[key];
    }

    return (int)similarity;
 }

void help(std::string_view program) {
    std::cout << program << "[-hv12] <file...>" << "\n";
    std::cout << "\t-h\thelp" << "\n";
    std::cout << "\t-v\tverbose" << "\n";
}

int main(int argc, char **argv) {
    char *program_name = argv[0];
    unsigned int part = 0x00;

    int c;
    while ((c = getopt(argc, argv, "hv12")) != -1) {
        switch (c) {
            case 'v': 
                verbose = true;
                break;
            case '1':
                part |= 0x01;
                break;
            case '2':
                part |= 0x02;
                break;
            default:
                help(program_name);
                return 0;
        }
    }

    argc -= optind; 
    argv += optind; 

    if (!part) {
        part = 0x03;
    }

    if (!argc) {
        std::cerr << "Error: No file specified.\n";
        help(program_name);
    }

    for (int arg = 0; arg < argc; arg++) {
        std::string path{argv[arg]};
        if (verbose) {
            std::cout << path << "\t";
        }

        try {

            if (part & 0x01) {
                int p1 = part1(path);
                if (verbose) {
                    std::cout << "p1=";
                }

                std::cout << p1;
            }

            if (part == 0x03) {
                std::cout << "\t";
            }

            if (part & 0x02) {
                int p2 = part2(path);
                if (verbose) {
                    std::cout << "p2=";
                }

                std::cout << p2;
            }

            std::cout << std::endl;
        } catch(std::exception const& e) {
            std::cerr << "Exception: " << e.what() << "\n";
            //throw;
        } catch(...) {
            std::cerr << "Exception: Unknown\n";
            //throw;
        }
    }

    return 0;   
}
#include <getopt.h>     // getopt() argument processing

#include <chrono>       // high resolution timer
#include <cstring>      // strtok, strdup
#include <fstream>      // ifstream (reading file)
#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output

#include "charmap.h"
#include "aoc2024.h"
#include "solution.h"

/* Output verbosity level; 0 = nothing extra, 1 = more... Set by command line. */
int verbose = 0;
bool show_time = false;

int answer_width = 15;
int time_width = 10;
int time_precision = 4;

const std::vector<std::string> split(const std::string &str, const std::string &delim) {
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

// from https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
const std::string read_file(const std::string &filename) {
    // get file size to read entire file in one go.
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
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

const std::vector<std::string> read_lines(const std::string &path) {
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

int part_wrapper(int part_n, long solution, const std::string &path) {
    int success = 0;
    if (solution >= 0) { // 0 <= solution, run the code...
        auto data = read_data(path);

        auto start = std::chrono::high_resolution_clock::now();
        auto answer = part_n == 1 ? part1(data) : part2(data);
        auto finish = std::chrono::high_resolution_clock::now();

        if (0 < solution) { // 0 < solution, check the answer
            success = (answer == solution) ? 1 : 0;

            if (verbose) {
                if (part_n == 1 && answer == solution) {
                    std::cout << "silver=\033[1;97m* ";
                } else if (part_n == 1 && answer != solution) {
                    std::cout << "silver=  ";
                } else if (part_n == 2 && answer == solution) {
                    std::cout << "  gold=\033[1;93m* ";
                } else if (part_n == 2 && answer != solution) {
                    std::cout << "  gold=  ";
                }
            }
        }

        std::cout << std::setw(answer_width) << std::left 
                  << answer << "\033[0m";

        std::chrono::duration<double, std::milli> ms_double = finish - start;
        if (verbose) {
            /* Getting number of milliseconds as a double. */
            std::cout << " ("
                      << std::setw(time_width) << std::fixed << std::right 
                      << std::setprecision(time_precision)
                      << ms_double.count() << "ms)";
        } else if (show_time) {
            /* Getting number of milliseconds as a double. */
            std::cout << std::fixed 
                      << std::setprecision(time_precision) 
                      << ms_double.count() << "ms";
        }
    }

    return success ? 0 : 1;
}

void help(std::string_view program) {
    std::cout << program << "[-hvt1:2:] <file...>" << "\n";
    std::cout << "\t-s n\tcheck solution for part 1 (silver) against n; n=-1 don't run, 0=don't check" << "\n";
    std::cout << "\t-g n\tcheck solution for part 2 (gold) against n; n=-1 don't run, 0=don't check" << "\n";
    std::cout << "\t-t\tprint execution time" << "\n";
    std::cout << "\t-h\thelp" << "\n";
    std::cout << "\t-v\tverbose" << "\n";
}

int main(int argc, char **argv) {
    char *program_name = argv[0];
    long p1_answer = 0;
    long p2_answer = 0;
    int exit_code = 0;

    int c;
    while ((c = getopt(argc, argv, "hvts:g:")) != -1) {
        switch (c) {
            case 'v': 
                verbose++;
                break;
            case 't':
                show_time = true;
                break;
            case 's':
                p1_answer = strtol(optarg, NULL, 10);
                break;
            case 'g':
                p2_answer = strtol(optarg, NULL, 10);
                break;
            default:
                help(program_name);
                return 0;
        }
    }

    argc -= optind; 
    argv += optind; 

    if (!argc) {
        std::cerr << "Error: No file specified." << std::endl;
        help(program_name);
        return EXIT_FAILURE;;
    }

    for (int arg = 0; arg < argc; arg++) {
        std::string path{argv[arg]};
        if (verbose) {
            std::cout << std::setw(10) << std::left << path << " ";
        }

        try {
            exit_code += part_wrapper(1, p1_answer, path);

            if (p1_answer >= 0 && p2_answer >= 0) {
                std::cout << "\t";
            }

            exit_code += part_wrapper(2, p2_answer, path);

            std::cout << std::endl;
        } catch(std::exception const& e) {
            std::cerr << "Exception: " << e.what() << "\n";
            //throw;
        } catch(...) {
            std::cerr << "Exception: Unknown\n";
            //throw;
        }
    }

    return exit_code;
}
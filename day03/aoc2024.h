
extern int verbose;
extern int part1(std::vector<data_t> data);
extern int part2(std::vector<data_t> data);

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

int part_wrapper(int part_n, int solution, const std::string &path) {
    int failure = 0;
    if (solution >= 0) { // 0 <= solutioin, run the code...
        if (verbose) {
            std::cout << "p" << part_n << "=";
        }

        auto data = read_data(path);

        auto start = std::chrono::high_resolution_clock::now();
        int answer = part_n == 1 ? part1(data) : part2(data);
        auto finish = std::chrono::high_resolution_clock::now();

        if (0 < solution) { // 0 < solution, check the answer
            failure = (answer == solution) ? 0 : 1;

            if (verbose) {
                const char *success = (failure == 0) ? "√ " : "! ";
                std::cout << success;
            }
        }

        std::cout << std::setw(10) << std::left << answer;

        if (verbose) {
            /* Getting number of milliseconds as a double. */
            std::chrono::duration<double, std::milli> ms_double = finish - start;
            std::cout << " (" 
                      << std::setw(10) << std::fixed << std::right << std::setprecision(6)
                      << ms_double.count() << "ms)";

        }
    }

    return failure;
}

void help(std::string_view program) {
    std::cout << program << "[-hv1:2:] <file...>" << "\n";
    std::cout << "\t-1\trun part 1" << "\n";
    std::cout << "\t-2\trun part 2" << "\n";
    std::cout << "\t-h\thelp" << "\n";
    std::cout << "\t-v\tverbose" << "\n";
}


int main(int argc, char **argv) {
    char *program_name = argv[0];
    int p1_answer = 0;
    int p2_answer = 0;
    int exit_code = 0;

    int c;
    while ((c = getopt(argc, argv, "hv1:2:")) != -1) {
        switch (c) {
            case 'v': 
                verbose++;
                break;
            case '1':
                p1_answer = atoi(optarg);
                break;
            case '2':
                p2_answer = atoi(optarg);
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
            std::cout << std::setw(10) << std::left << path << "\t";
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
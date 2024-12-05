#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate

#include "aoc2024.h"
#include "solution.h"

/* Emulated computer state. */
typedef struct CPU_ {
	long accumulator = 0;
	bool enable_mul = true;
} CPU;

/* Emulated instructions. */

const char *instr_mul_regex = "mul\\((\\d+),(\\d+)\\)";
void instr_mul(CPU &cpu, const std::vector<std::string> &argv) {
	if (cpu.enable_mul) {
		auto l = stoi(argv[1]);
		auto r = stoi(argv[2]);
		cpu.accumulator += l * r;
	}
}

const char *instr_do_regex = "do\\(\\)";
void instr_do(CPU &cpu, [[maybe_unused]] const std::vector<std::string> &argv) {
	cpu.enable_mul = true;
}

const char *instr_dont_regex = "don't\\(\\)";
void instr_dont(CPU &cpu, [[maybe_unused]] const std::vector<std::string> &argv) {
	cpu.enable_mul = false;
}

/* vector of valid instructions */
std::vector<std::string> i_patterns {
	instr_mul_regex, instr_do_regex, instr_dont_regex
};

/* regex built from instruction signatures */
std::string i_regex = std::accumulate(
	std::next(i_patterns.begin()), i_patterns.end(), i_patterns[0], 
	[](std::string &a, std::string &s) {
		return a.empty() ? s : a + "|" + s;
	});

long part1([[maybe_unused]]const std::vector<data_t> data) {
	long accumulator = 0;
	std::regex mul_regex("mul\\((\\d+),(\\d+)\\)");

	for (auto line : data) {
		auto m_beg = std::sregex_iterator(line.begin(), line.end(), mul_regex);
		auto m_end = std::sregex_iterator();
		for (std::regex_iterator i = m_beg; i != m_end; ++i) {
			int l = stoi((*i).str(1));
			int r = stoi((*i).str(2));

			accumulator += l * r;
		}
	}

	return accumulator;
}

std::vector<std::string> to_argv(const std::smatch &rg_match) {
	// set first argument to just the function name
	std::vector<std::string> argv {
		rg_match.str().substr(0, rg_match.str().find("("))
	};

	// remaining arguments from non-empty matches
	std::copy_if(std::next(rg_match.begin()), rg_match.end(), 
		std::back_inserter(argv), 
		[](auto &s) { return !s.str().empty(); }
	);

	if (verbose > 1) {
		for (auto arg : argv) {
			std::cout << arg << " ";
		}
		std::cout << std::endl;
	}

	return argv;
}

long part2(const std::vector<data_t> data) {
	CPU cpu;

	for (auto line : data) {
		std::regex instr_regex { i_regex };
		auto m_beg = std::sregex_iterator(line.begin(), line.end(), instr_regex);
		auto m_end = std::sregex_iterator();
		for (std::regex_iterator i = m_beg; i != m_end; ++i) {
			std::smatch match = *i;

			auto arguments = to_argv(*i);

			if (arguments[0] == "mul") {
				instr_mul(cpu, arguments);
			} else if (arguments[0] == "do") {
				instr_do(cpu, arguments);
			} else if (arguments[0] == "don't") {
				instr_dont(cpu, arguments);
			}
		}
	}

	return cpu.accumulator;
}

/* Read data from path and return a vector for each line in the file. */
const std::vector<data_t> read_data(const std::string &path) {
	return read_lines(path);
}


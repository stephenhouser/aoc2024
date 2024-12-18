#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <limits.h>
// #include <cmath>

#include "aoc2024.h"
#include "solution.h"

struct cpu_t {
	size_t a = 0;
	size_t b = 0;
	size_t c = 0;
	int ip = 0;

	cpu_t() {
	}

	cpu_t(const cpu_t &saved) {
		a = saved.a;
		b = saved.b;
		c = saved.c;
		ip = saved.ip;
	}
};

std::ostream& operator<<(std::ostream& os, const cpu_t &cpu) {
	std::ostringstream buffer;
	os << "A=" << cpu.a << " B=" << cpu.b << " C=" << cpu.c;
	return os;
}



std::tuple<cpu_t, std::vector<int>>
load_program(const data_collection_t &data) {
	cpu_t cpu;
	std::vector<int> instructions;

	for (auto line : data) {
		if (line.rfind("Register A:") == 0) {
			cpu.a = (size_t)atol(line.substr(line.rfind(":")+1,100).c_str());
		}
		if (line.rfind("Register B:") == 0) {
			cpu.b = (size_t)atol(line.substr(line.rfind(":")+1,100).c_str());
		}
		if (line.rfind("Register C:") == 0) {
			cpu.c = (size_t)atol(line.substr(line.rfind(":")+1,100).c_str());
		}

		if (line.rfind("Program:") == 0) {
			auto is = split_int(line.substr(line.rfind(":")+1,100), ",");
			instructions.insert(instructions.begin(), is.begin(), is.end());
		}
	}

	return {cpu, instructions};
} 

// combo
// 0-3 = literal 0-3
// 4=reg A, 5=regB, 6=regC
size_t get_combo(const cpu_t &cpu, size_t combo) {
	switch (combo) {
		case 0:
		case 1:
		case 2:
		case 3:
			return combo;
		case 4:
			return cpu.a;
		case 5:
			return cpu.b;
		case 6:
			return cpu.c;
		default:
			return 0;
	}

	return 0;
}


// 0 = adv; divide;  A = A / 2^combo
// 1 = bxl; bitwise xor; B = B ^ literal operand
// 2 = bst; B = combo % 8
// 3 = jnz; if A != 0; jump to literal operand
// 4 = bxc; B = B ^ C, reads but ignores operand
// 5 = out; print combo % 8 (multiple out by commas)
// 6 = bdv; divide;  B = A / 2^combo
// 7 = cdv; divide;  C = A / 2^combo
enum operands {
	adv = 0, bxl = 1, bst = 2, jnz = 3,
	bxc = 4, out = 5, bdv = 6, cdv = 7
};


std::vector<int> simulate(cpu_t &cpu, const std::vector<int> &instructions, size_t max_output = 0) {
	std::vector<int> output;

	bool halt = false;
	while (!halt && cpu.ip < (int)instructions.size()) {
		auto instr = instructions[(size_t)cpu.ip++];
		auto operand = (size_t)instructions[(size_t)cpu.ip++];

		if (verbose > 4) {
			std::cout << cpu.ip-2 << ":" << instr << " " << operand << " : ";
		}

		switch (instr) {
			case adv:
				cpu.a = cpu.a / (1 << get_combo(cpu, operand));
				break;
			case bxl:
				cpu.b = cpu.b ^ operand;
				break;
			case bst:
				cpu.b = get_combo(cpu, operand) % 8;
				break;
			case jnz:
				cpu.ip = (cpu.a == 0) ? cpu.ip : (int)operand;
				break;
			case bxc:
				cpu.b = cpu.b ^ cpu.c;
				break;
			case out:
				output.push_back(get_combo(cpu, operand) % 8);
				if (max_output && output.size() > max_output) {
					halt = true;
				}
				break;
			case bdv:
				cpu.b = cpu.a / (1 << get_combo(cpu, operand));
				break;
			case cdv:
				cpu.c = cpu.a / (1 << get_combo(cpu, operand));
				break;
			default:
				halt = true;
				break;
		}

		if (verbose > 4) {
			std::cout << std::endl;
		}
	}

	return output;
}


long part1(const data_collection_t data) {
	long solution = 0;

	auto [cpu, instructions] = load_program(data);

	if (verbose > 1) {
		std::cout << std::endl;
		std::cout << " Start:[" << cpu << "] " << instructions << std::endl;
	}

	auto output = simulate(cpu, instructions);

	if (verbose > 1) {
		std::cout << "   End:[" << cpu << "] " << instructions << std::endl;
		std::cout << "Output:" << output << std::endl;
	}

	// put answer into a single long for return, concatenate digits.
	for (size_t i = 0; i < output.size(); i++) {
		solution = solution * 10 + output[i];
	}

	return solution;
}

size_t ipow(size_t base, size_t exp) {
    size_t result = 1;
    for (;;) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

size_t set_octal(size_t n, size_t octal, size_t digit) {

	// std::cout << "    n " << std::oct << std::setfill('0') << std::setw(32)<< n << std::endl;

	size_t value_mask = ~(0x7ul << (digit * 3));
	// std::cout << "mask  " << std::oct << std::setfill('0') << std::setw(32) << value_mask << std::endl;

	size_t clean_value = n & value_mask;
	// std::cout << "clean " << std::oct << std::setfill('0') << std::setw(32)<< clean_value << std::endl;

	size_t octal_value = (octal & 0x7ul) << (digit * 3);
	// std::cout << "octal "<< std::oct << std::setfill('0') << std::setw(32) << octal_value << std::endl;

	// std::cout.fill(' ');
	// std::cout << std::dec;
	return clean_value | octal_value;
}

long part2([[maybe_unused]] const data_collection_t data) {
	long solution = 0;

	auto [cpu_initial, instructions] = load_program(data);

	std::vector<size_t> candidates;
	candidates.push_back(0);

	for (int instruction = (int)instructions.size() - 1; instruction >= 0; instruction--) {
		if (verbose > 2) {
			std::cout << "finding digit for " << instruction << "=" << instructions[instruction]
					<< candidates.size() << " candidates: "
					<< std::oct << std::setfill('0') << std::setw(16) << candidates
					<< std::endl;
		}

		std::vector<size_t> new_candidates;
		for (auto candidate : candidates) {
			if (verbose > 3) {
				std::cout << "candidate = " 
						<< std::oct << std::setfill('0') << std::setw(16) << candidate << std::endl;
			}

			for (size_t digit = 0; digit < 8; digit++) {
				auto cpu(cpu_initial);

				solution = set_octal(candidate, digit, instruction);
				cpu.a = solution;

				auto output = simulate(cpu, instructions, instructions.size());

				if (verbose > 3) {
					std::cout << digit << " " 
							<< std::oct << std::setfill('0') << std::setw(16) << solution << std::dec 
							<< ": " << output;
				}

				if (output.size() == instructions.size() && output[instruction] == instructions[instruction]) {
					if (verbose > 3) {
						std::cout << " **";
					}
					new_candidates.push_back(solution);
				}

				if (verbose > 3) {
					std::cout << std::endl;
				}
			}
		}

		candidates.clear();
		candidates.insert(candidates.begin(), new_candidates.begin(), new_candidates.end());
	}

	std::sort(candidates.begin(), candidates.end(), std::greater<size_t>());
	// std::cout << "correct=" << std::oct << 108107572487440 << std::dec << std::endl;
	std::cout << "candidates=" << candidates.size() << ": " << std::dec << candidates << std::endl;
	return candidates[0];
}

// 3045130135122420
// 3000000000000000


// 108107572487424 too low
// 108107572487440
// 108107574778365 << correct
long part2_works([[maybe_unused]] const data_collection_t data) {
	long solution = 0;

	auto [cpu_initial, instructions] = load_program(data);

	// start at 5924266
	// 1: 1,
	// 8: 1,1,
	// 64: 0,1,1,
	// 512: 0,0,1,1,
	// 4096: 0,0,0,1,1,
	// 32768: 0,0,0,0,1,1,
	// 262144: 0,0,0,0,0,1,1,
	// 2097152: 0,0,0,0,0,0,1,1,
	// 16777216: 0,0,0,0,0,0,0,1,1,
	// *8 for each additional digit
	// looking for 16 digits 
	// 35,184,391,701,813
	// 8^15  35,184,372,088,832
	// 8^16 281,474,976,710,656
	// size_t len = 0;
	// size_t last = 0;
	// 105553116266496

	int instruction = (int)instructions.size() - 1; 
	size_t start = ipow(8, instruction);
	size_t increment = start;

	while (instruction >= 0 && solution == 0) {
		size_t check = start;

		if (verbose > 3) {
			std::cout << "instr=" << instruction << "=" << instructions[instruction]
					<< " start=" << start
					<< " check=" << check 
					<< " increment=" << increment 
					<< std::endl;
		}

		for (size_t a = 0; a < 108107572487440; a++) {
			auto cpu(cpu_initial);
			check = start + (increment * a);
			cpu.a = check;

			auto output = simulate(cpu, instructions, instructions.size());
			// std::cout << "\t" << output << std::endl;

			if (instruction == 0) {
				if (output == instructions) {
					if (verbose > 2) {
						std::cout << "found @ " << check << " " << output << std::endl;
					}
					solution = check;
					instruction--;
					break;
					// return check;
				}
			} else if (output[instruction] == instructions[instruction]) {
				while (output[instruction] == instructions[instruction]) {
					if (verbose > 2) {
						std::cout << "Xfound @ " << check << " " << output << std::endl;
					}
					if (instruction <= 0) {
						solution = check;
						break;
					} else {
						start = check;
						increment /= 8;
						instruction--;
					}

				}

				break;
			}
		}
	}

	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return read_lines(path);
}


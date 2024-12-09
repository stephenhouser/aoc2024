#include <iostream>		// cout
#include <iomanip>		// setw and setprecision on output
#include <string>		// std::string
#include <vector>		// std::vector
#include <regex>		// std::regex regular expressions
#include <numeric>		// std::accumulate
#include <chrono>       // high resolution timer

#include "aoc2024.h"
#include "solution.h"

typedef struct file_t_ {
	int id; 
	size_t size;
	size_t block;
} file_t;

/* generate the file and free lists based on data. 
 */
void map_disk(const data_collection_t &data, std::vector<file_t> &files, std::vector<file_t> &free) {
	int fid = 0;
	size_t block = 0;

	for (size_t i = 0; i < data.size(); i += 2) {
		size_t file_size = (size_t)data[i] - '0';
		size_t free_space = (size_t)data[i+1] - '0';

		files.push_back({fid++, file_size, block});
		block += file_size;

		if (i+1 < data.size()) {
			free.push_back({-1, free_space, block});
			block += free_space;
		}
	}
}

void print_disk(std::vector<file_t> files) {
	// sort file_list for printing solution
	std::sort(files.begin(), files.end(), [](file_t &a, file_t &b) {
		return a.block < b.block;
	});

	size_t block = 0;
	for (const auto &file : files) {
		// std::cout << n.id << ":" << n.block << "," << n.size << std::endl;
		while (block < file.block) {
			std::cout << ".";
			block++;
		}

		for (size_t i = 0; i < file.size; i++) {
			std::cout << file.id;
			block++;
		}
	}
	std::cout << std::endl;
}

long part1(const data_collection_t data) {
	long solution = 0;

	// std::cout << data << "\n";

	std::vector<long> sparse_map;

	int fid = 0;
	for (size_t i = 0; i < data.size(); i+=2, fid++) {
		size_t file_size = (size_t)data[i] - '0';
		size_t free_space = (size_t)data[i+1] - '0';

		// std::cout << "file:" << file_size;
		for (size_t n = 0; n < file_size; n++) {
			// std::cout << fid ;
			sparse_map.push_back(fid);
		}

		if (i+1 < data.size()) {
			// std::cout << "free:" << free_space ;
			for (size_t n = 0; n < free_space; n++) {
				// std::cout << '.' ;
				sparse_map.push_back(-1);
			}
		}
	}

	// std::cout << std::endl;
	// for (auto n : sparse_map) {
	// 	if (n >= 0) {
	// 		std::cout << n;
	// 	} else {
	// 		std::cout << ".";
	// 	}
	// }
	// std::cout << std::endl;

	std::vector<long> disk_map;
	size_t front = 0;
	size_t back = sparse_map.size() -1;
	while (front < back ) {
		while (sparse_map[front] != -1) {
			// std::cout << " front push(" << sparse_map[front] << ") ";
			disk_map.push_back(sparse_map[front++]);
		}

		while (sparse_map[back] == -1) {
			// std::cout << " back skip(" << sparse_map[back] << ")";
			back--;
		}

		while (front < back && sparse_map[front] == -1 && sparse_map[back] != -1) {
			// std::cout << " copy(" << sparse_map[front] << "<-" << sparse_map[back] << ")";
			disk_map.push_back(sparse_map[back]);
			back--;
			front++;
		}

		while (front <= back && sparse_map[front] != -1) {
			// std::cout << " front push(" << sparse_map[front] << ") ";
			disk_map.push_back(sparse_map[front++]);
		}
	}

	// for (auto n : disk_map) {
	// 	std::cout << n;
	// }
	// std::cout << std::endl;
	// std::cout << "0099811188827773336446555566..............\n" ;

	for (size_t i = 0; i < disk_map.size(); i++) {
		solution += disk_map[i] * (long)i;
	}

	return solution;
}

long part2([[maybe_unused]] const data_collection_t data) {
	std::vector<file_t> file_list;
	std::vector<file_t> free_list;


	auto start = std::chrono::high_resolution_clock::now();

	map_disk(data, file_list, free_list);

	auto map_time = std::chrono::high_resolution_clock::now();

	if (verbose > 1) { std::cout << std::endl; print_disk(file_list); };

	// using std::for_each...
	// std::for_each(file_list.rbegin(), file_list.rend(), [&free_list](file_t &file) {

	// using reverse iterator
	for (auto it = file_list.rbegin(); it != file_list.rend(); ++it) {
		file_t &file = *it;

		// check free list for a space large enough
		// for (auto &free_f : free_list) {
		for (auto fit = free_list.begin(); fit != free_list.end(); ++fit) {
			file_t &free_f = *fit;

			if (free_f.block < file.block && file.size <= free_f.size) {
				// temp to hold file space we will free up. 
				// (not needed for solution)
				// file_t freed_space = file;

				// update the file's location to the start of the free space
				file.block = free_f.block;

				if (file.size == free_f.size) {
					// uses whole block, remove
					free_list.erase(fit);
				} else {
					// otherwise adjust starting free block and shrink size
					free_f.block += file.size;
					free_f.size -= file.size;
				}

				// add new free space where file used to be
				// (not needed for solution)
				// file_t new_free = {-1, freed_space.size, freed_space.block};
				// free_list.push_back(new_free);
				// should then sort free_list
				break;
			}
		}
	}

	auto defrag_time = std::chrono::high_resolution_clock::now();

	if (verbose > 1) print_disk(file_list);

	// compute checksum, sum of (block * file.id)
	// each file contributes: file_id * (file.size * (2 * file.block + file.size-1) / 2)
	long solution = 0;

	for (const auto &file : file_list) {
		solution += file.id * (((long)file.size*(2 * (long)file.block + (long)file.size - 1)) / 2);
	}

	if (verbose > 1) {
		auto cksum_time = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double, std::milli> ms_double = map_time - start;
		std::cout << "\n";
		std::cout << "   map_time: "
				<< std::setw(10) << std::fixed << std::right 
				<< std::setprecision(4)
				<< ms_double.count() << "ms\n";

		ms_double = defrag_time - map_time;
		std::cout << "defrag_time: "
				<< std::setw(10) << std::fixed << std::right 
				<< std::setprecision(4)
				<< ms_double.count() << "ms\n";

		ms_double = cksum_time - defrag_time;
		std::cout << " cksum_time: "
				<< std::setw(10) << std::fixed << std::right 
				<< std::setprecision(4)
				<< ms_double.count() << "ms\n";
	}
	
	return solution;
}

/* Read data from path and return a vector for each line in the file. */
const data_collection_t read_data(const std::string &path) {
	return read_lines(path)[0];
}


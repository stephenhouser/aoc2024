cmake_minimum_required(VERSION 3.15)
set(CMAKE_TARGET_MESSAGES OFF)

project(day01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -Weffc++ -Wconversion -Wsign-conversion -Werror)

include_directories(. ../aoc2024)
add_subdirectory(../aoc2024 ./aoc2024)

add_executable(solution solution.cpp solution.h)
target_link_libraries(solution aoc2024)

add_custom_target(timing ./solution -vs 11 -g 31 ../test.txt)

# enable_testing()
# add_test(NAME SILVER_TEST
# 		 COMMAND ./solution -vs 11 -g -1 test.txt)

# add_test(NAME GOLD_TEST
# 		 COMMAND ./solution -vs -1 -g 10 test.txt)

set_property(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES
      doc/ CMakeDoxyfile.in install_manifest.txt _CPack_Packages
)

add_custom_target(clean_all
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND find . -name "*.o" -delete
   COMMAND rm CMakeCache.txt
   # after the following being executed, cannot call this target again:
   COMMAND find . -name "*.cmake" -delete
   COMMAND find . -name Makefile -delete
   COMMAND find . -name CMakeFiles | xargs rm -r 
   # need to do this separately
   VERBATIM
)
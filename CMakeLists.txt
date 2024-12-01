cmake_minimum_required(VERSION 3.25)

project(aoc2024
	VERSION 1.0.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

#target_compile_features(flexlib PUBLIC cxx_std_20)
#target_compile_options(flexlib PRIVATE -g -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -fdiagnostics-color=always)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

add_subdirectory(day01)

#enable_testing()
#add_subdirectory(tests)
cmake_minimum_required(VERSION 3.10)

# Set the compiling mode flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

# Set the project name
project(UpperRand)

# Include the current directory for header files
include_directories(include)

# Add the executable
add_executable(upper_rand
src/main.c
src/upperRand.c
src/printArray.c)

# Add any libraries
# target_link_libraries(print_array m)
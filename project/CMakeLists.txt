cmake_minimum_required(VERSION 3.10)

# Set the compiling mode flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

# Set the project name
project(CosineSeries)

# Include the current directory for header files
include_directories(include)

# Add the executable
add_executable(cosine_series
src/main.c
src/cosineSeries.c)

# Add any libraries
target_link_libraries(cosine_series m)
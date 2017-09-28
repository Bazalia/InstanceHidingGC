cmake_minimum_required (VERSION 3.0.1)
project (InstanceHidingGC)

find_package( Boost COMPONENTS program_options )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -DNDEBUG -O3")
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3")

# Allow headers from this directory to be included (like -I command in gcc)
include_directories (${Boost_INCLUDE_DIRS} include/circuit include/garbler include/evaluator include/common)

# Create a variable "sources" that has all source files
file (GLOB sources "src/circuit/*.cpp" "src/garbler/*.cpp" "src/evaluator/*.cpp" "src/common/*.cpp")

# Main target
add_executable(run src/tests/run.cpp ${sources})

# Add linker libraries
target_link_libraries(run ${Boost_LIBRARIES})
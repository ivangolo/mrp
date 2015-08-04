cmake_minimum_required(VERSION 3.2)
project(mrp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(mrp ${SOURCE_FILES} src/utils.hpp src/utils.cpp src/Machine.cpp src/Machine.hpp src/Process.cpp src/Process.hpp src/Resource.cpp src/Resource.hpp src/Service.cpp src/Service.hpp src/Balance.cpp src/Balance.hpp src/sets.hpp src/Instance.cpp src/Instance.hpp src/Solution.cpp src/Solution.hpp src/HillClimbing.cpp src/HillClimbing.hpp src/Greedy.cpp src/Greedy.hpp src/LocalSearch.cpp src/LocalSearch.hpp)

cmake_minimum_required(VERSION 3.2)
project(mrp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(mrp ${SOURCE_FILES} src/utils.h src/utils.cpp src/Machine.cpp src/Machine.h src/Process.cpp src/Process.h src/Resource.cpp src/Resource.h src/Service.cpp src/Service.h src/Balance.cpp src/Balance.h src/sets.h src/Instance.cpp src/Instance.h src/Solution.cpp src/Solution.h src/HillClimbing.cpp src/HillClimbing.h src/Greedy.cpp src/Greedy.h)
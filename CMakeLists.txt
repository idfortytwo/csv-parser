cmake_minimum_required(VERSION 3.17)
project(CSVParser)

set(CMAKE_CXX_STANDARD 14)

include_directories(test include src)

add_executable(CSVParser test/main.cpp src/CSVReader.cpp)
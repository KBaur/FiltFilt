cmake_minimum_required(VERSION 3.16)
project(filtfilt-examples VERSION 0.1)
set (CMAKE_CXX_STANDARD 11)

include_directories(../src/include /usr/local/include)

find_library(FILTFILT filtfilt-lib /usr/local/lib)

add_executable(filtfilt-examples main.cpp)
target_link_libraries(filtfilt-examples PUBLIC ${FILTFILT})
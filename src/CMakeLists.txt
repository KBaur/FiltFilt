cmake_minimum_required(VERSION 3.16)
project(filtfilt VERSION 0.1)
set (CMAKE_CXX_STANDARD 11)

include_directories(./include)

set(H_FILES
    include/Filt.h
    include/FiltFilt.h
)

set(CPP_FILES
    include/Filt.cpp
    include/FiltFilt.cpp
)

add_library(filtfilt-lib ${H_FILES} ${CPP_FILES})
add_executable(filtfilt main.cpp)
target_link_libraries(filtfilt PUBLIC filtfilt-lib)

target_include_directories(filtfilt PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/include"
                          )
install(TARGETS filtfilt-lib DESTINATION /usr/local/lib)
install(FILES ${H_FILES} DESTINATION /usr/local/include)
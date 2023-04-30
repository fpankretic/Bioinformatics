cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 20)
project(Bioinformatics)

include_directories(${CMAKE_SOURCE_DIR}/sdsl-lite/install/include)
link_directories(${CMAKE_SOURCE_DIR}/sdsl-lite/install/lib)

add_executable(Bioinformatics src/main.cpp include/Wavelet.h)

target_link_libraries(Bioinformatics sdsl divsufsort divsufsort64)
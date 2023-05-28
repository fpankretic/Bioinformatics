cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 20)
project(Bioinformatics)

include_directories(${CMAKE_SOURCE_DIR}/sdsl-lite/install/include)
link_directories(${CMAKE_SOURCE_DIR}/sdsl-lite/install/lib)

add_executable(Bioinformatics main.cpp src/Wavelet.cpp src/FMIndex.cpp src/RIndex.cpp)

target_link_libraries(Bioinformatics sdsl divsufsort divsufsort64)

find_package(Catch2 3 REQUIRED)

add_executable(Tests tests/FMIndex_Test.cpp tests/Wavelet_Test.cpp src/Wavelet.cpp src/FMIndex.cpp src/RIndex.cpp tests/RIndex_Test.cpp)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain sdsl divsufsort divsufsort64)
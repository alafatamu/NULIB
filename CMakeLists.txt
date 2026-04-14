cmake_minimum_required(VERSION 3.10)

project(NULIB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ROOT REQUIRED COMPONENTS Tree RIO)

add_library(core
    src/analysis/eventutils.cpp
    src/analysis/detector.cpp
    src/analysis/analysis.cpp
    src/io/evthandler.cpp
    src/io/baseroot.cpp
    src/io/dataref.cpp
    src/proggy/config.cpp
    src/proggy/bitutils.cpp
)

target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(core PUBLIC
    ROOT::Tree
    ROOT::RIO
)

###########################################
add_executable(sifter
    apps/evtsifter.cc
)

target_link_libraries(sifter PRIVATE core)

###########################################
add_executable(processor
    apps/evtprocessor.cc
)

target_link_libraries(processor PRIVATE core)
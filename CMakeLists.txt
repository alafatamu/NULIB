cmake_minimum_required(VERSION 3.10)

project(NULIB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(core
    src/analysis/eventutils.cpp
    src/io/evthandler.cpp
    src/io/dataref.cpp
    src/proggy/bitutils.cpp
    src/proggy/config.cpp
)

target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
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
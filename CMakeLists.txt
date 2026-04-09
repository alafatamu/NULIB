cmake_minimum_required(VERSION 3.10)

project(sifter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    apps/evtsifter.cc
    src/events/eventutils.cpp
    src/io/evthandler.cpp
    src/proggy/bitutils.cpp
)

add_executable(sifter ${SOURCES})

target_include_directories(sifter PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
cmake_minimum_required(VERSION 3.29)
project(order_match_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 3 REQUIRED)

include_directories(include)

# Gather all source files except main.cpp
file(GLOB SOURCES src/*.cpp)
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable(order_match_engine src/main.cpp ${SOURCES})

add_executable(tests tests/tests.cpp ${SOURCES})

target_include_directories(order_match_engine PRIVATE include)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

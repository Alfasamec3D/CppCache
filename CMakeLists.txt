cmake_minimum_required(VERSION 3.11)

project("CACHE" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)

add_executable(cacheIdeal src/cache_ideal.cpp)

add_library(cache src/cache.cpp)

target_include_directories(cache PUBLIC "include")

target_link_libraries (main PRIVATE cache)

target_link_libraries (cacheIdeal PRIVATE cache)

option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
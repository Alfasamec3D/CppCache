cmake_minimum_required(VERSION 3.11)

project("CACHE")

enable_testing()

find_package(GTest REQUIRED)

add_executable(main main.cpp)

add_executable(test_cache tests/test_cache.cpp)

add_executable(cacheIdeal src/cache_ideal.cpp)

add_library(cache src/cache.cpp)

include_directories("include")

target_link_libraries (main cache)

target_link_libraries (test_cache cache gtest gtest_main)
target_link_libraries (cacheIdeal cache)

add_test(NAME CacheTest COMMAND test_cache)
cmake_minimum_required(VERSION 3.11)

project("CACHE")

enable_testing()

add_executable(main main.cpp)

add_executable(test_cache tests/test_cache.cpp)

add_library(cache src/cache.cpp)

include_directories("include")

target_link_libraries (main cache)

target_link_libraries (test_cache cache)

add_test(NAME Test_cache COMMAND test_cache)
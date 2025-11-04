cmake_minimum_required(VERSION 3.11)

project("CACHE")

enable_testing()

add_executable(main main.cpp)

target_include_directories(main PUBLIC "include")
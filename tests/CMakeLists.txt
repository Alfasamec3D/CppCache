enable_testing()

find_package(GTest REQUIRED)

include(GoogleTest)

add_executable(unit_test unit/unit_test.cpp)

target_link_libraries(unit_test PRIVATE cache GTest::gtest_main)

gtest_discover_tests(unit_test)


find_program(BASH_EXECUTABLE bash REQUIRED)
file (GLOB testfiles "e2e/io/data/*.dat")
foreach (file ${testfiles})
    get_filename_component(filename_we ${file} NAME_WE)

    set(ans_main "${CMAKE_SOURCE_DIR}/tests/e2e/io/ans/lfu/${filename_we}.ans")
    set(ans_ideal "${CMAKE_SOURCE_DIR}/tests/e2e/io/ans/ideal/${filename_we}.ans")

    set(lfu_test "lfu_${file}")
    add_test(NAME ${lfu_test} 
        COMMAND bash -c "${CMAKE_SOURCE_DIR}/tests/e2e/run.sh ${file} ./main ${ans_main}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        
    )
    set_tests_properties(${lfu_test} PROPERTIES DEPENDS main)

    set(ideal_test "ideal_${file}")
    add_test(NAME ${ideal_test} 
        COMMAND bash -c "${CMAKE_SOURCE_DIR}/tests/e2e/run.sh ${file} ./cacheIdeal ${ans_ideal}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        
    )
    set_tests_properties(${ideal_test} PROPERTIES DEPENDS cacheIdeal)
endforeach()
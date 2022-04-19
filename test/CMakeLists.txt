cmake_minimum_required(VERSION 3.10)

add_executable(tests ../test/test.cpp)

#add_dependencies(tests nb_lib)
#
target_link_libraries(tests PRIVATE nb_lib GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main unofficial::sqlite3::sqlite3)

find_package(GTest CONFIG REQUIRED)
message("GTest_DIR: ${GTest_DIR}")
target_link_libraries(tests PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(tests)

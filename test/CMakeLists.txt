cmake_minimum_required(VERSION 3.10)

add_executable(tests ../test/test.cpp net_test.cpp)

#add_dependencies(tests nb_lib)

target_link_libraries(tests PRIVATE nb_lib nb_lib_db nb_lib_net GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main unofficial::sqlite3::sqlite3)

find_package(GTest CONFIG REQUIRED)
message("GTest_DIR: ${GTest_DIR}")
target_link_libraries(tests PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(tests)

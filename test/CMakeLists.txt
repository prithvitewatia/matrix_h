cmake_minimum_required(VERSION 2.6)

project(TEST)
# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# link and run
add_executable(runTests test_matrix.cpp ../matrix.c)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

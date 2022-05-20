cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD 17)

project(matrix_library)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link test code with code we want to test and the GTest and pthread library
#add_executable(test_day03_part2 test_day03_part2.cpp day03_part2_functions.cpp)
#add_executable(day03_part2 day03_part2.cpp day03_part2_functions.cpp)
add_executable(test_main test_main.cpp)
add_executable(main main.cpp)

#target_link_libraries(test_day03_part2 ${GTEST_LIBRARIES} pthread)
#target_link_libraries(day03_part2)
target_link_libraries(test_main ${GTEST_LIBRARIES} pthread)
target_link_libraries(main ${GTEST_LIBRARIES} pthread)


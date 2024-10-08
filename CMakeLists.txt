cmake_minimum_required(VERSION 3.10)
project(Opportunistic-Trading-System-Algo)

# turn off auto type warnings
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(mergeLib)
add_subdirectory(mergeApp)
add_subdirectory( test )
# Add the test to CTest
enable_testing()
add_test(NAME simple_test COMMAND simple_test)



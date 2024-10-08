
add_library(mergeLib src/mergefiles.cpp)
 
message("CalcLib current source dir = ${CMAKE_CURRENT_SOURCE_DIR}")
 
target_include_directories( mergeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

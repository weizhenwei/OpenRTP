message(STATUS "Entering directory ${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(foo)
add_subdirectory(openrtp)

message(STATUS "Leaving directory ${CMAKE_CURRENT_SOURCE_DIR}")


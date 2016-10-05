message(STATUS "Entering directory ${CMAKE_CURRENT_SOURCE_DIR}")


file(GLOB EXAMPLE_FILES *.h *.cpp)

# library include and link
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/util)
link_directories(${PROJECT_SOURCE_DIR}/src)
link_directories(${PROJECT_SOURCE_DIR}/src/util)

add_executable(OpenRTP ${EXAMPLE_FILES})
target_link_libraries(OpenRTP glog openrtp pthread) 

message(STATUS "Leaving directory ${CMAKE_CURRENT_SOURCE_DIR}")


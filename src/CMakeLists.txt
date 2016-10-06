message(STATUS "Entering directory ${CMAKE_CURRENT_SOURCE_DIR}")

# detect platform;
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(LINUX_PLATFORM TRUE)
    endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    # for eliminating the macosx_rpath warning;
    set(CMAKE_MACOSX_RPATH 1)

    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(MAC_PLATFORM TRUE)
    endif()
endif()

if(NOT LINUX_PLATFORM AND NOT MAC_PLATFORM)
    message(FATAL_ERROR "Only Linux-x86_64 and Darwin-x86_64 platform supported")
endif()

if(LINUX_PLATFORM)
    include_directories(${PROJECT_SOURCE_DIR}/vendor/glog/linux-x86_64/include)
    link_directories(${PROJECT_SOURCE_DIR}/vendor/glog/linux-x86_64/lib)
elseif(MAC_PLATFORM)    
    include_directories(${PROJECT_SOURCE_DIR}/vendor/glog/mac-x86_64/include)
    link_directories(${PROJECT_SOURCE_DIR}/vendor/glog/mac-x86_64/lib)
endif()

# util library
add_subdirectory(util)


# openrtp library
include_directories(${PROJECT_SOURCE_DIR}/src/util)
link_directories(${PROJECT_SOURCE_DIR}/src/util)
include_directories(${PROJECT_SOURCE_DIR}/src)
link_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB UNIVERSAL_FILES *.h *.cpp)
set(OPENRTP_FILES ${UNIVERSAL_FILES})
add_library(openrtp SHARED ${OPENRTP_FILES})
set_target_properties(openrtp PROPERTIES OUTPUT_NAME "openrtp")
target_link_libraries(openrtp util)


# example(main entry);
add_subdirectory(example)

message(STATUS "Leaving directory ${CMAKE_CURRENT_SOURCE_DIR}")


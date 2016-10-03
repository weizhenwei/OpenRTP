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

# include directory;
# include_directories(${PROJECT_SOURCE_DIR}/src/include)
# include_directories(${PROJECT_SOURCE_DIR}/src/capture)
# include_directories(${PROJECT_SOURCE_DIR}/src/util)
# include_directories(${PROJECT_SOURCE_DIR}/src/main)

if(LINUX_PLATFORM)
    include_directories(${PROJECT_SOURCE_DIR}/vendor/glog/linux-x86_64/include)
    link_directories(${PROJECT_SOURCE_DIR}/vendor/glog/linux-x86_64/lib)
elseif(MAC_PLATFORM)    
    include_directories(${PROJECT_SOURCE_DIR}/vendor/glog/mac-x86_64/include)
    link_directories(${PROJECT_SOURCE_DIR}/vendor/glog/mac-x86_64/lib)
endif()

# main source files;
# file(GLOB UNIVERSAL_FILES main/*.h main/*.cpp main/client/*.h main/client/*.cpp
#     main/server/*.h main/server/*.cpp)
file(GLOB UNIVERSAL_FILES main.h main.cpp)
add_executable(OpenRTP ${UNIVERSAL_FILES})

# libraries
# add_subdirectory(capture)
# add_subdirectory(util)
# link_directories(${PROJECT_SOURCE_DIR}/src/capture)
# link_directories(${PROJECT_SOURCE_DIR}/src/util)
# target_link_libraries(OpenRTP glog capture util pthread ${PLATFORM_LIB}) 
target_link_libraries(OpenRTP glog pthread ${PLATFORM_LIB}) 

message(STATUS "Leaving directory ${CMAKE_CURRENT_SOURCE_DIR}")


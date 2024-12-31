cmake_minimum_required(VERSION 3.0)
project(project)

set(CMAKE_CXX_STANDARD "11" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DRAPIDJSON_HAS_STDSTRING=1")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(POLICY CMP0063)
  cmake_policy(SET CMP0063 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

include_directories(libs/milo)
include_directories(libs/rapidjson/include)
include_directories(libs/ecf)
include_directories(algorithm)

add_subdirectory(simulator)
# add_subdirectory(algorithm)
add_subdirectory(libs/ecf)

set(SOURCE_FILES algorithm/main.cpp)

foreach(SRC_PATH ${SOURCE_FILES})
    get_filename_component(SRC_NAME ${SRC_PATH} NAME)
    string(REPLACE ".cpp" "" EXE_NAME ${SRC_NAME})
    add_executable(${EXE_NAME} ${SRC_PATH})
    set_target_properties(${EXE_NAME} PROPERTIES CXX_VISIBILITY_PRESET "hidden")
    target_link_libraries(${EXE_NAME} PRIVATE ${PROJECT_LIB_NAME})
    target_link_libraries(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/build/libs/debug/libECF.a)
endforeach()

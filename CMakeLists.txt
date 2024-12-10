cmake_minimum_required(VERSION 3.25)
project(Carto)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(third_party)

cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

add_executable(carto ${SOURCES})

target_include_directories(carto PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
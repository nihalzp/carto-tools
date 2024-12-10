cmake_minimum_required(VERSION 3.25)
project(Carto LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/*.cpp)

add_executable(carto)

target_sources(carto PRIVATE ${SOURCES})

target_include_directories(carto
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${Boost_INCLUDE_DIRS}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(carto PRIVATE
    -Wall -Wextra -Wpedantic
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(carto PRIVATE
    /W4
  )
endif()

install(TARGETS carto DESTINATION /usr/local/bin)

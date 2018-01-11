cmake_minimum_required(VERSION 3.0)
project(PriLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14 -DPRILIB=${PRILIB}")

include_directories(include)
aux_source_directory(source PRILIB_SOURCE_FILES)
aux_source_directory(include PRILIB_INCLUDE_FILES)

add_library(prilib STATIC ${PRILIB_INCLUDE_FILES} ${PRILIB_SOURCE_FILES})

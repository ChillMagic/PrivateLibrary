cmake_minimum_required(VERSION 3.0)
project(PriLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14")

message("")
message("Operating System : ${CMAKE_SYSTEM_NAME}")
message("Processor : ${CMAKE_SYSTEM_PROCESSOR}")
message("")

if (PRILIB)
  message("Find PriLib Macro.")
  message("The PriLib name is ${PRILIB}")
  add_definitions(-DPRILIB=${PRILIB})
else (PRILIB)
  message("Unfind PriLib Macro.")
endif (PRILIB)

include_directories(include)
aux_source_directory(source PRILIB_SOURCE_FILES)

add_library(prilib STATIC ${PRILIB_SOURCE_FILES} include)

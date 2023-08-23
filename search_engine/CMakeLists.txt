set (CMAKE_CXX_STANDARD 20)

cmake_minimum_required (VERSION 3.8)

project ("search_engine")

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(nlohmann_json)

add_subdirectory(src)

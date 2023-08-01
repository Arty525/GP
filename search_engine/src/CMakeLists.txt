cmake_minimum_required (VERSION 3.8)

project ("search_engine")

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_executable (search_engine "search_engine.cpp" "${INCLUDE_DIR}/search_engine.h")

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

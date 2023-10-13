set (CMAKE_CXX_STANDARD 20)

set (CMAKE_CXX_EXTENSIONS OFF)

cmake_minimum_required (VERSION 3.20)

add_executable (search_engine "search_engine.cpp" "ConvertJSON.cpp" "inverted_index.cpp" "${INCLUDE_DIR}/search_engine.h" "../include/inverted_index.h" "${INCLUDE_DIR}/convertJSON.h")

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(search_engine INTERFACE gtest_main)

add_library (GTest::GTest INTERFACE IMPORTED)

target_link_libraries(search_engine PRIVATE gtest_main)

gtest_discover_tests(search_engine)

add_test (se_gtests search_engine)
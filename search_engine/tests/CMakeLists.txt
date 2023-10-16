set (CMAKE_CXX_STANDARD 20)

cmake_minimum_required (VERSION 3.20)

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(search_engine INTERFACE gtest_main)

add_library (GTest::GTest INTERFACE IMPORTED)

target_link_libraries(search_engine PRIVATE gtest_main)

add_test (se_gtests tests)

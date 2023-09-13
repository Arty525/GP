set (CMAKE_CXX_STANDARD 20)

cmake_minimum_required (VERSION 3.8)

add_executable (search_engine "search_engine.cpp" "ConvertJSON.cpp" "InvertedIndex.cpp" "${INCLUDE_DIR}/search_engine.h" "${INCLUDE_DIR}/InvertedIndex.h" "${INCLUDE_DIR}/convertJSON.h")

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

set (CMAKE_CXX_STANDARD 20)

set (CMAKE_CXX_EXTENSIONS OFF)

cmake_minimum_required (VERSION 3.20)

project ("search_engine")

enable_testing()

include(FetchContent)
FetchContent_Declare(
	googletest
	URL
	https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

FetchContent_Declare(
	nlohmann_json
	URL
	https://github.com/nlohmann/json/archive/refs/heads/develop.zip
)

FetchContent_MakeAvailable(googletest)

FetchContent_MakeAvailable(nlohmann_json)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(${nlohmann_json_SOURCE_DIR}/include ${nlohmann_json_SOURCE_DIR})

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(GoogleTest)

add_subdirectory(src)

add_subdirectory(tests)

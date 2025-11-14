cmake_minimum_required(VERSION 3.14)
project(MyVectorProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_library(vector_lib INTERFACE)

target_include_directories(vector_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(vector_tests
    tests/vector_tests.cpp
)

target_link_libraries(vector_tests
    PRIVATE
        vector_lib
        gtest_main
)

enable_testing()
add_test(NAME VectorTests COMMAND vector_tests)

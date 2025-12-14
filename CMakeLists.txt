cmake_minimum_required(VERSION 3.50)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

include_directories(${gtest_SOURCE_DIR}/include${gtest_SOURCE_DIR})

set(gtest disable pthreadson)

set(gtest force shared crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(SearchEngine
    main.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)

target_include_directories(SearchEngine PRIVATE
    include
)

target_compile_options(SearchEngine PRIVATE -static)

add_executable(tests 
    tests.cpp 
    InvertedIndex.cpp 
    SearchServer.cpp
    ConverterJSON.cpp
)

target_include_directories(tests PRIVATE 
    include
    ${gtest_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
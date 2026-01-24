cmake_minimum_required(VERSION 3.80)

# get JSON from github
include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.12.0
        )
FetchContent_MakeAvailable(json)

configure_file(${CMAKE_SOURCE_DIR}/config/config.json ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/config/request.json ${CMAKE_CURRENT_BINARY_DIR})


add_executable(search_server main.cpp converter.cpp inverted_index.cpp server.cpp)

target_include_directories(search_server PUBLIC  ${INCLUDE_DIR})

target_link_libraries(search_server PRIVATE nlohmann_json::nlohmann_json)

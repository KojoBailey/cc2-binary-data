cmake_minimum_required(VERSION 3.19)
project(cc2_binary_data VERSION 0.1.0 DESCRIPTION "CyberConnect2 Binary Data Library")
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
find_package(nlohmann_json QUIET)
if (NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found, using submodule in extern/json")
    add_subdirectory(extern/json)
endif()

if (NOT TARGET kojo::nucc)
    add_subdirectory(extern/nucc)
endif()

file(GLOB SOURCES 
    src/asbr/message_info.cpp
)
add_library(cc2_binary_data STATIC ${SOURCES})
target_include_directories(cc2_binary_data PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC  
        kojo::nucc
        nlohmann_json
)

add_library(kojo::cc2_binary_data ALIAS cc2_binary_data)

message(STATUS "SOURCES = ${SOURCES}")
cmake_minimum_required(VERSION 3.20)
project(hel VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.cpp
        src/token/Token.h
        src/token/Tokenizer.h
        src/token/Tokenizer.cpp
        src/parsing/Parser.cpp
        src/parsing/Parser.h
        src/parsing/Node.h
        src/generation/Generator.cpp
        src/generation/Generator.h)

target_include_directories(${PROJECT_NAME} PRIVATE src)


# Compile definitions ==================================================================================
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE __DEBUG__)
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE __RELEASE__ PRIVATE __NDEBUG__)
endif ()
# =====================================================================================================
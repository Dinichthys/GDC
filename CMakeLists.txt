cmake_minimum_required (VERSION 3.20)
project (GiantDad CXX)

# -----------------------------
# General settings
# -----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(TEST_EXEC_NAME "${PROJECT_NAME}Tests")

# -----------------------------
# CPM Setup
# -----------------------------
include(cmake/CPM.cmake) # <-- download CPM.cmake once into cmake/

# -----------------------------
# Dependencies via CPM
# -----------------------------

CPMAddPackage(
    NAME CLI11
    GITHUB_REPOSITORY CLIUtils/CLI11
    GIT_TAG v2.3.2
)

CPMAddPackage(
    NAME onnx_proto_only
    GITHUB_REPOSITORY onnx/onnx
    GIT_TAG v1.15.0
    DOWNLOAD_ONLY TRUE
)


# -----------------------------
# Protobuf generation (ONNX)
# -----------------------------

find_package(Protobuf REQUIRED)

set(ONNX_PROTO_ROOT
    ${onnx_proto_only_SOURCE_DIR}
)

set(ONNX_PROTO_FILE
    ${ONNX_PROTO_ROOT}/onnx/onnx.proto
)

set(ONNX_GENERATED_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

file(MAKE_DIRECTORY ${ONNX_GENERATED_DIR})

protobuf_generate(
    LANGUAGE cpp
    PROTOS ${ONNX_PROTO_FILE}
    IMPORT_DIRS ${ONNX_PROTO_ROOT}
    PROTOC_OUT_DIR ${ONNX_GENERATED_DIR}
    APPEND_PATH
    OUT_VAR ONNX_SRCS
)

add_library(onnx_proto STATIC ${ONNX_SRCS})

target_link_libraries(onnx_proto
    PUBLIC protobuf::libprotobuf
)

target_include_directories(onnx_proto
    PUBLIC ${ONNX_GENERATED_DIR}
)

# -----------------------------
# Compiler options
# -----------------------------
set(COMMON_COMPILE_OPTIONS
    -fdiagnostics-color=always
    -Wall
    -Wextra
    -fstack-protector-strong
    -fcheck-new
    -fstrict-overflow
    -march=native
)

set(DEBUG_COMPILE_OPTIONS
    -Og
    -g3
    -DDEBUG
    -ggdb
    -fsanitize=address,leak,undefined
)

set(RELEASE_COMPILE_OPTIONS
    -O2
    -DNDEBUG
)

# -----------------------------
# Compiler executable
# -----------------------------

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/graph/GraphBuilder.cpp
    src/graph/Value.cpp
    src/graph/DumpGraphViz.cpp
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
    CMAKE_CXX_CLANG_TIDY "clang-tidy"
)

target_include_directories(${PROJECT_NAME}
    SYSTEM PRIVATE
    external/onnx
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        ${COMMON_COMPILE_OPTIONS}
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    CLI11::CLI11
    onnx_proto
)

# -----------------------------
# Catch2 test executable
# -----------------------------

CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    GIT_TAG v3.5.2
)

add_executable(${TEST_EXEC_NAME}
    tests/Test.cpp
)

set_target_properties(${TEST_EXEC_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
    CMAKE_CXX_CLANG_TIDY "clang-tidy"
)

target_include_directories(${TEST_EXEC_NAME}
    SYSTEM PRIVATE
    external/onnx
)

target_include_directories(${TEST_EXEC_NAME}
    PRIVATE include
)

target_link_libraries(${TEST_EXEC_NAME}
    PRIVATE Catch2::Catch2WithMain
    CLI11::CLI11
    onnx_proto
)

target_compile_options(${TEST_EXEC_NAME}
    PRIVATE
        ${COMMON_COMPILE_OPTIONS}
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_link_options(${TEST_EXEC_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

# -----------------------------
# Enable testing
# -----------------------------
enable_testing()
add_test(NAME UnitTests COMMAND ${TEST_EXEC_NAME})

# cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++
# cmake --build build

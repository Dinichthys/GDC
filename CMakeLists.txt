cmake_minimum_required (VERSION 3.20)
project (GiantDad CXX)

# -----------------------------
# General settings
# -----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(TEST_EXEC_NAME "${PROJECT_NAME}Tests")

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

find_package(Protobuf REQUIRED)

set(ONNX_PROTO_DIR
    ${CMAKE_SOURCE_DIR}/external/onnx/onnx
)

set(ONNX_PROTO_FILE
    ${ONNX_PROTO_DIR}/onnx.proto
)

protobuf_generate_cpp(
    ONNX_PROTO_SRCS
    ONNX_PROTO_HDRS
    ${ONNX_PROTO_FILE}
)

set(ONNX_PROTO_EXEC onnx_proto)

add_library(${ONNX_PROTO_EXEC}
    ${ONNX_PROTO_SRCS}
)

target_include_directories(${ONNX_PROTO_EXEC}
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${ONNX_PROTO_DIR}
)

target_link_libraries(${ONNX_PROTO_EXEC}
    PUBLIC
        protobuf::libprotobuf
)

# -----------------------------
# CLI11
# -----------------------------

add_subdirectory(external/CLI11)

# -----------------------------
# Compiler executable
# -----------------------------

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/graph/GraphBuilder.cpp
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
        -march=native
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    CLI11::CLI11
    ${ONNX_PROTO_EXEC}
)

# -----------------------------
# Catch2 test executable
# -----------------------------
find_package(Catch2 3 REQUIRED)

add_executable(${TEST_EXEC_NAME}
    src/main.cpp
    src/graph/GraphBuilder.cpp

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
    ${ONNX_PROTO_EXEC}
)

target_compile_options(${TEST_EXEC_NAME}
    PRIVATE
        ${COMMON_COMPILE_OPTIONS}
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_link_options(${TEST_EXEC_NAME}
    PRIVATE
        -march=native
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

# -----------------------------
# Enable testing
# -----------------------------
enable_testing()
add_test(NAME UnitTests COMMAND ${TEST_EXEC_NAME})

# cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang
# cmake --build build

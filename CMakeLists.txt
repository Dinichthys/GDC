cmake_minimum_required (VERSION 3.20)
project (GiantDad CXX)

# -----------------------------
# General settings
# -----------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
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

# -----------------------------
# Compiler executable
# -----------------------------

add_executable(${PROJECT_NAME}
    src/main.cpp
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
)

# -----------------------------
# Catch2 test executable
# -----------------------------
find_package(Catch2 3 REQUIRED)

add_executable(${TEST_EXEC_NAME}
    src/main.cpp
)

target_include_directories(${TEST_EXEC_NAME}
    PRIVATE include
)

target_link_libraries(${TEST_EXEC_NAME}
    PRIVATE Catch2::Catch2WithMain
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

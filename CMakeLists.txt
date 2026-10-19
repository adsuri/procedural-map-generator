cmake_minimum_required(VERSION 3.16)

project(ProceduralMapGenerator LANGUAGES CXX)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "This project requires GCC (g++).")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/main.cpp
    src/NoiseGenerator.cpp
    src/TerrainGenerator.cpp
    src/util.cpp
)


include_directories(${PROJECT_SOURCE_DIR}/include)

# --------------------
# Release executable
# --------------------
add_executable(mapgen ${SOURCES})

target_compile_options(mapgen PRIVATE
    -O3
    -Wall
)

# --------------------
# Debug executable
# --------------------
add_executable(mapgen_debug ${SOURCES})

target_compile_options(mapgen_debug PRIVATE
    -O0
    -g3
    -Wall
    -Wextra
    -Wpedantic
    -D_GLIBCXX_ASSERTIONS
)

add_custom_target(release DEPENDS mapgen)
add_custom_target(debug DEPENDS mapgen_debug)

cmake_minimum_required(VERSION 3.30)
project(RayTracing)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

add_subdirectory(external/stb)

add_executable(
        RayTracing
        ${SRC}
        ${HEADERS}
)


target_include_directories(
        RayTracing
        PUBLIC
        external/stb
)

target_link_libraries(
        RayTracing
        PUBLIC
        stb
)

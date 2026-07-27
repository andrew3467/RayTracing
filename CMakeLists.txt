cmake_minimum_required(VERSION 3.30)
project(RayTracing LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_ARCHITECTURES 89)

find_package(CUDAToolkit REQUIRED)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE HEADERS src/*.cu)

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
        CUDA::cudart
)

cmake_minimum_required(VERSION 3.16)
project(card-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(card-engine ${SOURCES})
target_link_libraries(card-engine raylib)

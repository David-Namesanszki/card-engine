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

# ---- Tests ----
option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
        GIT_SHALLOW TRUE
    )
    # Prevent GoogleTest from overriding parent project's compiler/linker settings on Windows.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(card-engine-tests
        tests/transformComponentTest.cpp
        src/engine/components/transformComponent.cpp
    )
    target_link_libraries(card-engine-tests PRIVATE raylib GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(card-engine-tests)
endif()

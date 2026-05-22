cmake_minimum_required(VERSION 3.15...4.3)

project(FlecsTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakePrintHelpers)

include(FetchContent)


#if defined(__linux__)
    set(sep "\\")
#else
    set(sep "\/")

set(CD "${CMAKE_CURRENT_SOURCE_DIR}")
set(SRC "src")

FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG master
)
FetchContent_MakeAvailable(flecs)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries (${PROJECT_NAME} PUBLIC
    flecs::flecs_static
)


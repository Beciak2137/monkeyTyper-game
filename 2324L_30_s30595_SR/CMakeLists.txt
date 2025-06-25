cmake_minimum_required(VERSION 3.26)
project(projekt_v3)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(SFML)

FetchContent_MakeAvailable(fmt)

add_executable(projekt_v3 main.cpp)
target_link_libraries(projekt_v3 sfml-graphics)
target_link_libraries(projekt_v3 fmt)

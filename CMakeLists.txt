cmake_minimum_required(VERSION 3.13)
project(GameEngine)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

include_directories( Source/)

file(GLOB_RECURSE helloworld_SRC
        "Source/*.h"
        "Source/*.cpp"
        )

add_executable(BezNazwy ${helloworld_SRC})
target_include_directories(BezNazwy PUBLIC )
target_link_libraries(BezNazwy)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x -Wall -Wformat-security -ggdb")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "cmake")

project(GhostlyJones)

find_package(SDL2 REQUIRED)

add_definitions(-DSPICE_INPUT_ENABLED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} "spiceframework/include" "game/include")


file(GLOB_RECURSE SOURCE
    "game/include/*.h"
    "game/src/*.c"
    "spiceframework/include/*.h"
    "spiceframework/src/*.c"
)

add_executable(GhostlyJones ${SOURCE})
target_include_directories(GhostlyJones PUBLIC include)
target_include_directories(GhostlyJones PUBLIC "spiceframework/include")
target_link_libraries(GhostlyJones PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} m GL)
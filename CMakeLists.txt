cmake_minimum_required(VERSION 3.2)
project(MyGame VERSION 1.0.0 LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED OFF)
# set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "" FORCE)
set(FASTNOISE2_TESTS OFF CACHE BOOL "" FORCE)

add_library(game_lib STATIC
    src/engine/traits/positionable.cpp
    src/engine/camera.cpp
    src/engine/object.cpp
    src/engine/collision.cpp
    src/engine/collision_box.cpp
    src/engine/collision_sphere.cpp
    src/engine/frustum.cpp
    src/engine/gravity.cpp
    src/engine/object.cpp
    src/engine/object/block.cpp
    src/engine/player.cpp
    src/engine/shader.cpp
    src/engine/world.cpp
    src/utils/imageBMP.cpp
    src/utils/model.cpp
)

target_include_directories(game_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_compile_features(game_lib PUBLIC cxx_std_20)

# Fetches libs

include(FetchContent)

FetchContent_Declare(
    FastNoise2
    GIT_REPOSITORY https://github.com/Auburn/FastNoise2.git
    GIT_TAG v1.0.0
)
FetchContent_Declare(
    ASSIMP
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.4
)

FetchContent_MakeAvailable(
    FastNoise2
    ASSIMP
)

#Link all libs

target_link_libraries(game_lib PUBLIC 
    FastNoise
    assimp
)

# Game app 
add_executable(game_app
    src/main.cpp
)

target_link_libraries(game_app PRIVATE game_lib)

target_compile_features(game_app PUBLIC cxx_std_20)

target_link_libraries(game_app PRIVATE 
    GL
    glfw
    GLEW
)

cmake_minimum_required(VERSION 3.16)
project(TravelInSpace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES
    Src/Main.cpp
    Src/Engine/Core/Game.cpp
    Src/Engine/Core/Actor.cpp
    Src/Engine/Core/Component.cpp
    Src/Engine/Core/MoveComponent.cpp
    Src/Engine/Core/InputComponent.cpp
    Src/Engine/Core/CircleComponent.cpp
    Src/Engine/Renderer/SpriteComponent.cpp
    Src/Engine/Renderer/AnimSpriteComponent.cpp
    Src/Engine/Renderer/BGSpriteComponent.cpp
    Src/Engine/Utils/Math.cpp
    Src/Engine/Utils/Random.cpp
    Src/Game/Asteroid.cpp
    Src/Game/Laser.cpp
    Src/Game/Ship.cpp
)

set(SDL2_DIR D:/SDL2)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    Src/Engine/Core
    Src/Engine/Renderer
    Src/Engine/Utils
    Src/Game
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image)

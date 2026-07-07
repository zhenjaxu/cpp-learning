cmake_minimum_required(VERSION 3.16)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES
    Src/Main.cpp
    Src/Game/Bullet.cpp
    Src/Game/Enemy.cpp
    Src/Game/Grid.cpp
    Src/Game/Tile.cpp
    Src/Game/Tower.cpp
    Src/Engine/Core/Game.cpp
    Src/Engine/Core/Actor.cpp
    Src/Engine/Core/Component.cpp
    Src/Engine/Core/MoveComponent.cpp
    Src/Engine/Core/CircleComponent.cpp
    Src/Engine/Renderer/SpriteComponent.cpp
    Src/Engine/AI/AIState.cpp
    Src/Engine/AI/AIComponent.cpp
    Src/Engine/AI/NavComponent.cpp
    Src/Engine/Utils/Math.cpp
)

set(SDL2_DIR D:/SDL2)   # SDL2路径

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    Src/Engine/Core
    Src/Engine/Renderer
    Src/Engine/Utils
    Src/Engine/AI
    Src/Game
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/include)     # 包含SDL2.h、SDL2_image.h
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/lib)    # 包含SDL2、SDL2_image库
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image)

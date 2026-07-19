cmake_minimum_required(VERSION 3.16)
project(TravelInSpaceOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES
    Src/Main.cpp
    Src/Engine/Game.cpp
    Src/Engine/Actor.cpp
    Src/Engine/Component.cpp
    Src/Engine/MoveComponent.cpp
    Src/Engine/InputComponent.cpp
    Src/Engine/CircleComponent.cpp
    Src/Engine/Renderer/SpriteComponent.cpp
    Src/Engine/Renderer/Shader.cpp
    Src/Engine/Renderer/Texture.cpp
    Src/Engine/Renderer/VertexArray.cpp
    Src/Engine/Utils/Math.cpp
    Src/Engine/Utils/Random.cpp
    Src/Game/Asteroid.cpp
    Src/Game/Laser.cpp
    Src/Game/Ship.cpp
)

set(SDL2_DIR D:/SDL2)
set(GLEW_DIR D:/GLEW)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    Src/Engine
    Src/Engine/Renderer
    Src/Engine/Utils
    Src/Game
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/lib)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${GLEW_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 glew32 opengl32)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "D:/SDL2/bin/SDL2.dll"
    "D:/GLEW/bin/glew32.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
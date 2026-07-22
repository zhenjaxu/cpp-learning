cmake_minimum_required(VERSION 3.16)
project(Pong3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES
    Src/Main.cpp
    Src/Engine/Core/Game.cpp
    Src/Engine/Core/Actor.cpp
    Src/Engine/Core/CameraActor.cpp
    Src/Engine/Core/PlaneActor.cpp
    Src/Engine/Core/Component.cpp
    Src/Engine/Core/MoveComponent.cpp
    Src/Engine/Core/CircleComponent.cpp
    Src/Engine/Renderer/Renderer.cpp
    Src/Engine/Renderer/Mesh.cpp
    Src/Engine/Renderer/Shader.cpp
    Src/Engine/Renderer/Texture.cpp
    Src/Engine/Renderer/VertexArray.cpp
    Src/Engine/Renderer/MeshComponent.cpp
    Src/Engine/Renderer/SpriteComponent.cpp
    Src/Engine/Utils/Math.cpp
    Src/Engine/Utils/Random.cpp
)

set(SDL2_DIR D:/SDL2)
set(GLEW_DIR D:/GLEW)
set(JSON_DIR D:/rapidjson1.1)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    Src/Engine/Core
    Src/Engine/Renderer
    Src/Engine/Utils
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${GLEW_DIR}/lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${JSON_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 glew32 opengl32)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "D:/SDL2/bin/SDL2.dll"
    "D:/GLEW/bin/glew32.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
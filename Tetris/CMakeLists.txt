cmake_minimum_required(VERSION 3.10)
project(tetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES
    Src/Main.cpp
    Src/Game/Game.cpp
    Src/Game/Board.cpp
    Src/Game/Piece.cpp
)

set(SDL2_DIR D:/SDL2)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE Src/Game)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "D:/SDL2/bin/SDL2.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
cmake_minimum_required(VERSION 3.10)
project(BlankWindow)

set(CMAKE_CXX_STANDARD 17)
set(SDL2_DIR "D:/SDL2")

add_executable(game main.cpp)

target_include_directories(game PRIVATE ${SDL2_DIR}/include)
target_link_directories(game PRIVATE ${SDL2_DIR}/lib)
target_link_libraries(game PRIVATE SDL2)
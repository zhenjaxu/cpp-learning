cmake_minimum_required(VERSION 3.10)
project(CppGameDemo)

set(CMAKE_CXX_STANDARD 17)

# SDL2 路径
set(SDL2_INCLUDE_DIR "D:/SDL2/include")
set(SDL2_LIB_DIR "D:/SDL2/lib")

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(game main.cpp)

# Windows MinGW 链接顺序: SDL2main, SDL2
target_link_libraries(game SDL2main SDL2)
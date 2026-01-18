cmake_minimum_required(VERSION 3.28)
project(HomeWorkProgram1)

set(CMAKE_CXX_STANDARD 17)


add_executable(Program2
        Program2/Test.cpp
        Program2/Trees.h
)
# 添加头文件搜索路径
target_include_directories(Program2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

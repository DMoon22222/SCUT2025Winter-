cmake_minimum_required(VERSION 3.28)
project(HomeWorkProgram1)

set(CMAKE_CXX_STANDARD 17)

# Program1 executable
add_executable(ArrayBasedListTest
        Program1/MyAList.cpp
        Program1/Test.cpp
)

target_include_directories(ArrayBasedListTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Program2 executable
add_executable(BookTest
        Program2/Book.cpp
        Program2/BookManager.cpp
        Program1/MyAList.cpp
        Program2/BookTest.cpp
)

target_include_directories(BookTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
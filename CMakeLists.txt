cmake_minimum_required(VERSION 3.6)
project(oneAnotherAlignment)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp SWAlignment.cpp ScoreMatrix.cpp FileManager.cpp)
add_executable(courseAlignment ${SOURCE_FILES})
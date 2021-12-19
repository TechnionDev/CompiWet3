cmake_minimum_required(VERSION 3.15)
project(CompiWet3)

set(CMAKE_CXX_STANDARD 17)

add_executable(CompiWet3 main.cpp hw3_output.cpp hw3_output.hpp Semantics.cpp Semantics.h SymbolTable.h TokensClasses.h)
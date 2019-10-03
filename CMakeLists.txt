cmake_minimum_required(VERSION 3.10)
project(Project_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project_1 Project1.cpp LexicalAnalyzer.cpp LexicalAnalyzer.h SetLimits.cpp SetLimits.h SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h)
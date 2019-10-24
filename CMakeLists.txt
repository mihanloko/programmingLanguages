cmake_minimum_required(VERSION 3.15)
project(scanner)

set(CMAKE_CXX_STANDARD 14)

add_executable(scanner main.cpp defs.h Scanner.cpp Scanner.h SyntaxDiagrams.cpp SyntaxDiagrams.h)
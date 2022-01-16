cmake_minimum_required(VERSION 3.10)

# set the project name
project(Chess)

# add the executable
add_executable(Chess display.cpp main.cpp chess/standard/fen.cpp chess/shared.h chess/standard/shared_notation.h chess/standard/shared_notation.cpp chess/standard/valid_moves.cpp chess/standard/protects.cpp chess/standard/validate.cpp chess/standard/gamestate.cpp chess/standard/move.cpp chess/standard/pgn.cpp chess/standard/chess_notation.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


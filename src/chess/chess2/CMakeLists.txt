cmake_minimum_required(VERSION 3.10)

# set the project name
project(Chess)

# add the executable
add_executable(Chess board.cpp board.h display.cpp display.h main.cpp position.cpp position.h position/defends.cpp position/gamestate.cpp position/make_move.cpp position/validate.cpp position/turn.cpp position/moves.cpp chess.h chess.cpp notation/shared_notation.h notation/shared_notation.cpp notation/move_notation.cpp notation/move_notation.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

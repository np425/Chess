cmake_minimum_required(VERSION 3.21)
project(Chess)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(src/chess)
include_directories(src/chess/notation)
include_directories(src/chess/position)

add_executable(Chess
        src/chess/notation/char_types.cpp
        src/chess/notation/char_types.h
        src/chess/notation/fen.cpp
        src/chess/notation/fen.h
        src/chess/notation/move_notation.cpp
        src/chess/notation/move_notation.h
        src/chess/notation/notation.cpp
        src/chess/notation/notation.h
        src/chess/notation/pgn.cpp
        src/chess/notation/pgn.h
        src/chess/position/defends.cpp
        src/chess/position/gamestate.cpp
        src/chess/position/make_move.cpp
        src/chess/position/move_piece.cpp
        src/chess/position/moves.cpp
        src/chess/position/validate.cpp
        src/chess/basic_types.h
        src/chess/board.cpp
        src/chess/board.h
        src/chess/chess.cpp
        src/chess/chess.h
        src/chess/display.cpp
        src/chess/display.h
        src/chess/position.cpp
        src/chess/position.h
        src/chess/utils.h
        src/cli_args.cpp
        src/cli_args.h
        src/main.cpp
        src/menu.cpp
        src/menu.h
        src/pgn_file.cpp
        src/pgn_file.h)

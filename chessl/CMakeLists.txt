cmake_minimum_required(VERSION 3.21)

set(LIBRARY_NAME ChessL)

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_HEADERS board.h position.h chess.h notation.h move_notation.h fen.h char_types.h pgn.h display.h)

set(LIBRARY_SOURCE validate.cpp defends.cpp game_state.cpp moves.cpp move_piece.cpp make_move.cpp char_types.h)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_HEADERS} ${LIBRARY_SOURCE})


set(CMAKE_CXX_STANDARD 20)

cmake_minimum_required(VERSION 3.21)

project(ChessGame)

add_executable(${PROJECT_NAME} test_pgn.cpp pgn_file.h pgn_file.cpp)

add_subdirectory(chessl)

target_link_libraries(${PROJECT_NAME} ChessL)

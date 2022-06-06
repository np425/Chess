// TODO: Rewrite in separate cpp file

#ifndef CHESS_DISPLAY_H
#define CHESS_DISPLAY_H

#include "board.h"
#include <iostream>

namespace chess {
[[nodiscard]] constexpr const char *stateTypeToString(StateType state) noexcept {
    switch (state) {
        case StateType::PLAYING:
            return "playing";
        case StateType::CHECKMATE:
            return "checkmate";
        case StateType::STALEMATE:
            return "stalemate";
        default:
            return nullptr;
    }
}

[[nodiscard]] constexpr const char *playerToString(Player pl) noexcept {
    switch (pl) {
        case WHITE:
            return "white";
        case BLACK:
            return "black";
        default:
            return nullptr;
    }
}

[[nodiscard]] constexpr char pieceTypeToChar(PieceType piece) noexcept {
    switch (piece) {
        case ROOK:
            return 'R';
        case BISHOP:
            return 'B';
        case KNIGHT:
            return 'N';
        case QUEEN:
            return 'Q';
        case PAWN:
            return 'P';
        case KING:
            return 'K';
        case VOID:
            return '_';
        default:
            return 0;
    }
}

void printBoard(const Board &board) {
    // File number
    std::cout << "   | ";
    for (int x = 0; x < literals::BOARD_SIZE_X; ++x) {
        std::cout << static_cast<char>('A' + x) << " ";
    }
    std::cout << "|" << std::endl;

    // Separator
    std::cout << "---|-";
    for (int x = 0; x < literals::BOARD_SIZE_X; ++x) {
        std::cout << "--";
    }
    std::cout << "|---" << std::endl;

    // Board
    for (int y = literals::BOARD_SIZE_Y - 1; y >= 0; --y) {
        // Rank number left
        std::cout << " " << y + 1 << " | ";

        for (int x = 0; x < literals::BOARD_SIZE_X; ++x) {
            PieceType pieceType = pieceToType(board[{x, y}]);
            std::cout << pieceTypeToChar(pieceType) << " ";
        }

        // Rank number right
        std::cout << "| " << y + 1 << std::endl;
    }

    // Separator
    std::cout << "---|-";
    for (int x = 0; x < literals::BOARD_SIZE_X; ++x) {
        std::cout << "--";
    }
    std::cout << "|---" << std::endl;

    // File number
    std::cout << "   | ";
    for (int x = 0; x < literals::BOARD_SIZE_X; ++x) {
        std::cout << static_cast<char>('A' + x) << " ";
    }
    std::cout << "|" << std::endl;
}
}

#endif //CHESS_DISPLAY_H

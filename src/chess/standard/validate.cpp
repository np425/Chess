//
// Created by np on 2021-12-26.
//

/*
 * Board validation functions
 * */

#include "../standard.h"
#include <stdexcept>

void StandardChess::validateChecks() {
    // Find checks for both players, ensure no more than two checks
    Player inCheck = PL_NONE;
    for (int pl = PL_WHITE; pl <= PL_BLACK; ++pl) {
        checksEnd = getProtects(kingPos[pl], checks, (Player)!pl);
        if (checksEnd != checks) {
            if (inCheck != PL_NONE) { // Ensure both players don't have checks
                throw std::runtime_error("Both players have checks");
            } else inCheck = (Player)pl;
        }
    }
}

void StandardChess::validateBoard() {
    // Ensure exactly two kings are on the board
    kingPos[0] = {-1,-1};
    kingPos[1] = {-1,-1};

    for (int y = 0; y != BOARD_SIZE_Y; ++y) {
        for (int x = 0; x != BOARD_SIZE_X; ++x) {
            if (pieceToType(board[y*BOARD_SIZE_X+x]) != PT_KING) continue;

            Player kingOwner = pieceToPlayer(board[y*BOARD_SIZE_X+x]);

            if (kingPos[kingOwner].y != -1) {
                throw std::runtime_error("Multiple kings on the board");
            } else kingPos[kingOwner] = {y,x};
        }
    }
    if (kingPos[0].y == -1 || kingPos[1].y == -1) {
        throw std::runtime_error("Missing kings on the board");
    }

    // Ensure kings are not together
    if (abs(kingPos[0].x - kingPos[1].x) <= 1 && abs(kingPos[0].y - kingPos[1].y) <= 1) {
        throw std::runtime_error("Kings are together");
    }

    // Ensure no pawns on the last ranks
    for (int pl = PL_WHITE; pl <= PL_BLACK; ++pl) {
        int y = pl * (BOARD_SIZE_Y - 1);
        for (int x = 0; x != BOARD_SIZE_X; ++x) {
            if (pieceToType(board[y*BOARD_SIZE_X+x]) == PT_PAWN) {
                throw std::runtime_error("Pawns on last ranks");
            }
        }
    }

    return validateChecks();
}

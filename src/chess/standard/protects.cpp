//
// Created by np on 2021-12-26.
//

#include "../standard.h"

/*
 * Chess game state (checkmate, playing, stalemate) functions
 * */

StandardChess::Square* StandardChess::getBRQKProtects(const Square& target, Square* it, const Player pl) {
    for (int ySign = -1; ySign <= 1; ++ySign) {
        int y = target.y + ySign;
        if (y < 0 || y >= BOARD_SIZE_Y) continue; // Ensure Y within bounds

        for (int xSign = -1; xSign <= 1; ++xSign) {
            if (!xSign && !ySign) continue; // Ensure not the same square

            int x = target.x + xSign;
            if (x < 0 || x >= BOARD_SIZE_X) continue; // Ensure X within bounds

            // King protects
            if (pieceToPlayer(board[y*BOARD_SIZE_X+x]) == pl && pieceToType(board[y*BOARD_SIZE_X+x]) == PT_KING) {
                *(it++) = {y,x};
                break;
            }

            PieceType targetType = (!xSign || !ySign ? PT_ROOK : PT_BISHOP);
            // Queen, Rook, Bishop protects
            // Iterate in the direction
            for (int iy = y, ix = x; inBounds(iy, ix); iy += ySign, ix += xSign) {
                if (!board[iy*BOARD_SIZE_X+ix]) continue;
                PieceType curType = pieceToType(board[iy*BOARD_SIZE_X+ix]);
                if (pieceToPlayer(board[iy*BOARD_SIZE_X+ix]) == pl && (curType == targetType || curType == PT_QUEEN)) {
                    *(it++) = {iy,ix};
                }
                break; // Break after any piece has been found
            }
        }
    }
    return it;
}

StandardChess::Square* StandardChess::getKnightProtects(const Square& target, Square* it, const Player pl) {
    for (int z = 1; z <= 2; ++z) {
        for (int yAdd = -z; yAdd <= z; yAdd += z * 2) {
            for (int xAdd = -(z % 2) - 1; xAdd <= z % 2 + 1; xAdd += 2 * ((z % 2) + 1)) {
                int y = target.y + yAdd;
                int x = target.x + xAdd;
                if (!inBounds(y, x)) continue;
                if (pieceToPlayer(board[y*BOARD_SIZE_X+x]) == pl && pieceToType(board[y*BOARD_SIZE_X+x]) == PT_KNIGHT) {
                    *(it++) = {y,x};
                }
            }
        }
    }
    return it;
}

StandardChess::Square* StandardChess::getPawnProtects(const Square& target, Square* it, const Player pl) {
    int plSign = getPlayerSign(pl);
    int y = target.y - plSign;
    if (y < 0 || y >= BOARD_SIZE_Y) return it;

    for (int xSign = -1; xSign <= 1; xSign += 2) { // One forward diagonally
        int x = target.x + xSign;
        if (x < 0 || x >= BOARD_SIZE_X) continue;
        if (pieceToType(board[y*BOARD_SIZE_X+x]) == PT_PAWN && pieceToPlayer(board[y*BOARD_SIZE_X+x]) == pl) {
            *(it++) = {y,x};
        }
    }
    return it;
}

StandardChess::Square* StandardChess::getProtects(const Square& target, Square* it, const Player pl) {
    it = getBRQKProtects(target, it, pl);
    it = getKnightProtects(target, it, pl);
    it = getPawnProtects(target, it, pl);
    return it;
}

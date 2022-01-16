//
// Created by np on 2021-12-26.
//

#include "../standard.h"
#include "../shared.h"

void StandardChess::updateGameState() {
    // Sign is from the perspective of the opponent
    int sign = getPlayerSign((Player)!toMove);
    if (isCheckmate(toMove))
        gameState = sign * GS_CHECKMATE;
    else if (isStalemate(toMove))
        gameState = sign * GS_STALEMATE;
    else
        gameState = GS_PLAYING;
}

bool StandardChess::isCheckmate(const Player pl) {
    Square moves[MAX_ATTACKERS];
    unsigned checksAmount = checksEnd - checks;

    if (checksAmount == 0) {
        return false;
    } else if (checksAmount == 1) {
        // 1. Attack the attacker
        if (moves != getMoves(checks[0], moves, pl)) return false;

        // 2. Block the attacker (only if the check is not a knight or a pawn)
        PieceType checkType = pieceToType(board[checks[0].y*BOARD_SIZE_X+checks[0].x]);
        if (checkType != PT_KNIGHT && checkType != PT_PAWN) {
            Square diff = {checks[0].y - kingPos[pl].y, checks[0].x - kingPos[pl].x};
            Square sign = {numSign(diff.y), numSign(diff.x)};

            int y = kingPos[pl].y + sign.y;
            int x = kingPos[pl].x + sign.x;

            for (int iy = y, ix = x; iy < checks[0].y && ix < checks[0].x; iy += sign.y, ix += sign.x) {
                if (moves != getMoves(checks[0], moves, pl)) return false;
            }
        }
    }

    // 3. move the king
    for (int ySign = -1; ySign <= 1; ++ySign) {
        int y = kingPos[pl].y + ySign;
        if (y < 0 || y >= BOARD_SIZE_Y) continue;

        for (int xSign = -1; xSign <= 1; ++xSign) {
            if (xSign == 0 && ySign == 0) continue;

            int x = kingPos[pl].x + xSign;
            Piece target = board[y*BOARD_SIZE_X+x];

            // Cannot take your own pieces
            if (x < 0 || x >= BOARD_SIZE_X || pieceToPlayer(target) == pl) continue;

            // Check if square is protected
            if (moves == getProtects({y,x}, moves, (Player)!pl)) return false;
        }
    }

    return true;
}

bool StandardChess::isStalemate(const Player pl) {
    Square moves[MAX_ATTACKERS];

    for (int y = 0; y < BOARD_SIZE_Y; ++y) {
        for (int x = 0; x < BOARD_SIZE_X; ++x) {
            if (pieceToPlayer(board[y*BOARD_SIZE_X+x]) == pl) continue;

            if (moves != getMoves({y,x}, moves, pl)) return false;
        }
    }
    return true;
}

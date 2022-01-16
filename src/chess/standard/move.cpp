//
// Created by np on 2021-12-26.
//

#include "../standard.h"

/*
 * Chess piece moving functions including castling
 *
 */

void StandardChess::completeMove(const Square& from, const Square& to, const PieceType promote) {
    PieceType type = pieceToType(board[from.y*BOARD_SIZE_X+from.x]);
    int plSign = getPlayerSign(toMove);

    bool twoUp = false;

    // Update global variables
    switch (type) {
        case PT_PAWN:
            if (from.y + 2 * plSign == to.y) { // Two up
                twoUp = true;
                // Update passant (move "behind" the pawn)
                passant = {to.y - plSign, to.x};
            } else if (passant.y == from.y && passant.x == to.x) { // En passant
                board[from.y*BOARD_SIZE_X+to.x] = PL_NONE;
            }
            break;
        case PT_KING: // Eliminate castling
            canCastle[toMove][0] = canCastle[toMove][1] = false;
            kingPos[toMove] = to; // Update king position
            break;
        case PT_ROOK:
            if (from.x == 0) canCastle[toMove][0] = false;
            else if (from.x == BOARD_SIZE_X - 1) canCastle[toMove][1] = false;
            break;
        default:
            break;
    }

    // Handle promotion
    Piece movePiece = (promote ? plSign * promote : board[from.y*BOARD_SIZE_X+from.x]);

    // Move the piece
    board[to.y*BOARD_SIZE_X+to.x] = movePiece;
    board[from.y*BOARD_SIZE_X+from.x] = PT_NONE;

    // Invalidate en passant opportunity
    if (!twoUp) passant = {-1, -1};

    // Update player
    toMove = (Player)!toMove;

    // Update for enemy: checks
    checksEnd = getProtects(kingPos[toMove], checks, (Player)!toMove);

    // Update game state for player
    updateGameState();
}

bool StandardChess::attemptCastles(const int side) {
    if (!canCastle[toMove][side]) return false;

    int rx = side * (BOARD_SIZE_X - 1);
    int ry = toMove * (BOARD_SIZE_Y - 1);
    int xSign = (side ? 1 : -1);

    // Ensure empty squares in castling direction
    for (int x = kingPos[toMove].x; x != rx; x += xSign) {
        if (!board[ry*BOARD_SIZE_X+x]) return false;
    }

    Square protects[MAX_ATTACKERS];

    // Ensure two squares are not protected
    for (int x = 1; x <= 2; ++x) {
        if (protects != getProtects({rx, kingPos[toMove].x + x * xSign}, protects, (Player)!toMove))
        return false;
    }

    // Move pieces
    board[ry*BOARD_SIZE_X+kingPos[toMove].x + xSign] = board[ry*BOARD_SIZE_X+rx];
    board[ry*BOARD_SIZE_X+kingPos[toMove].x + 2 * xSign] = board[ry*BOARD_SIZE_X+kingPos[toMove].x];
    board[ry*BOARD_SIZE_X+rx] = PT_NONE;
    board[ry*BOARD_SIZE_X+kingPos[toMove].x] = PT_NONE;

    // Invalidate castling variables
    canCastle[toMove][0] = false;
    canCastle[toMove][1] = false;

    // Invalidate en passant opportunity
    passant = {-1,-1};

    // Update player
    toMove = (Player)!toMove;

    // Update checks
    checksEnd = getProtects(kingPos[toMove], checks, (Player)!toMove);

    // Update game state for player
    updateGameState();

    return true;
}

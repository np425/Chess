#include "board.h"
#include "utils.h"

namespace chess {

Board::Board(const Piece *pieces) {
    kingPos[WHITE].invalidate();
    kingPos[BLACK].invalidate();
    setBoard(pieces);
}

Coord Board::getKingPos(Player pl) const {
    return kingPos[pl];
}

Piece Board::operator[](unsigned idx) const {
    return board[idx];
}

Piece Board::operator[](const Coord &coord) const {
    return board[coord.y * BOARD_SIZE_Y + coord.x];
}

void Board::setBoard(const Piece *pieces) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        board[i] = pieces[i];
    }
}

void Board::placePiece(Piece piece, const Coord &to) {
    clearPiece(to);

    PieceType pt = pieceToType(piece);
    Player pl = pieceToPlayer(piece);

    board[to.y * BOARD_SIZE_X + to.x] = piece;

    if (pt == KING) {
        kingPos[pl] = to;
    }
}

void Board::clearPiece(const Coord &coord) {
    Piece piece = board[coord.y * BOARD_SIZE_X + coord.x];

    PieceType pt = pieceToType(piece);
    Player pl = pieceToPlayer(piece);

    board[coord.y * BOARD_SIZE_X + coord.x] = VOID;

    if (pt == KING) {
        kingPos[pl].invalidate();
    }
}

void Board::movePiece(const Coord &from, const Coord &to) {
    placePiece(board[from.y * BOARD_SIZE_X + from.x], to);
    board[from.y * BOARD_SIZE_X + from.x] = VOID;
}

bool Board::validatePawns() {
    // Pawns on the last rank
    for (int pl = 0; pl <= 1; ++pl) {
        int y = pl * (BOARD_SIZE_Y - 1);
        for (int x = 0; x < BOARD_SIZE_X; ++x) {
            if (pieceToType(board[y * BOARD_SIZE_X + x]) == PAWN) {
                return false;
            }
        }
    }
    return true;
}

bool Board::validateKings() {
    // King positions
    kingPos[WHITE].invalidate();
    kingPos[BLACK].invalidate();

    // Find kings
    for (int y = 0; y < BOARD_SIZE_Y; ++y) {
        for (int x = 0; x < BOARD_SIZE_X; ++x) {
            if (pieceToType(board[y * BOARD_SIZE_X + x]) != KING) {
                continue;
            }

            Player kingOwner = pieceToPlayer(board[y * BOARD_SIZE_X + x]);

            if (kingPos[kingOwner]) {
                // Player has too many kings
                return false;
            }

            kingPos[kingOwner] = {x, y};
        }
    }

    if (!kingPos[WHITE] || !kingPos[BLACK]) {
        // Missing king for a player
        return false;
    }

    // Make sure kings are one or more squares away from each other
    int dx = abs(kingPos[WHITE].x - kingPos[BLACK].x);
    int dy = abs(kingPos[WHITE].y - kingPos[BLACK].y);

    if (dx <= 1 && dy <= 1) {
        // Kings are too close
        return false;
    }
    return true;
}

bool Board::validate() {
    return validatePawns() && validateKings();
}

}

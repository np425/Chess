#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "basic_types.h"

namespace chess {

// Upside down (white first)
const Piece DEFAULT_BOARD[] = {
        +ROOK, +KNIGHT, +BISHOP, +QUEEN, +KING, +BISHOP, +KNIGHT, +ROOK,
        +PAWN, +PAWN, +PAWN, +PAWN, +PAWN, +PAWN, +PAWN, +PAWN,
        VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID,
        VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID,
        VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID,
        VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID,
        -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN,
        -ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK,
};

class Board {
protected:
    Piece board[BOARD_SIZE]{};
    Coord kingPos[2]{-1, -1};

    bool validatePawns();
    bool validateKings();
public:
    explicit Board(const Piece * = DEFAULT_BOARD);

    Piece operator[](unsigned) const;
    Piece operator[](const Coord&) const;

    Coord getKingPos(Player) const;

    void placePiece(Piece, const Coord &);
    void movePiece(const Coord &, const Coord &);
    void clearPiece(const Coord &);

    void setBoard(const Piece *pieces);

    bool validate();
};

}

#endif

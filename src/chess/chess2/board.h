#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "basic_types.h"
#include "utils.h"

namespace chess {

// Upside down (white first)
const Piece DEFAULT_BOARD[] = {
	+ROOK, +KNIGHT, +BISHOP, +QUEEN, +KING, +BISHOP, +KNIGHT, +ROOK,
	+PAWN, +PAWN,   +PAWN,   +PAWN,  +PAWN, +PAWN,   +PAWN,   +PAWN,
	 VOID,  VOID,    VOID,    VOID,   VOID,  VOID,    VOID,    VOID,
	 VOID,  VOID,    VOID,    VOID,   VOID,  VOID,    VOID,    VOID,
	 VOID,  VOID,    VOID,    VOID,   VOID,  VOID,    VOID,    VOID,
	 VOID,  VOID,    VOID,    VOID,   VOID,  VOID,    VOID,    VOID,
	-PAWN, -PAWN,   -PAWN,   -PAWN,  -PAWN, -PAWN,   -PAWN,   -PAWN,
	-ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK,
};

class Board {
protected:
	Piece board[BOARD_SIZE];
	Coord kingPos[2];
	
public:
	Board(const Piece* =DEFAULT_BOARD); 
	Piece operator[](unsigned) const;

	const Piece* getBoard() const;
	Coord getKingPos(const Player) const;

	void movePiece(const Piece, const Coord&);
	void movePiece(const Coord&, const Coord&);
	void clearPiece(const Coord&);

	void changeBoard(const Piece*);

	bool validate();
};

}

#endif

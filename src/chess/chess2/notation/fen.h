#ifndef CHESS_FEN_H
#define CHESS_FEN_H

#include "types.h"

#define FEN_MAX_LENGTH 50

/*

https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

Simplified FEN notation:
- Slashes are optional
- Whitespace are allowed and optional

*/

namespace chess {

// Reads a FEN square, one character could be denoted as a piece, or as many squares
// Returns: the amount of squares read
unsigned readFENSquare(const char*& it, Piece& piece);

bool readBoard(const char*& it, Piece* board);

bool readPlayer(const char*& it, Player& player);

bool readCastlingRights(const char*& it, bool canCastle[2][2]);

// Reads coordinates for a pawn that can be taken en passant
bool readPassant(const char*& it, Coord& sqr);

// Reads FEN position:
// 1. Board
// 2. Player to move
// 3. Castling rights
// 4. Pawn that can be taken en passant
bool readFENPosition(const char*& it, Position& pos);
	
// Reads FEN:
// 1. Board
// 2. Player to move
// 3. Castling rights
// 4. Pawn that can be taken en passant
// 5. Number of half moves before last capture or pawn advance
// 6. Number of full moves
bool readFEN(const char*& it, Position& pos, Count& count);

}
#endif

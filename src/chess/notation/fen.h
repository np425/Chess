/*

https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

Simplified FEN notation:
- Slashes are optional
- Whitespace are allowed and optional

*/

#ifndef CHESS_FEN_H
#define CHESS_FEN_H

#include "../board.h"
#include "shared_notation.h"

#define FEN_MAX_LENGTH 100

namespace chess {

// Reads a FEN square, one character could be denoted as a piece, or as many squares
// Returns: the amount of squares read
ExprEval readFENSquare(const char*& it, Piece& piece, unsigned& read);

ExprEval readBoard(const char*& it, Piece* board);

ExprEval readPlayer(const char*& it, Player& player);

ExprEval readCastlingRights(const char*& it, CastlingSide canCastle[2]);

// Reads coordinates for a pawn that can be taken en passant
ExprEval readPassant(const char*& it, Coord& sqr);

// Reads FEN:
// 1. Board
// 2. Player to move
// 3. Castling rights
// 4. Pawn that can be taken en passant
// 5. Number of half moves before last capture or pawn advance
// 6. Number of full moves
ExprEval readFEN(const char*& it, Board& board, PositionInfo& info);

}
#endif

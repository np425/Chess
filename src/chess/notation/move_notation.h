#ifndef CHESS_MOVE_NOTATION_H
#define CHESS_MOVE_NOTATION_H

#include "../basic_types.h"
#include "shared_notation.h"

// TODO: Refactor comments

/* 

https://en.wikipedia.org/wiki/Algebraic_notation_(chess)

Optimised move (algebraic) notation:
- Allows to denote pawn with P

*/

namespace chess {

// Reads capture symbol 
// Returns: true if capture symbolic, false if none
ExprEval readCapture(const char*& it, bool& promote);

// Reads PieceType that pawn can promote to (queen, rook, bishop, knight)
// Returns: 
// - true if given piece type the pawn can promote to
// - false if no given piece type or the piece type is not valid for use in promotion
ExprEval readPromoteType(const char*& it, PieceType& promote);

// Reads promote symbolic (=)
// Returns: True if symbol exists, false otherwise
ExprEval readPromote(const char*& it, PieceType& promote);

// Reads check and checkmate symbolic
// Returns: true if there is check or checkmate symbolic, false if none
ExprEval readFinalChecks(const char*& it, MoveInfo& move);

// Reads optional comment, skips string towards the end of it
// Returns: False if malformed comment, true if no comment or valid comment
ExprEval readOptComment(const char*& it);

// Reads castling notation
// Parameter castles: array of two bools: 
// - first bool = left (queen-side) castles
// - second bool = right (king-side) castles
// Returns: false if no castling notation or malformed expression, true otherwise
ExprEval readCastling(const char*& it, CastlingSide& side);

// Reads piece placing notation, includes:
// Piece names have to be in uppercase to avoid ambiguity with pawn coordinates
// - piece type, moving, castling and capturing symbolic
// excludes:
// - promotion, checks and comments symbolic
// Returns: true if valid notation, false otherwise
ExprEval readPiecePlacement(const char*& it, MoveInfo& move);

// Reads entire notation, including promotion, checks and comments
ExprEval readMoveNotation(const char*& it, MoveInfo& moveInfo);

}

#endif

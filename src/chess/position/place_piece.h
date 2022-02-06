#ifndef CHESS_PLACE_PIECE_H
#define CHESS_PLACE_PIECE_H

/*

Functions specifically for placing a piece, including castling
Does not validate if it's a legal move in the position

*/

#include "../types.h"

namespace chess {

// Assumes piece placement is valid, places piece and updates position metadata
void placePiece(Position& pos, const Coord& from, const Coord& to, const PieceType promote);

// Assumes castling is valid, castles and updates position metadata
void castles(Position& pos, const int side); 

}

#endif

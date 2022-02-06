#ifndef CHESS_MOVES_H
#define CHESS_MOVES_H

/*

Chess functions for finding valid moves

*/

#include "../types.h"

namespace chess {

// Checks if the path from piece to another piece's new location is clear
// Ignores piece's old placement coordinate
bool pathToMoveIsClear(const Piece* board, const Coord& from, const Coord& moveFrom,
                       const Coord& moveTo);

// Makes sure a pawn can move to the target square
// Makes sure there is no piece in between the pawn move
// Checks if capturing is viable depending on the target square
// Assumes a pawn is in the origin square
// Does not validate pins and checks
bool validPawnMove(const Position& pos, const Coord& from, const Coord& to);

// Makes sure the piece can move to the target square without being restricted by a pin
bool isPinned(const Position& pos, const Coord& from, const Coord& to);

// Makes sure the piece can move to the target square without being restricted by checks
bool movePreventsCheck(const Position& pos, const Coord& from, const Coord& to);

// Makes sure the piece can move to the target square
// Ensures move validity in current position, including pins and checks
bool canMove(const Position& pos, const Coord& from, const Coord& to);

// Gets all the pieces (their coordinates) that can move to this square in this position
void getMoves(const Position& pos, const Coord& coord, CoordArray& moves, const Player by);

// Determines whether player can castle in the chosen side
bool canCastle(const Position& pos, const unsigned side, const Player pl);
}

#endif

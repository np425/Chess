#ifndef CHESS_MAKE_MOVE_H
#define CHESS_MAKE_MOVE_H

/*

Functions for making a move by MoveInfo for current player
If the absolute coordinate is not given, tries to deduce the move
Validates move and updates it's metadata

*/

#include "../types.h"

namespace chess {

// Finds a valid move in the position and updates move data with its coordinates
// Updates capture notation as well
// If piece's origin coordinates are ambiguous (has -1), then
// tries to find an absolute coordinate and saves it in MoveInfo
// Returns: 
// - true if found a valid move
// - false if move is ambiguous, or such move does not exist in position
bool findValidMove(const Position& pos, MoveInfo& move);

// Makes a move and updates it's metadata
// Updates capture, check and checkmate notation after making the move
// If piece's origin coordinates are ambiguous (has -1), then
// tries to find an absolute coordinate and saves it in MoveInfo
// Returns: 
// - true if found a valid move
// - false if move is ambiguous, or such move does not exist in position
bool makeMove(Position& pos, MoveInfo& move);

}

#endif

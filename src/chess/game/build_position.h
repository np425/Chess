#ifndef CHESS_BUILD_POSITION_H
#define CHESS_BUILD_POSITION_H

/*

Validates and builds chess position from every move in moves array

*/

#include "../types.h"

namespace chess {

// Builds position from array of moves based on the current position
// Does not validate initial position
// Updates given more array metadata
// Prerequisite: valid first position
bool buildPosFromMoves(MovesArray& moves, Position& pos);

}

#endif

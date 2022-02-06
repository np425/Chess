#ifndef CHESS_VALIDATE_POSITION_H
#define CHESS_VALIDATE_POSITION_H

/*

Functions to validate if given position is legal

*/

#include "../types.h"

namespace chess {

// Prerequisite: king positions
// Gets all checks and validates them
bool findAndValidateChecks(const Position& pos, CoordArray& checks); 

// Finds king positions and validates them
bool findAndValidateKings(const Position& pos, Coord kingPos[2]);

// Makes sure no pawns are on the last ranks
bool validatePawnsPlacement(const Piece* board);

// Validates passant, if not absolute coordinate is given, finds the absolute 
bool validatePassant(const Position& pos, const Player pl);

// Validates and updates metadata about position
bool validatePosition(Position& pos); 

}

#endif 

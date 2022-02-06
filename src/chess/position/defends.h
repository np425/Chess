#ifndef CHESS_DEFENDS_H
#define CHESS_DEFENDS_H

/*

Chess functions for figuring out if pieces can defend a square

*/

#include "../types.h"

namespace chess {

// Makes sure there is no piece in between the origin and the target squares
bool pathIsClear(const Piece* board, const Coord& from, const Coord& to);

// Makes sure the piece in origin square could theoretically defend the target square
// Ignores pieces placement on the board, does not check whether there are pieces in the way
bool canDefend(const Piece* board, const Coord& from, const Coord& to);

// Makes sure the piece in origin square actually defends the target square
// Checks pieces placement on the board, makes sure there are no pieces in the way
bool defends(const Piece* board, const Coord& from, const Coord& to);

// Finds all the pieces (their squares) that can defend the given square
void getDefenders(const Piece* board, const Coord& coord, CoordArray& defenders, const Player by);

}

#endif

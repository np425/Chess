#ifndef CHESS_SHARED_NOTATION_H
#define CHESS_SHARED_NOTATION_H

#include "../basic_types.h"

/*

Notation-reading functions that are shared between notation formats

*/

namespace chess {

// Reads rank (x coordinate)
bool readX(const char*& it, int& x);

// Reads file (y coordinate)
bool readY(const char*& it, int& y);

// Reads coordinate: X and/or Y
// Returns: how many symbols for a coordinate read:
// - 2 for X and Y
// - 1 for X or Y
// - 0 for nothing
int readCoord(const char*& it, Coord& coord);

// Converts character to piece type, assumes character is uppercase
bool charToPieceType(const char chr, PieceType& pieceType);

// Reads a positive integer
// Parameters:
// it - reference to char pointer where to look for number, becomes place where it stopped reading
// num - converted number
// Returns: true if a number has been read, false otherwise
bool readPositiveInteger(const char*& it, unsigned& num);

}

#endif

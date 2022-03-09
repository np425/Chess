#ifndef CHESS_SHARED_NOTATION_H
#define CHESS_SHARED_NOTATION_H

#include "../basic_types.h"

/*

Notation-reading functions that are shared between notation formats

*/

namespace chess {

enum class ExprEval {
    None, Malformed, Valid
};

// Reads rank (x coordinate)
ExprEval readX(const char*& it, int& x);

// Reads file (y coordinate)
ExprEval readY(const char*& it, int& y);

// Reads incomplete coordinate: X and/or Y
// Returns: how many symbols for a coordinate read:
// - 2 for X and Y
// - 1 for X or Y
// - 0 for nothing
unsigned readCoord(const char*& it, Coord& coord);

// Converts character to piece type, assumes character is uppercase
// TODO: Maybe cast operator
PieceType charToType(const char chr);

PieceType charToPromoteType(const char chr);

CastlingSide charToCastlingSide(const char chr);

// Reads a positive integer
// Parameters:
// it - reference to char pointer where to look for number, becomes place where it stopped reading
// num - converted number
// Returns: true if a number has been read, false otherwise
ExprEval readInteger(const char*& it, unsigned& num);

ExprEval readStringInsensitive(const char*&, const char*);

}

#endif

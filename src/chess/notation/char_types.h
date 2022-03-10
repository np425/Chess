#ifndef CHESS_CHAR_TYPES_H
#define CHESS_CHAR_TYPES_H

#include "../basic_types.h"

namespace chess {	

PieceType charToType(const char chr);

PieceType charToPromoteType(const char chr);

CastlingSide charToCastlingSide(const char chr);

}

#endif

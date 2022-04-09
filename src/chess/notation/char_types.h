#ifndef CHESS_CHAR_TYPES_H
#define CHESS_CHAR_TYPES_H

#include "../basic_types.h"

namespace chess {

PieceType charToType(char chr);

PieceType charToPromoteType(char chr);

CastlingSide charToCastlingSide(char chr);

}

#endif

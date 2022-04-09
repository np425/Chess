#include "char_types.h"
#include <cctype>

namespace chess {

PieceType charToType(char chr) {
    switch (toupper(chr)) {
        case 'R':
            return ROOK;
        case 'N':
            return KNIGHT;
        case 'B':
            return BISHOP;
        case 'Q':
            return QUEEN;
        case 'K':
            return KING;
        case 'P': // pe4 allowed
            return PAWN;
        default:
            return VOID;
    }
}

PieceType charToPromoteType(char chr) {
    switch (toupper(chr)) {
        case 'R':
            return ROOK;
        case 'B':
            return BISHOP;
        case 'N':
            return KNIGHT;
        case 'Q':
            return QUEEN;
        default:
            return VOID;
    }

}

CastlingSide charToCastlingSide(char chr) {
    switch (toupper(chr)) {
        case 'K':
            return CASTLES_KSIDE;
        case 'Q':
            return CASTLES_QSIDE;
        default:
            return CASTLES_NONE;
    }
}

}

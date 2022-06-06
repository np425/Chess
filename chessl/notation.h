#ifndef CHESS_NOTATION_H
#define CHESS_NOTATION_H

#include "board.h"
#include "char_types.h"
#include <cmath>

namespace chess {

[[nodiscard]] constexpr PieceType charToType(const UpperChar chr) noexcept {
    switch (static_cast<char>(chr)) {
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
        case 'P':
            return PAWN;
        default:
            return VOID;
    }
}

class NotationParser {
public:
    static constexpr bool expectChar(char expected, const char *&outIt) noexcept {
        if (*outIt != expected) {
            return false;
        }

        ++outIt;
        return true;
    }

    static constexpr bool expectInsensitiveString(const char *str, const char *&outIt) noexcept {
        auto it{outIt};
        for (; *str; ++str, ++it) {
            if (UpperChar{*str} != UpperChar{*it}) {
                return false;
            }
        }
        outIt = it;
        return true;
    }

    // X must be lowered case
    static constexpr bool parseX(int &outX, const char *&outIt) noexcept {
        int x = *outIt - 'a';
        if (!xInBounds(x)) {
            return false;
        }

        outX = x;
        ++outIt;
        return true;
    }

    static constexpr bool parseY(int &outY, const char *&outIt) noexcept {
        int y = *outIt - '1';
        if (!yInBounds(y)) {
            return false;
        }

        outY = y;
        ++outIt;
        return true;
    }

    static constexpr int parseCoordPoints(Coord &outCoord, const char *&outIt) noexcept {
        int read{0};
        if (parseX(outCoord.x, outIt)) {
            ++read;
        }
        if (parseY(outCoord.y, outIt)) {
            ++read;
        }
        return read;
    }

    static constexpr bool parsePlayer(Player &outPl, const char *&outIt) noexcept {
        switch ((char) *outIt) {
            case 'W':
                outPl = WHITE;
                break;
            case 'B':
                outPl = BLACK;
                break;
            default:
                return false;
        }
        ++outIt;
        return true;
    }

    static constexpr bool parseInsensitivePieceType(PieceType &outType, const char *&outIt) noexcept {
        PieceType type;

        if (!(type = charToType(UpperChar{*outIt}))) {
            return false;
        }

        outType = type;
        ++outIt;
        return true;
    }

    static constexpr bool parseUpperPieceType(PieceType &outType, const char *&outIt) noexcept {
        PieceType type;

        if (!(type = charToType(UpperChar::assume(*outIt)))) {
            return false;
        }

        outType = type;
        ++outIt;
        return true;
    }

    static constexpr int parseUnsignedInt(int &outInt, const char *&outIt) noexcept {
        int l = 0, num = 0;

        for (char chr = *outIt; chr >= '0' && chr <= '9'; chr = *(outIt + l)) {
            ++l;
        }

        if (!l) {
            // No number
            return 0;
        }

        // Construct number
        for (int i = l; i; --i) {
            int digit = *outIt - '0';
            num += (static_cast<int>(std::pow(10, i - 1)) * digit);
            ++outIt;
        }

        outInt = num;
        return l;
    }
};

}

#endif //CHESS_NOTATION_H
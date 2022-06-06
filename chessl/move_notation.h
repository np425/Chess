// TODO: Put in separate cpp file

#ifndef CHESS_MOVE_NOTATION_H
#define CHESS_MOVE_NOTATION_H

#include "position.h"
#include "notation.h"

namespace chess {

// Pieces are notated in upper case
[[nodiscard]] constexpr PieceType charToPromoteType(const UpperChar chr) noexcept {
    switch (static_cast<char>(chr)) {
        case 'Q':
            return QUEEN;
        case 'N':
            return KNIGHT;
        case 'R':
            return ROOK;
        case 'B':
            return BISHOP;
        default:
            return VOID;
    }
}

class MoveNotationParser : public NotationParser {
public:
    static constexpr bool parseCaptureSymbolic(bool &outCapture, const char *&outIt) noexcept {
        if (!expectChar('x', outIt)) {
            return false;
        }

        outCapture = true;
        return true;
    }

    static bool parseCastling(CastlingSide &outSide, const char *&outIt) noexcept {
        if (!expectInsensitiveString("O-O", outIt)) {
            return false;
        }

        if (!expectInsensitiveString("-O", outIt)) {
            outSide = CASTLES_KING_SIDE;
            return true;
        }

        outSide = CASTLES_QUEEN_SIDE;
        return true;
    }

    static constexpr bool parseCheckType(CheckType &outType, const char *&outIt) noexcept {
        switch (*outIt) {
            case '+':
                outType = CheckType::Check;
                break;
            case '#':
                outType = CheckType::Checkmate;
                break;
            default:
                return false;
        }
        ++outIt;
        return true;
    }

    static constexpr bool parsePromoteType(PieceType &outType, const char *&outIt) noexcept {
        PieceType type;

        if (!(type = charToPromoteType(UpperChar{*outIt}))) {
            return false;
        }

        outType = type;
        ++outIt;
        return true;
    }

    static constexpr bool expectOptionalComment(const char *&outIt) noexcept {
        auto it{outIt};

        if (expectChar(';', it)) {
            // Comment to the end of line
            while (*it != '\n' && *it) {
                ++it;
            }
        } else if (expectChar('{', it)) {
            // Comment to the next curly brace, wraps
            while (*it != '}' && *it) {
                ++it;
            }

            if (!expectChar('}', it)) {
                // Missing ending }
                return false;
            }
        } else {
            // Do not skip spaces
            return true;
        }

        outIt = it;
        return true;
    }

    static bool parseMove(MoveInfo &outMove, const char *&outIt) noexcept {
        auto it{outIt};
        MoveInfo move{};
        move.path.origin = literals::INVALID_COORD;

        move.castles = CASTLES_NONE;
        move.capture = false;

        // If piece type is provided
        bool explicitPiece{true};

        if (!parseUpperPieceType(move.pieceType, it)) {
            move.pieceType = PAWN;
            explicitPiece = false;
        }

        /* Move Placement */
        int read = parseCoordPoints(move.path.origin, it);
        if (read > 0) {
            if (parseX(move.path.target.x, it)) {
                if (!parseY(move.path.target.y, it)) {
                    return false;
                }
            } else if (parseCaptureSymbolic(move.capture, it)) {
                if (!parseX(move.path.target.x, it) || !parseY(move.path.target.y, it)) {
                    return false;
                }
            } else if (read == 2) {
                move.path.target = move.path.origin;
                move.path.origin = literals::INVALID_COORD;
            } else {
                // Malformed piece
                return false;
            }
        } else if (explicitPiece && parseCaptureSymbolic(move.capture, it)) {
            if (!parseX(move.path.target.x, it) || !parseY(move.path.target.y, it)) {
                return false;
            }
        } else if (!explicitPiece) {
            // If not marked with piece, then it has to be castling
            move.pieceType = VOID;

            if (!parseCastling(move.castles, it)) {
                return false;
            }
        } else {
            // No piece
            return false;
        }

        /* Promotion */
        if (expectChar('=', it)) {
            if (move.pieceType != PAWN || !parsePromoteType(move.promoteType, it)) {
                return false;
            }
        }

        parseCheckType(move.checks, it);

        /* Optional comment */
        if (!expectOptionalComment(it)) {
            return false;
        }

        outIt = it;
        outMove = move;
        return true;
    }
};

}

#endif //CHESS_MOVE_NOTATION_H

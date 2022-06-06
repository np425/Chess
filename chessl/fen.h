// TODO: Requires testing
// TODO: Put in separate cpp file

#ifndef CHESS_FEN_H
#define CHESS_FEN_H

#include "notation.h"
#include "position.h"

namespace chess {

[[nodiscard]] constexpr CastlingSide charToCastlingSide(const UpperChar chr) {
    switch (static_cast<char>(chr)) {
        case 'K':
            return CASTLES_KING_SIDE;
        case 'Q':
            return CASTLES_QUEEN_SIDE;
        default:
            return CASTLES_NONE;
    }
}

struct FENParser : public NotationParser {
    static constexpr int parseFENSquare(Piece &outPiece, const char *&outIt) noexcept {
        PieceType pieceType;
        int read;
        const char chr = *outIt;

        if (!parseInsensitivePieceType(pieceType, outIt)) {
            // Assume empty squares

            read = chr - '0';
            if (!xInBounds(read)) {
                return 0;
            }

            outPiece = VOID;
            ++outIt;
        } else {
            // A piece
            int plSign = (CharUtils::isLower(chr) ? -1 : 1);
            outPiece = pieceType * plSign;
            read = 1;
        }

        return read;
    }

    static constexpr int parseOptionalFENCastlingRights(std::array<CastlingPerms, 2> &outPerms, const char *&outIt) {
        auto it{outIt};
        std::array<CastlingPerms, 2> perms{CASTLES_NONE, CASTLES_NONE};

        int read = true;

        if (!expectChar('-', it)) {
            read = 0;

            while (true) {
                char chr = *it;

                CastlingSide side = charToCastlingSide(UpperChar(chr));

                if (side == CASTLES_NONE) {
                    if (read) {
                        break;
                    }
                    return false;
                }

                Player pl = (CharUtils::isLower(chr) ? BLACK : WHITE);

                if (perms[pl] & side) {
                    // No repetitive castling rights
                    return false;
                }

                perms[pl] |= side;
                ++it;
                ++read;
            }
        }

        outIt = it;
        outPerms = perms;
        return read;
    }

    static bool parseFENBoard(Board &outBoard, const char *&outIt) noexcept {
        Board board{};
        auto it{outIt};

        // Start reading from black pieces
        for (Coord cIt{0, literals::BOARD_SIZE_Y - 1}; cIt.y != -1; --cIt.y) {
            for (; cIt.x < literals::BOARD_SIZE_X;) {
                Piece piece;
                int read = parseFENSquare(piece, it);

                if (!read || cIt.x > literals::BOARD_SIZE_X) {
                    // Unable to read a piece or read too many pieces
                    return false;
                }

                // Populate board with appropriate amount of pieces
                for (; read > 0; --read, ++cIt.x) {
                    board[cIt] = piece;
                }
            }

            // Skips slash and space
            while (*it == ' ' || *it == '/') {
                ++it;
            }
        }

        outBoard = board;
        outIt = it;
        return true;
    }

    static bool parseOptionalFENPassant(Coord &outCoord, const char *&outIt) {
        Coord coord{};
        auto it{outIt};

        if (!expectChar('-', it) && (!parseX(coord.x, it) || !parseY(coord.y, it))) {
            return false;
        }

        outCoord = coord;
        outIt = it;
        return true;
    }

    static bool parseFEN(Position &outPos, const char *&outIt) noexcept {
        auto it{outIt};

        Board board{};
        Player toMove;
        std::array<CastlingPerms, 2> castlingPerms{};
        Coord passant{};
        int halfMoves, fullMoves;

        if (!parseFENBoard(board, it)) {
            return false;
        }
        CharUtils::skipWhiteSpace(it);

        if (!parsePlayer(toMove, it)) {
            return false;
        }
        CharUtils::skipWhiteSpace(it);

        if (!parseOptionalFENCastlingRights(castlingPerms, it)) {
            return false;
        }
        CharUtils::skipWhiteSpace(it);

        if (!parseOptionalFENPassant(passant, outIt)) {
            return false;
        }
        CharUtils::skipWhiteSpace(it);

        if (!parseUnsignedInt(halfMoves, it)) {
            return false;
        }
        CharUtils::skipWhiteSpace(it);

        if (!parseUnsignedInt(fullMoves, it)) {
            return false;
        }

        outPos = Position{board, toMove, PLAYING, castlingPerms, passant, fullMoves, halfMoves};
        outIt = it;
        return true;
    }

};

}


#endif //CHESS_FEN_H

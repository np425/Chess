//
// Created by np on 2021-12-25.
//

/*
 Forsyth–Edwards Notation (FEN) notation functions
 https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 */

#include "../standard.h"
#include "../shared.h"
#include "shared_notation.h"
#include <cctype>

int StandardChess::readPiece(const char *&it, Piece &piece) {
    int sign = (std::isupper(*it) ? 1 : -1);

    unsigned char chr = std::tolower(*it);

    switch (chr) {
        case 'r':
            piece = PT_ROOK;
            break;
        case 'n':
            piece = PT_KNIGHT;
            break;
        case 'b':
            piece = PT_BISHOP;
            break;
        case 'q':
            piece = PT_QUEEN;
            break;
        case 'k':
            piece = PT_KING;
            break;
        case 'p':
            piece = PT_PAWN;
            break;
        default:
            // Empty spaces
            if (chr >= '1' && chr <= '8') {
                piece = PT_NONE;
                ++it;
                return chr - '0';
            } else
                return 0;
    }

    piece *= sign;
    ++it;
    return 1;
}

bool StandardChess::readPlayer(const char *&it, Player &player) {
    switch (*it) {
        case 'w':
            player = PL_WHITE;
            break;
        case 'b':
            player = PL_BLACK;
            break;
        default:
            return false;
    }

    ++it;
    return true;
}

bool StandardChess::readCastlingRights(const char *&it, bool castlingRights[2][2]) {
    castlingRights[0][0] = false;
    castlingRights[0][1] = false;
    castlingRights[1][0] = false;
    castlingRights[1][1] = false;

    if (*it == '-') {
        ++it;
        return true;
    }
    bool foundOne = false;

    while (true) {
        Player player = (isupper(*it) ? PL_WHITE : PL_BLACK);
        unsigned char chr = tolower(*it);

        int side;

        // Order does not matter
        switch (chr) {
            case 'k':
                side = 1;
                foundOne = true;
                break;
            case 'q':
                side = 0;
                foundOne = true;
                break;
            default:
                // Make sure there is at least one castling right if no -
                return foundOne;
        }

        if (castlingRights[player][side]) {
            return false; // Side not found or it's already set
        } else {
            castlingRights[player][side] = true;
            ++it;
        }
    }
}

bool StandardChess::readPassant(const char *&it, Square &sqr) {
    sqr = {-1, -1};

    if (*it == '-') {
        ++it;
        return true;
    }

    if (!readX(it, sqr.x) || !readY(it, sqr.y)) return false;
    else return true;
}

bool readPositiveInteger(const char *&start, unsigned &number) {
    const char *end = textToPositiveInteger(start, number);

    // Must be separated by space
    bool returnVal = (start != end && (*end == ' ' || *end == 0));
    start = end;
    return returnVal;
}


bool StandardChess::loadFEN(const char *fen) {
    const char *it = fen;

    // 1. Pieces reading (from White's perspective)
    for (int y = StandardChess::BOARD_SIZE_Y - 1; y >= 0; --y) {
        for (int x = 0; x < StandardChess::BOARD_SIZE_X;) {
            int read = readPiece(it, StandardChess::board[y * BOARD_SIZE_X + x]);
            if (!read) return false; // Unable to read a piece
            else x += read;
        }
        // Skip whitespaces and slashes (slashes are optional)
        while (*it == ' ' || *it == '/') ++it;
    }
    while (*it == ' ') ++it;

    // 2. Next player to move
    if (!readPlayer(it, StandardChess::toMove))
        return false;
    else
        while (*it == ' ') ++it;

    // 3. Castling rights
    if (!readCastlingRights(it, StandardChess::canCastle))
        return false;
    else
        while (*it == ' ') ++it;

    // 4. Passant
    if (!readPassant(it, StandardChess::passant))
        return false;
    else
        while (*it == ' ') ++it;

    // 5. Half moves
    if (!readPositiveInteger(it, StandardChess::halfMoves))
        return false;
    else
        while (*it == ' ') ++it;

    // 6. Full moves
    if (!readPositiveInteger(it, StandardChess::fullMoves))
        return false;
    else
        while (*it == ' ') ++it;

    return true;
}

//
// Created by np on 2021-12-26.
//

/*
 * Chess moves standard notation
 * */

#include "../standard.h"
#include "shared_notation.h"
#include <cctype>

bool readOptPromote(const char*& it, StandardChess::PieceType& promote) {
    if (*it != '=') return true;
    else ++it;

    switch (*it) {
        case 'r': promote = StandardChess::PT_ROOK; break;
        case 'b': promote = StandardChess::PT_BISHOP; break;
        case 'n': promote = StandardChess::PT_KNIGHT; break;
        case 'q': promote = StandardChess::PT_QUEEN; break;
        default: --it; return false;
    }

    ++it;
    return true;
}

// ----------- Composite reading functions (input lowercase)
bool readOptCaptureXY(const char*& it, bool& capture, int& x, int& y) {
    readCapture(it, capture);

    if (!readX(it, x)) return false;
    else if (!readY(it, y)) it -= (1 + capture);
    else return true;

    return false;
}

bool readOptPromoteFinalChecks(const char*& it, StandardChess::PieceType& promote, bool& check, bool& checkmate) {
    readOptPromote(it, promote);

    if (!readOptFinalChecks(it, check, checkmate)) it -= 2;
    else return true;

    return false;
}


bool StandardChess::translateNotation(const char* notation, MoveInfo &move) {
//    textToLower(notation); // Lowercase notation

    const char* it = notation;

    unsigned char chr = std::tolower(*it);

    // Deduce piece types
    if (chr == 'r') move.pieceType = PT_ROOK;
    else if (chr == 'n') move.pieceType = PT_KNIGHT;
    else if (chr == 'b') move.pieceType = PT_BISHOP;
    else if (chr == 'q') move.pieceType = PT_QUEEN;
    else if (chr == 'k') move.pieceType = PT_KING;
    else if (readX(it, move.from.x)) { // If X is given, assume piece is a pawn
        // Expect x->X->Y->(=p)->(+,#), Y->(=p)->(+,#), Y->(x)->X->Y->(=p)->(+,#)
        move.pieceType = PT_PAWN;
        if (readCapture(it, move.capture)) { // x->X->Y->(=p)->(+,#)
            if (!readX(it, move.to.x) || !readY(it, move.to.y))
                return false;
            else
                return readOptPromoteFinalChecks(it, move.promote, move.check, move.checkmate);
        } else if (readY(it, move.from.y)) { // Y->(=p)->(+,#), Y->(x)->X->Y->(=p)->(+,#)
            if (readOptCaptureXY(it, move.capture, move.to.x, move.to.y)) {
                return readOptPromoteFinalChecks(it, move.promote, move.check, move.checkmate);
            } else if (readOptPromoteFinalChecks(it, move.promote, move.check, move.checkmate)) {
                // Invalidate origin coordinates
                move.to.x = move.from.x;
                move.to.y = move.from.y;
                move.from = {-1,-1};
                return true;
            } else
                return false;
        } else
            return false;
    } else if (chr == 'o') { // Castling
        // O-O(+,#); O-O-O(+,#)

        //Expect -
        ++it;
        if (*it != '-')
            return false;

        // Expect o: king side castle
        ++it;
        if (std::tolower(*it) != 'o')
            return false;

        ++it;
        // Optional king side checks
        if (readOptFinalChecks(it, move.check, move.checkmate)) {
            move.castles[1] = true;
            return true;
        }

            // Expect -
        else if (*it != '-')
            return false;

        // Expect o: queen side castle
        ++it;
        if (std::tolower(*it) != 'o')
            return false;

        // Optional queen side checks
        return (move.castles[0] = readOptFinalChecks(it, move.check, move.checkmate));
    } else
        return false; // Unknown first character

    // For all pieces except pawns (pawns are processed separately) continue here:

    // Expects:
    // x: X->Y->(+,#);
    // X: (x)->X->Y->(+,#), Y(+,#), Y->(x)->X->Y->(+,#);
    // Y: (x)->X->Y->(+,#)

    ++it;
    if (readCapture(it, move.capture)) { // Takes
        // Expect X->Y->(+,#)
        if (!readX(it, move.to.x) || !readY(it, move.to.y))
            return false;
        else
            return readOptFinalChecks(it, move.check, move.checkmate);
    } else if (readX(it, move.from.x)) { // X
        // Expect (x)->X->Y->(+,#) or Y->(+,#) or Y->(x)->X->Y->(+,#)
        if (readY(it, move.from.y)) {
            if (readOptFinalChecks(it, move.check, move.checkmate)) {
                // Invalidate origin coordinates
                move.to.x = move.from.x;
                move.to.y = move.from.y;
                move.from = {-1,-1};
                return true;
            } else if (readOptCaptureXY(it, move.capture, move.to.x, move.to.y)) {
                return readOptFinalChecks(it, move.check, move.checkmate);
            } else
                return false;
        } else if (readOptCaptureXY(it, move.capture, move.to.x, move.to.y)) {
            return readOptFinalChecks(it, move.check, move.checkmate);
        } else
            return false;
    } else if (readY(it, move.from.y)) { // Y coordinate
        // Expect (x)->X->Y->(+,#)
        if (readOptCaptureXY(it, move.capture, move.to.x, move.to.y)) {
            return readOptFinalChecks(it, move.check, move.checkmate);
        } else
            return false;
    } else
        return false;
}

bool StandardChess::loadNotation(const char *notation) {
    MoveInfo move;
    if (!translateNotation(notation, move)) return false;

    if (move.castles[0] || move.castles[1]) {
        int side = (move.castles[0] ? 0 : 1);
        return attemptCastles(side);
    }

    // Normal moves
    Square foundPiece = {-1,-1};
    Square moves[MAX_ATTACKERS];
    Square* end = getAvailableMoves(move.to, moves);

    for (Square* it = moves; it != end; ++it) {
        if (pieceToType(board[it->y*BOARD_SIZE_X+it->x]) != move.pieceType) continue;
        if (move.from.y != -1 && it->y != move.from.y) continue;
        if (move.from.x != -1 && it->x != move.from.x) continue;

        if (foundPiece.y == -1) foundPiece = *it;
        else return false; // Ambiguous move
    }

    if (foundPiece.y != -1) {
        completeMove(foundPiece, move.to, move.promote);
        return true;
    } else return false;
}

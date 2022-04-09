#include "../position.h"
#include "utils.h"

namespace chess {

bool Position::findValidMove(MoveInfo &move, Player by) const {
    if (!move.to) {
        // Must be absolute target square coordinates
        return false;
    }

    Coords moves;
    getMoves(move.to, moves, by);

    Coord *chosen = nullptr;
    for (auto it = moves.begin(); it < moves.end(); ++it) {
        // Filter the right move

        Piece piece = board[*it];
        PieceType type = pieceToType(piece);

        if (type != move.type) {
            continue;
        }

        if (chosen) {
            // Ambiguous move
            return false;
        }
        chosen = &(*it);
    }

    if (!chosen) {
        // No valid move found
        return false;
    }

    move.from = *chosen;

    return true;
}

bool Position::makeMove(MoveInfo &move, Player pl) {
    if (move.castles != CASTLES_NONE) {
        // Castling
        if (move.castles != CASTLES_QSIDE && move.castles != CASTLES_KSIDE) {
            // Unknown castling side
            return false;
        }

        if (!canCastle(move.castles, pl)) {
            return false;
        }

        castles(move.castles);
    } else {
        // Other piece placement
        if (move.type == VOID) {
            return false;
        }

        if (!findValidMove(move, pl)) {
            // Could not find a move that's valid in current position
            return false;
        }

        if (board[move.to]) {
            move.capture = true;
        }

        movePiece(move.from, move.to, move.promote);
    }

    // Updates move information
    if (stateToType(state) == CHECKMATE) {
        move.checks = CheckType::Checkmate;
    } else if (!checks.empty()) {
        move.checks = CheckType::Check;
    }

    // Update checks
    return true;
}

bool Position::makeMove(MoveInfo &move) {
    return makeMove(move, toMove);
}

}

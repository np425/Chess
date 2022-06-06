#include "position.h"

namespace chess {

Coord Position::findValidMove(const MoveInfo &move, Player by) const noexcept {
    const auto &target = move.path.target;
    const auto &origin = move.path.origin;

    if (!target.withinBounds()) {
        // Must be absolute target square coordinates
        return literals::INVALID_COORD;
    }

    Coord chosen{literals::INVALID_COORD};

    for (const Coord &coord: getMoves(target, by)) {
        // Filter the right move
        Piece piece = m_board[coord];
        PieceType type = pieceToType(piece);

        // Filter the moves
        if (type != move.pieceType
            || (xInBounds(origin.x) && origin.x != coord.x)
            || (yInBounds(origin.y) && origin.y != coord.y)) {
            continue;
        }

        if (chosen != literals::INVALID_COORD) {
            // Ambiguous move
            return literals::INVALID_COORD;
        }
        chosen = coord;
    }

    return chosen;
}

bool Position::makeMove(MoveInfo &move) noexcept {
    if (move.castles != CASTLES_NONE) {
        // Castling
        if (move.castles != CASTLES_QUEEN_SIDE && move.castles != CASTLES_KING_SIDE) {
            // Unknown castling side
            return false;
        }

        if (!canCastle(move.castles, m_toMove)) {
            return false;
        }

        castles(move.castles);
    } else {
        // Other piece placement
        if (!move.pieceType) {
            return false;
        }

        Coord origin = findValidMove(move, m_toMove);
        if (origin == literals::INVALID_COORD) {
            // Could not find a move that's valid in current position
            return false;
        }
        // Update move
        move.path.origin = origin;

        if (m_board[move.path.target]) {
            // Update capture flag
            move.capture = true;
        }

        movePiece(move.path, move.promoteType);
    }

    // Updates move information
    if (stateToType(m_state) == CHECKMATE) {
        move.checks = CheckType::Checkmate;
    } else if (!m_checks.empty()) {
        move.checks = CheckType::Check;
    } else {
        move.checks = CheckType::None;
    }

    // Update checks
    return true;
}

}

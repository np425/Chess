#include "position.h"

namespace chess {

Coord Position::findDefender(Coord it, const Coord &target, Player by) const noexcept {
    for (; it != literals::END_COORD; ++it) {
        Piece piece = m_board[it];

        if (pieceToPlayer(piece) == by && defends({it, target})) {
            break;
        }
    }
    return it;
}

CoordVector Position::getDefenders(const Coord &target, Player by) const noexcept {
    CoordVector defenders;
//    defenders.reserve(MAX_MOVES_PER_SQUARE);

    Coord it{0, 0};
    while ((it = findDefender(it, target, by)) != literals::END_COORD) {
        defenders.push_back(it);
        ++it;
    }

    return defenders;
}

bool Position::defends(const Path &path) const noexcept {
    Piece piece = m_board[path.origin];
    PieceType pieceType = pieceToType(piece);
    return couldDefend(piece, path) && (pieceType == KNIGHT || isPathClear(path));
}

bool Position::isPathClear(const Path &path) const noexcept {
    Coord sign{path.distance().toSign()};

    for (Coord it{path.origin + sign}; it != path.target; it += sign) {
        if (m_board[it]) {
            // Blocking piece found
            return false;
        }
    }

    return true;
}

bool Position::defendsMove(const Coord &from, const Path &move) const noexcept {
    Piece piece = m_board[from];
    PieceType pieceType = pieceToType(piece);
    return couldDefend(piece, {from, move.target}) && (pieceType == KNIGHT || isPathToMoveClear(from, move));
}

Coord Position::findMoveDefender(Coord it, const Path &move, Player by) const noexcept {
    for (; it != literals::END_COORD; ++it) {
        Piece piece = m_board[it];

        if (pieceToPlayer(piece) == by && defendsMove(it, move)) {
            break;
        }
    }
    return it;
}

bool Position::couldDefend(Piece piece, const Path &path) noexcept {
    PieceType pieceType = pieceToType(piece);
    Player piecePl = pieceToPlayer(piece);
    int plSign = getPlayerSign(piecePl);

    Coord d{path.distance()};

    if (!d.x && !d.y) {
        // Can't defend itself
        return false;
    }

    // For rook, bishop and queen path must be checked if it's clear
    switch (pieceType) {
        case KING:
            return std::abs(d.x) <= 1 && std::abs(d.y) <= 1;
        case ROOK:
            return !d.x || !d.y;
        case BISHOP:
            return std::abs(d.x) == std::abs(d.y);
        case QUEEN:
            return std::abs(d.x) == std::abs(d.y) || !d.x || !d.y;
        case KNIGHT:
            return std::abs(d.x * d.y) == 2;
        case PAWN:
            return std::abs(d.x) == 1 && d.y == plSign;
        default:
            // Unknown piece type
            return false;
    }
}

}// namespace old
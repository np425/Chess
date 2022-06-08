#include "position.h"

namespace chess {

bool Position::canCastle(const CastlingSide side, Player who) const noexcept {
    if (!(m_castlingPerms[who] & side)) {
        // If moved the rook or castled, cannot castle anymore
        return false;
    }

    if (!m_checks.empty()) {
        // No castling under check
        return false;
    }

    Coord kPos = m_kingPos[who];
    Coord rPos{
        side * (literals::BOARD_SIZE_X - 1),
        who * (literals::BOARD_SIZE_Y - 1)
    };
    int xSign = (side ? 1 : -1);

    // Ensure empty squares in the castling direction
    for (Coord it{kPos.x + xSign, kPos.y}; it.x != rPos.x; ++it.x) {
        if (m_board[it]) {
            return false;
        }
    }

    // Ensure two squares are not protected
    for (int ix = 1; ix <= 2; ++ix) {
        Coord it{kPos.x + ix * xSign, kPos.y};

        if (findDefender({0, 0}, it, static_cast<Player>(!who)) != literals::END_COORD) {
            return false;
        }
    }

    return true;
}

bool Position::isValidPawnMove(const Path &move) const noexcept {
    Piece piece = m_board[move.origin];
    Player piecePl = pieceToPlayer(piece);
    int plSign = getPlayerSign(piecePl);

    if (pieceToType(piece) != PAWN) {
        // Piece must be a pawn
        return false;
    }

    // One up
    piece = m_board[{move.target.x, move.origin.y + plSign}];

    Coord d{move.target - move.origin};

    // Diagonal pawn move (capture)
    if (std::abs(d.x) == 1 && d.y == plSign) {
        Player targetPl = pieceToPlayer(piece);

        // Can capture or passant square
        return (targetPl == static_cast<Player>(!piecePl) || m_passant == move.target);
    }

    /* Pawn up moves */
    if (d.x || piece) {
        // Must not be a move to the side and must be an empty piece square
        return false;
    }

    if (d.y == plSign) {
        // Pawn one up move
        return true;
    }

    /* Two up */
    if (m_board[move.target]) {
        // Must be empty piece square
        return false;
    }

    // Starting y for pawns
    int initialY = piecePl * (literals::BOARD_SIZE_X - 1) + plSign;

    // Pawn two up move, y must be pawn starting y (initialY)
    return (move.target.y - 2 * plSign == initialY);
}

bool Position::isMovePinned(const Path &move) const noexcept {
    Piece piece = m_board[move.origin];
    PieceType pieceType = pieceToType(piece);
    Player piecePl = pieceToPlayer(piece);
    Coord kPos = m_kingPos[piecePl];

    Coord d{move.origin - kPos};

    if (pieceType == KING || (d.x != d.y && d.x && d.y)) {
        // Make sure piece is placed horizontally/vertically/diagonally from the king
        // And is not the same square as the king
        return false;
    }

    bool expectPassant = false;
    if (pieceType == PAWN && m_passant == move.target) {
        expectPassant = true;
    }

    Coord sign{d.toSign()};

    bool foundPiece = false;

    // Iterate from king to the piece's direction
    for (Coord it{kPos + sign}; it.withinBounds(); it += sign) {
        Piece target = m_board[it];
        if (!target) {
            // Skip empty squares
            continue;
        }

        Player targetPl = pieceToPlayer(target);

        if (targetPl == static_cast<Player>(!piecePl)) {
            if (expectPassant && it != Coord{m_passant.x, move.origin.y}) {
                // Ignore passant piece to find pin
                continue;
            } else if (foundPiece && couldDefend(target, {it, kPos})) {
                // Make sure the piece can move in the pinned direction
                return (move.target - kPos) % sign != 0;
            } else {
                // Enemy piece is before given piece
                break;
            }
        } else {
            if (it == move.origin) {
                // Found the given piece
                foundPiece = true;
            } else {
                // Friendly piece found before or after the given piece
                break;
            }
        }
    }

    return false;
}

bool Position::doesMovePreventCheck(const Path &move) const noexcept {
    Piece piece = m_board[move.origin];
    PieceType pieceType = pieceToType(piece);
    Player piecePl = pieceToPlayer(piece);

    unsigned checksAmount = m_checks.size();

    if (!checksAmount) {
        return true;
    }

    if (checksAmount == 1) {
        const Coord &check = m_checks.front();
        if (check == move.target) {
            // 1. Attack the piece
            if (pieceType != KING) {
                // If taking not with the king, it can take the piece
                return true;
            }

            // If taking with the king, make sure the piece is not defended
            return (findDefender({0, 0}, move.target, static_cast<Player>(!piecePl)) == literals::END_COORD);
        }

        if (pieceType != KING) {
            Coord kPos = m_kingPos[piecePl];
            // 2. Block the piece
            Coord signCT{(check - move.target).toSign()};
            Coord signCK{(check - kPos).toSign()};

            return (signCT == signCK && (move.target - kPos) % signCT == 0);
        }
    }

    // If there's more than 1 check, only king move is legal and move must be unprotected
    return (pieceType == KING &&
            (findMoveDefender({0, 0}, move, static_cast<Player>(!piecePl)) == literals::END_COORD));
}

bool Position::canMove(const Path &move) const noexcept {
    Piece piece = m_board[move.origin];
    Player piecePl = pieceToPlayer(piece);
    PieceType pieceType = pieceToType(piece);

    piece = m_board[move.target];
    Player targetPl = pieceToPlayer(piece);

    if (targetPl == piecePl || move.origin == move.target) {
        // Cannot capture your own pieces or move to the same square
        return false;
    }

    if (pieceType == PAWN) {
        if (!isValidPawnMove(move)) {
            // Must be either capture or one or two up move
            return false;
        }
    } else if (!defends(move)) {
        return false;
    }

    return !isMovePinned(move) && doesMovePreventCheck(move);
}

bool Position::isPathToMoveClear(const Coord &from, const Path &move) const noexcept {
    Coord sign{(move.target - from).toSign()};

    for (Coord it{from + sign}; it != move.target; it += sign) {
        if (it != move.origin && m_board[it]) {
            // Blocking piece found
            return false;
        }
    }

    return true;
}

Coord Position::findValidMove(Coord it, const Coord &target, Player by) const noexcept {
    for (; it != literals::END_COORD; ++it) {
        Piece piece = m_board[it];

        if (pieceToPlayer(piece) == by && canMove({it, target})) {
            break;
        }
    }
    return it;
}

CoordVector Position::getMoves(const Coord &to, Player by) const noexcept {
    CoordVector moves;
//    moves.reserve(MAX_MOVES_PER_SQUARE);

    Coord it{0, 0};
    while ((it = findValidMove(it, to, by)) != literals::END_COORD) {
        moves.push_back(it);
        ++it;
    }

    return moves;
}

}


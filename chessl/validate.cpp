#include "position.h"
#include <cmath>

namespace chess {

bool Position::validate() noexcept {
    // Pawns
    for (int pl = 0; pl <= 1; ++pl) {
        int y = pl * (literals::BOARD_SIZE_Y - 1);

        for (Coord it{0, y}; it.x != literals::BOARD_SIZE_X; ++it.x) {
            if (pieceToType(m_board[it]) == PAWN) {
                return false;
            }
        }
    }

    // Kings
    m_kingPos = {literals::INVALID_COORD, literals::INVALID_COORD};

    // Find king pos
    for (Coord it{0, 0}; it != literals::END_COORD; ++it) {
        Piece piece = m_board[it];
        if (pieceToType(piece) != KING) {
            continue;
        }

        Player kingPl = pieceToPlayer(piece);
        if (m_kingPos[kingPl] != literals::INVALID_COORD) {
            // Player has too many kings
            return false;
        }

        m_kingPos[kingPl] = it;
    }

    if (m_kingPos[WHITE] == literals::INVALID_COORD || m_kingPos[BLACK] == literals::INVALID_COORD) {
        // Missing king for a player
        return false;
    }

    Coord d = m_kingPos[WHITE] - m_kingPos[BLACK];

    if (std::abs(d.x) <= 1 && std::abs(d.y) <= 1) {
        // Kings are too close
        return false;
    }

    // Checks
    for (int pl = 0; pl < 2; ++pl) {
        m_checks = getDefenders(m_kingPos[pl], static_cast<Player>(!pl));
        if (pl != m_toMove && !m_checks.empty()) {
            // Opposite player of the one who is about to move cannot have a check
            return false;
        }
    }

    // Passant
    if (m_passant != literals::INVALID_COORD) {
        int plSign = getPlayerSign(m_toMove);

        Piece piece = m_board[{m_passant.x, m_passant.y + plSign}];
        PieceType pieceType = pieceToType(piece);
        Player piecePl = pieceToPlayer(piece);

        // Piece in passant location has to be enemy pawn
        if (pieceType != PAWN || piecePl == m_toMove) {
            m_passant = literals::INVALID_COORD;
        }
    }

    m_state = updateState(m_toMove);
    return true;
}

}// namespace chess
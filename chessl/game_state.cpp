#include "position.h"

namespace chess {

bool Position::isStalemate(Player pl) const noexcept {
    for (Coord it{0, 0}; it != literals::INVALID_COORD; ++it) {
        if (findValidMove({0, 0}, it, pl) != literals::END_COORD) {
            return false;
        }
    }
    return true;
}

bool Position::isCheckmate(Player pl) const noexcept {
    unsigned checksAmount = m_checks.size();
    Coord kPos = m_kingPos[pl];

    if (checksAmount == 0) {
        return false;
    }

    if (checksAmount == 1) {
        const Coord &check = m_checks.front();

        // 1. Attack the attacker (the one who checks)
        if (findValidMove({0, 0}, check, pl) != literals::END_COORD) {
            return false;
        }

        // 2. Block the attacker (only if the check is not a knight or a pawn)
        PieceType checkType = pieceToType(m_board[check]);

        if (checkType != KNIGHT && checkType != PAWN) {
            Coord sign{(check - kPos).toSign()};

            // Iterate from king towards the check
            for (Coord it{kPos + sign}; it != check; it += sign) {
                if (findValidMove({0, 0}, it, pl) != literals::END_COORD) {
                    return false;
                }
            }
        }
    }

    // 3. If there is more than 1 check, only legal move is to move the king
    for (Coord sign{-1, -1}; sign.y <= 1; ++sign.y) {
        Coord it{kPos.x, kPos.y + sign.y};
        if (!yInBounds(it.y)) {
            continue;
        }

        for (sign.x = -1; sign.x <= 1; ++sign.x) {
            it.x += sign.x;
            if (xInBounds(it.x) && canMove({kPos, it})) {
                return false;
            }
        }
    }
    return true;
}

State Position::updateState(Player pl) const noexcept {
    int plSign = getPlayerSign(pl);

    if (isCheckmate(pl)) {
        return CHECKMATE * plSign;
    }
    if (isStalemate(pl)) {
        return STALEMATE * plSign;
    }
    return PLAYING;
}

}

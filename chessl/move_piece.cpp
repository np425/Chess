#include "position.h"

namespace chess {

void Position::movePiece(const Path &move, PieceType promote) noexcept {
    Piece piece = m_board[move.origin];

    PieceType type = pieceToType(piece);
    int plSign = getPlayerSign(m_toMove);

    bool twoUp = false;

    // Update game metadata & handle m_passant
    switch (type) {
        case PAWN:
            // Update m_passant
            if (move.origin.y + 2 * plSign == move.target.y) { // Two up
                // Update m_passant: set it "behind" the move (-1 Y)
                twoUp = true;
                m_passant = {move.origin.x, move.origin.y + plSign};
            } else if (move.origin == m_passant) {
                // En m_passant capture
                m_board[{move.target.x, move.origin.y}] = VOID;
            }
            break;
        case KING:
            // Eliminate castling & Update king pos
            m_castlingPerms[m_toMove] = CASTLES_NONE;
            m_kingPos[m_toMove] = move.target;
            break;
        case ROOK:
            if (move.origin.x == 0) {
                // Left rook
                m_castlingPerms[m_toMove] &= ~CASTLES_QUEEN_SIDE;
            } else if (move.origin.x == literals::BOARD_SIZE_X - 1) {
                // Right rook
                m_castlingPerms[m_toMove] &= ~CASTLES_KING_SIDE;
            }
            break;
        default:
            break;
    }

    // Invalidate en m_passant opportunity
    if (!twoUp) {
        m_passant = literals::INVALID_COORD;
    }

    // Handle promotion
    Piece movePiece = (promote ? plSign * promote : piece);

    // Move the piece
    m_board[move.origin] = VOID;
    m_board[move.target] = movePiece;

    nextTurn();
}

void Position::castles(CastlingSide side) noexcept {
    Coord kPos = m_kingPos[m_toMove];
    Coord rPos{
        side * (literals::BOARD_SIZE_X - 1),
        m_toMove * (literals::BOARD_SIZE_Y - 1)
    };
    int xSign = (side ? 1 : -1);

    // Move king & rook to new squares
    m_board[{kPos.x + 2 * xSign, rPos.y}] = m_board[kPos];
    m_board[{kPos.x + xSign, rPos.y}] = m_board[rPos];

    // Invalidate old squares
    m_board[rPos] = VOID;
    m_board[kPos] = VOID;

    // Update king position
    m_kingPos[m_toMove] = {kPos.x + 2 * xSign, rPos.y};

    // Invalidate castling variables
    m_castlingPerms[m_toMove] = CASTLES_NONE;

    // Invalidate en m_passant opportunity
    m_passant = {-1, -1};

    nextTurn();
}

void Position::nextTurn() noexcept {
    // Update player
    m_toMove = static_cast<Player>(!m_toMove);

    // Update checks
    m_checks = getDefenders(m_kingPos[m_toMove], static_cast<Player>(!m_toMove));

    // Update game state for player
    m_state = updateState(m_toMove);
}

}

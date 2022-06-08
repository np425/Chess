#ifndef CHESSL_POSITION_H
#define CHESSL_POSITION_H

#include <utility>
//#include <vector>
#include "stack_vector.h"
#include "board.h"

// TODO: Vector vs StackVector performance

namespace chess {

constexpr unsigned MAX_MOVES_PER_SQUARE = 16;

enum StateType {
    PLAYING = 0,
    WIN,
    CHECKMATE,
    DRAW,
    STALEMATE
};

enum CastlingSide {
    CASTLES_QUEEN_SIDE = 0, // To the left
    CASTLES_KING_SIDE = 1, // To the right
    CASTLES_NONE = 2,
};

enum class CheckType {
    None,
    Check,
    Checkmate
};

struct MoveInfo {
    PieceType pieceType;
    PieceType promoteType;

    bool capture;
    Path path;

    CheckType checks;
    CastlingSide castles;
};

typedef int State;
typedef int CastlingPerms;

[[nodiscard]] constexpr StateType stateToType(State state) noexcept {
    return static_cast<StateType>(std::abs(state));
}

[[nodiscard]] constexpr Player stateToPlayer(State state) noexcept {
    if (state > 0) {
        return WHITE;
    } else if (state < 0) {
        return BLACK;
    }
    return PL_NONE;
}

typedef StackVector<Coord, MAX_MOVES_PER_SQUARE> CoordVector;

class Position {
protected:
    Board m_board{};

    Player m_toMove{};
    State m_state{};
    std::array<CastlingPerms, 2> m_castlingPerms{};
    Coord m_passant{};

    int m_fullMoves{};
    int m_halfMoves{};

    CoordVector m_checks;

    std::array<Coord, 2> m_kingPos{};

    /*
     * Checks if player is in stalemate
     * */
    [[nodiscard]] bool isStalemate(Player pl) const noexcept;

    /*
     * Checks if player is in checkmate
     * */
    [[nodiscard]] bool isCheckmate(Player pl) const noexcept;

    /*
     * Finds out current game state for a player
     */
    [[nodiscard]] State updateState(Player pl) const noexcept;

    /*
     * Checks if given move is a valid pawn move
     */
    [[nodiscard]] bool isValidPawnMove(const Path &move) const noexcept;

    /*
     * Checks if given move is pinned to the king
     */
    [[nodiscard]] bool isMovePinned(const Path &move) const noexcept;

    /*
     * Checks if given move prevents check, that is puts king in a safe position
     * */
    [[nodiscard]] bool doesMovePreventCheck(const Path &move) const noexcept;

    /*
     * Switches to next turn
     */
    void nextTurn() noexcept;

    /*
     * Attempts to find a valid piece which can move to the target square
     * Origin square coordinate (x or y) can be left unassigned, and will be attempted to be deduced
     * If no piece was found, or more than one piece can move to the target square then END_COORD is returned
     * */
    [[nodiscard]] Coord findValidMove(const MoveInfo &move, Player by) const noexcept;

    /*
     * Current player moves the piece from the origin coordinate to the target coordinate
     * Optionally promotion piece type can be given, which would replace the piece with it
     * Switches to next turn
     * */
    void movePiece(const Path &move, PieceType promote = VOID) noexcept;

    /*
     * Current player castles in the given side
     * Switches to next turn
     * */
    void castles(CastlingSide side) noexcept;

public:
    constexpr explicit Position() noexcept = default;

    constexpr explicit Position(const Board &board, Player toMove, State state,
                                const std::array<CastlingPerms, 2> &castlingPerms, const Coord &passant,
                                int fullMoves, int halfMoves) :
        m_board(board),
        m_state(state),
        m_castlingPerms(castlingPerms),
        m_passant(passant),
        m_fullMoves(fullMoves),
        m_halfMoves(halfMoves),
        m_toMove(toMove) {
    }

    /*
     * Validates current position and initializes meta data required for position processing
     * */
    bool validate() noexcept;

    /*
     * Checks if path from given coordinate the coordinate after moving is clear,
     * that is it treats the origin coordinate of the move as an empty square
     * Path must be diagonal, horizontal or vertical
     * */
    [[nodiscard]] bool isPathToMoveClear(const Coord &from, const Path &move) const noexcept;

    /*
     * Finds a defender for the target coordinate for the player,
     * starting from the given iterator coordinate,
     * and iterating through the board in increasing order
     *
     * Returns coordinate for a piece that can defend the target square, or END_COORD if there is none
     */
    [[nodiscard]] Coord findDefender(Coord it, const Coord &target, Player by) const noexcept;

    /*
     * Gets all defenders for the target square for the player
     * */
    [[nodiscard]] CoordVector getDefenders(const Coord &target, Player by) const noexcept;

    /*
     * Checks if piece in the origin square defends the target square
     * Path must be diagonal, vertical or horizontal
     */
    [[nodiscard]] bool defends(const Path &path) const noexcept;

    /*
     * Checks if a given piece can defend target square based on solely coordinate path
     * */
    [[nodiscard]] static bool couldDefend(Piece piece, const Path &path) noexcept;

    /*
     * Checks if path is clear, path must be diagonal, vertical or horizontal
     * */
    [[nodiscard]] bool isPathClear(const Path &path) const noexcept;

    /*
     * Checks if a piece can move to the target square
     */
    [[nodiscard]] bool canMove(const Path &move) const noexcept;

    /*
     * Checks if a player can castle on the given side
     */
    [[nodiscard]] bool canCastle(CastlingSide, Player) const noexcept;

    /*
     * Checks if a piece defends a move
     * that is it treats the origin coordinate of the move as an empty square
     */
    [[nodiscard]] bool defendsMove(const Coord &from, const Path &move) const noexcept;

    /*
     * Finds a defender for the move's target square for the player,
     * ignoring move's origin square
     * starting from the given iterator coordinate,
     * and iterating through the board in increasing order
     *
     * Returns coordinate of a piece that can defend the target square, or END_COORD if there is none
     */
    [[nodiscard]] Coord findMoveDefender(Coord it, const Path &move, Player by) const noexcept;

    /*
     * Finds a valid move to the target square for the player,
     * starting from the given iterator coordinate,
     * and iterating through the board in increasing order
     *
     * Returns coordinate of a piece that can move to the target square, or END_COORD if there is none
     */
    [[nodiscard]] Coord findValidMove(Coord it, const Coord &target, Player by) const noexcept;

    /*
     * Finds all the valid moves to the target square for a player
     */
    [[nodiscard]] CoordVector getMoves(const Coord &to, Player by) const noexcept;

    /*
     * Attempts to execute the move: can be castling or a simple piece move
     * For a simple piece move, one of the origin square coordinates (x or y)
     * can be left unassigned, it'll attempt to deduce the ambiguous coordinate
     *
     * Returns boolean value whether the move is valid in current position
     * */
    [[nodiscard]] virtual bool makeMove(MoveInfo &move) noexcept;

    /* Getters */
    [[nodiscard]] constexpr const Board& getBoard() const noexcept {
        return m_board;
    }

    [[nodiscard]] constexpr State getState() const noexcept {
        return m_state;
    }

    [[nodiscard]] constexpr bool isOver() const noexcept {
        return m_state != PLAYING;
    }

    [[nodiscard]] constexpr Player getToMove() const noexcept {
        return m_toMove;
    }
};

namespace literals {
constexpr Position STANDARD_POSITION(DEFAULT_BOARD, WHITE, PLAYING, {
    CASTLES_KING_SIDE | CASTLES_QUEEN_SIDE,
    CASTLES_KING_SIDE | CASTLES_QUEEN_SIDE
}, INVALID_COORD, 0, 0);
}

}

#endif//CHESSL_POSITION_H

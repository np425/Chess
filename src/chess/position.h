#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

/*
Chess position definitions: variables and mechanics
*/

// TODO: Move counter

#include "board.h"

namespace chess {

class Position : protected PositionInfo {
protected:
    Board board;

    /* Meta Information to improve processing speed */
    Coords checks; // Depends on current player

    /* Game State */
    void updateGameState();
    bool isStalemate(Player) const;
    bool isCheckmate(Player) const;
    void updateChecks(Player);

    bool findValidMove(MoveInfo &, Player) const; // Also updates MoveInfo

    bool isPathToMoveClear(const Coord &from, const Coord &moveFrom, const Coord &moveTo) const;
    bool isValidPawnMove(const Coord &from, const Coord &to) const;
    bool isMovePinned(const Coord &from, const Coord &to) const;
    bool doesMovePreventCheck(const Coord &from, const Coord &to) const;

    void castles(CastlingSide, Player);

    /* Turns */
    void nextTurn();

    /* Moves */
    void movePiece(const Coord &, const Coord &, PieceType promote = VOID);
    void castles(CastlingSide);

public:
    /* Position */
    const Board &getBoard() const;
    Player getPlayer() const;
    GameState getState() const;
    const CastlingPerms &getCastlingPerms(Player) const;
    const Coords &getChecks() const;
    Coord getPassant() const;

    void getMoves(const Coord &, Coords &, Player) const;
    bool canMove(const Coord &, const Coord &) const;
    bool canCastle(CastlingSide, Player) const;
    bool makeMove(MoveInfo &); // Also updates MoveInfo

    explicit Position(const Piece * = DEFAULT_BOARD, PositionInfo= {});

    void setBoard(const Piece *pieces);
    void setPositionInfo(const PositionInfo &info);

    /* Board */
    void setPosition(const Piece * = DEFAULT_BOARD, PositionInfo= {});
    bool isPathClear(const Coord &, const Coord &) const;

    /* Validation */
    bool validate();

    /* Defenders */
    void getDefenders(const Coord &, Coords &, Player) const;
    bool defends(const Coord &, const Coord &) const;

    bool makeMove(MoveInfo &, Player); // Also updates MoveInfo

};

/* Can piece defend regardless of current position (how pieces move) */
bool canDefend(Piece, const Coord &from, const Coord &to);

}

#endif

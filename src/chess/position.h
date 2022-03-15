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
	bool isStalemate(Player pl) const;
	bool isCheckmate(Player pl) const;
	void updateChecks(Player pl);

	bool findValidMove(MoveInfo& move, Player by) const; // Also updates MoveInfo
	bool isPathToMoveClear(const Coord& from, const Coord& moveFrom, const Coord& moveTo) const;
	bool isValidPawnMove(const Coord& from, const Coord& to) const;
	bool isMovePinned(const Coord& from, const Coord& to) const;
	bool doesMovePreventCheck(const Coord& from, const Coord& to) const;

	void castles(CastlingSide, Player);

	bool makeMove(MoveInfo& move, Player pl); // Also updates MoveInfo

	/* Turns */
	void nextTurn();

public:
	/* Position */
	const Board& getBoard() const;
	Player getPlayer() const;
	GameState getState() const;
	const CastlingPerms& getCastlingPerms(Player pl) const;
	const Coords& getChecks() const;
	Coord getPassant() const;
	bool isGameOver() const;

	void getMoves(const Coord& coord, Coords& arr, Player by) const;
	bool canMove(const Coord& from, const Coord& to) const;
	bool canCastle(CastlingSide side, Player pl) const;
	bool makeMove(MoveInfo& move); // Also updates MoveInfo

	explicit Position(Board={}, PositionInfo={});

	void changePositionInfo(const PositionInfo&);

	/* Board */
	void changePosition(Board={}, PositionInfo={});
	void changePosition(const Position& pos);
	bool isPathClear(const Coord&, const Coord&) const;

	/* Validation */
	bool validate();

	/* Defenders */
	void getDefenders(const Coord& coord, Coords& arr, Player by) const;
	bool defends(const Coord& from, const Coord& to) const;

	/* Moves */
	void movePiece(const Coord& from, const Coord& to, PieceType promote=VOID);
	void castles(CastlingSide side);
};

/* Can piece defend regardless of current position (how pieces move) */
bool canDefend(Piece piece, const Coord& from, const Coord& to);

}

#endif

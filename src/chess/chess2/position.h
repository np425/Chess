#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

/*
Chess position definitions: variables and mechanics
*/

#include "basic_types.h"
#include "utils.h"

namespace chess {

class Position {
	Piece board[BOARD_SIZE];
	Player toMove;
	GameState state;
	CastlingPerms castlePerms[2];

	/* Meta Information to improve processing speed */
	Coord kingPos[2];
	Coord passant;
	CoordArray checks;

	void updateChecks();

	/* Validation */
	bool findAndValidateKings();
	bool findAndValidateChecks();
	bool validatePawnsPlacement() const;
	bool validatePassant(const Player) const;

	/* Game State */
	void updateGameState();
	bool isStalemate(const Player pl) const;
	bool isCheckmate(const Player pl) const;

	/* Moves */
	bool findValidMove(MoveInfo& move, const Player by) const; // Also updates MoveInfo
	bool isPathToMoveClear(const Coord& from, const Coord& moveFrom, const Coord& moveTo) const;
	bool isValidPawnMove(const Coord& from, const Coord& to) const;
	bool isMovePinned(const Coord& from, const Coord& to) const;
	bool doesMovePreventCheck(const Coord& from, const Coord& to) const;

	void placePiece(const Coord& from, const Coord& to, const PieceType promote);
	void castles(const int side);
	bool makeMove(MoveInfo& move, const Player pl); // Also updates MoveInfo

	/* Turns */
	void nextTurn();

public:
	/* Getters */
	const Piece* getBoard() const;
	Player getPlayer() const;
	GameState getGameState() const;
	const CastlingPerms* getCastlingPerms() const;
	const CoordArray* getChecks() const;

	/* Validation */
	bool validate();

	/* Defenders */
	void getDefenders(const Coord& coord, CoordArray& arr, const Player by) const;
	bool defends(const Coord& from, const Coord& to) const;

	/* Board */
	bool isPathClear(const Coord&, const Coord&) const;

	/* Moves */
	void getMoves(const Coord& coord, CoordArray& arr, const Player by) const;
	bool canMove(const Coord& from, const Coord& to) const;
	bool canCastle(const CastlingSide side, const Player pl) const;
	bool makeMove(MoveInfo& move); // Also updates MoveInfo
};

/* Can piece defend regardless of current position (how pieces move) */
bool canDefend(const Piece piece, const Coord& from, const Coord& to);

}

#endif

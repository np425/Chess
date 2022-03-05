#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

/*
Chess position definitions: variables and mechanics
*/

// TODO: Move counter

#include "board.h"
#include "notation/move_notation.h"

namespace chess {

class Position {
	Board board;

	/* Positional variables */
	Player toMove;
	GameState state;
	CastlingPerms castlePerms[2];
	Coord passant;

	/* Meta Information to improve processing speed */
	CoordArray checks; // Depends on current player

	/* Validation */
	bool validateChecks();
	bool validatePassant(const Player) const;

	/* Game State */
	void updateGameState();
	bool isStalemate(const Player pl) const;
	bool isCheckmate(const Player pl) const;
	void updateChecks(const Player pl);

	/* Moves */
	bool findValidMove(MoveInfo& move, const Player by) const; // Also updates MoveInfo
	bool isPathToMoveClear(const Coord& from, const Coord& moveFrom, const Coord& moveTo) const;
	bool isValidPawnMove(const Coord& from, const Coord& to) const;
	bool isMovePinned(const Coord& from, const Coord& to) const;
	bool doesMovePreventCheck(const Coord& from, const Coord& to) const;

	void castles(const CastlingSide, const Player);

	bool makeMove(MoveInfo& move, const Player pl); // Also updates MoveInfo

	/* Turns */
	void nextTurn();

public:
	/* Position */
	const Board& getBoard() const;
	Player getPlayer() const;
	GameState getState() const;
	const CastlingPerms& getCastlingPerms(const Player pl) const;
	const CoordArray& getChecks() const;
	Coord getPassant() const;
	bool isGameOver() const;

	void getMoves(const Coord& coord, CoordArray& arr, const Player by) const;
	bool canMove(const Coord& from, const Coord& to) const;
	bool canCastle(const CastlingSide side, const Player pl) const;
	bool makeMove(MoveInfo& move); // Also updates MoveInfo
	bool makeMove(const char* notation);

	//Position(Board newBoard={}) : board(newBoard) {};
	Position(Board newBoard={});

	/* Board */
	void changeBoard(Board newBoard);
	bool isPathClear(const Coord&, const Coord&) const;

	/* Validation */
	bool validate();

	/* Defenders */
	void getDefenders(const Coord& coord, CoordArray& arr, const Player by) const;
	bool defends(const Coord& from, const Coord& to) const;

	/* Moves */
	void movePiece(const Coord& from, const Coord& to, const PieceType promote=VOID);
	void castles(const CastlingSide side);
};

/* Can piece defend regardless of current position (how pieces move) */
bool canDefend(const Piece piece, const Coord& from, const Coord& to);

}

#endif

#include "position.h"

namespace chess {

/* Getters */
const Board& Position::getBoard() const {
	return board;
}

Player Position::getPlayer() const {
	return toMove;
}

GameState Position::getState() const {
	return state;
}

const CoordArray& Position::getChecks() const {
	return checks;
}

const CastlingPerms& Position::getCastlingPerms(const Player pl) const {
	return castlePerms[pl];
}

Coord Position::getPassant() const {
	return passant;
}

Position::Position(Board newBoard) : board(newBoard) {
	toMove = FIRST_TO_MOVE;
	castlePerms[0] = QSIDE | KSIDE;
	castlePerms[1] = QSIDE | KSIDE;
	passant = {-1,-1};
}

void Position::changeBoard(Board newBoard) {
	board = newBoard;
	toMove = FIRST_TO_MOVE;
	castlePerms[0] = QSIDE | KSIDE;
	castlePerms[1] = QSIDE | KSIDE;
	passant = {-1,-1};
}

}

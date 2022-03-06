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

Position::Position(Board newBoard, PositionInfo info) : board(newBoard) {
	changePositionInfo(info);
}


Position::Position(const char* notation) {
	loadFEN(notation);
}

bool Position::loadFEN(const char* notation) {
	const char* it = notation;
	Board newBoard;
	PositionInfo newPosInfo;
	if (!readFEN(it, newBoard, newPosInfo)) {
		return false;
	}

	changeBoard(newBoard);
	changePositionInfo(newPosInfo);
	
	return true;
}

void Position::changeBoard(Board newBoard) {
	board = newBoard;
}

void Position::changePositionInfo(const PositionInfo& info) {
	toMove = info.toMove;
	castlePerms[0] = info.castlePerms[0];
	castlePerms[1] = info.castlePerms[1];
	passant = info.passant;
	state = info.state;
	fullMoves = info.fullMoves;
	halfMoves = info.halfMoves;
}

}

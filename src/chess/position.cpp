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

const Coords& Position::getChecks() const {
	return checks;
}

const CastlingPerms& Position::getCastlingPerms(Player pl) const {
	return castlePerms[pl];
}

Coord Position::getPassant() const {
	return passant;
}

Position::Position(const Piece* pieces, PositionInfo info) : board(pieces), PositionInfo(info) {
}

void Position::setBoard(const Piece *pieces) {
    board.setBoard(pieces);
}

void Position::setPosition(const Piece* pieces, PositionInfo info) {
    setBoard(pieces);
    setPositionInfo(info);
}

void Position::setPositionInfo(const PositionInfo& info) {
    PositionInfo::operator=(info);
}

}

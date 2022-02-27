#include "position.h"
#include "basic_types.h"

namespace chess {

/* Getters */
const Piece* Position::getBoard() const {
	return board;
}

Player Position::getPlayer() const {
	return toMove;
}

GameState Position::getGameState() const {
	return state;
}

const CastlingPerms* Position::getCastlingPerms() const {
	return castlePerms;
}

}

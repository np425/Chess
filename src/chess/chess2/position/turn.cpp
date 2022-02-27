#include "../position.h"

namespace chess {

void Position::updateChecks() {
	checks.clear();
	getDefenders(kingPos[toMove], checks, (Player)!toMove);
}

void Position::nextTurn() {
	// Update player
	toMove = (Player)!toMove;

	// Update checks
	updateChecks();

	// Update game state for player
	updateGameState();
}

}

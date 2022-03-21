#include "../position.h"
#include "utils.h"

namespace chess {
bool Position::validate() {
	if (!board.validate()) {
		return false;
	}

	// Checks
	for (int pl = WHITE; pl <= BLACK; ++pl) {
		updateChecks((Player)pl);
		if (!checks.empty() && pl != toMove) {
			// Opposite player of the one who is about to move cannot have a check
			return false;
		}
	}

	// Passant
	if (passant) {
		int sign = getPlayerSign(toMove);
	
		Piece piece = board[{passant.x, passant.y+sign}];
		PieceType type = pieceToType(piece);
		Player targetPl = pieceToPlayer(piece);

		// Piece in passant location has to be enemy pawn
		if (type != PAWN || targetPl != (Player)!toMove) {
			// Invalidate passant
            passant.invalidate();
		}
	}	

	updateGameState();
	return true;
}

}

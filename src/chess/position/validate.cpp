#include "../position.h"

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
	if (passant.x != -1) {
		int sign = getPlayerSign(toMove);
	
		int coord = (passant.y + sign) * BOARD_SIZE_X + passant.x;
		Piece piece = board[coord];
		PieceType type = pieceToType(piece);
		Player targetPl = pieceToPlayer(piece);

		// Piece in passant location has to be enemy pawn
		if (type != PAWN || targetPl != (Player)!toMove) {
			// Invalidate passant
			passant = {-1,-1};
		}
	}	

	updateGameState();
	return true;
}

}

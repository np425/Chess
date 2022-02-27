#include "../position.h"

namespace chess {

bool Position::findAndValidateChecks() {
	for (int pl = PL_WHITE; pl <= PL_BLACK; ++pl) {
		updateChecks();
		if (checks.isEmpty() && pl != toMove) {
			// Opposite player of the one who is about to move cannot have a check
			return false;
		} 
	}
	return true;
}

bool Position::validatePawnsPlacement() const {
	for (int side = 0; side <= 1; ++side) {
		int y = side * (BOARD_SIZE_Y - 1);
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			if (pieceToType(board[y*BOARD_SIZE_X+x]) == PT_PAWN) {
				return false; // Pawn on the last rank
			}
		}
	}

	return true;
}

bool Position::findAndValidateKings() {
	// Reset king positions
	kingPos[0] = {-1,-1};
	kingPos[1] = {-1,-1};

	// Find kings
	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			if (pieceToType(board[y*BOARD_SIZE_X+x]) != PT_KING) continue;

			Player kingOwner = pieceToPlayer(board[y*BOARD_SIZE_X+x]);

			if (kingPos[kingOwner].x != -1) {
				// Player has too many kings
				return false; 
			}
			
			kingPos[kingOwner] = {x,y};
		}
	}

	if (kingPos[0].y == -1 || kingPos[1].y == -1) {
		// Missing king for a player
		return false; 
	}

	// Make sure kings are one or more squares away from each other
	int dx = abs(kingPos[0].x - kingPos[1].x);
	int dy = abs(kingPos[0].y - kingPos[1].y);

	if (dx <= 1 && dy <= 1) {
		// Kings are too close
		return false; 
	}
		
	return true;
}

bool Position::validatePassant(const Player pl) const {
	if (passant.y == -1 || passant.x == -1) {
		// No passant
		return true; 
	}

	int sign = getPlayerSign(toMove);
	
	int coord = (passant.y + sign) * BOARD_SIZE_X + passant.x;
	Piece piece = board[coord];
	PieceType type = pieceToType(piece);
	Player targetPl = pieceToPlayer(piece);

	// Piece in passant location be enemy pawn
	return type == PT_PAWN && targetPl == (Player)!pl;
}

bool Position::validate() {
	if (!findAndValidateKings()) return false;

	if (!validatePawnsPlacement()) return false;

	if (!validatePassant(toMove)) return false;

	if (!findAndValidateChecks()) return false;

	updateGameState();
	return true;
}

}

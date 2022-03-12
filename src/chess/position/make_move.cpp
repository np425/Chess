#include "../position.h"

namespace chess {

bool Position::findValidMove(MoveInfo& move, const Player by) const {
	if (move.to.y == -1 || move.to.x == -1) {
		// Must be absolute target square coordinates
		return false; 
	}
	
	Coords moves;
	getMoves(move.to, moves, by);

	Coord* chosen = nullptr;
	for (auto it = moves.begin(); it < moves.end(); ++it) {
		// Filter the right move

		Piece piece = board[it->y * BOARD_SIZE_X + it->x];
		PieceType type = pieceToType(piece);

		if (type != move.type) {
			continue;
		}
		if (move.from.x != -1 && it->x != move.from.x) {
			continue;
		}
		if (move.from.y != -1 && it->y != move.from.y) {
			continue;
		}

		if (chosen) {
			// Ambiguous move
			return false; 
		}
		chosen = &(*it);
	}

	if (!chosen) {
		// No valid move found
		return false; 
	}

	move.from = {chosen->x, chosen->y};

	return true;
}

bool Position::makeMove(MoveInfo& move, const Player pl) {
	if (move.castles != CASTLES_NONE) {
		// Castling
		if (move.castles != CASTLES_QSIDE && move.castles != CASTLES_KSIDE) {
			// Unknown castling side
			return false; 
		}

		if (!canCastle(move.castles, pl)) {
			return false;
		}

		castles(move.castles);
	} else {
		// Other piece placement
		if (move.type == VOID) {
			return false;
		}

		if (!findValidMove(move, pl)) {
			// Could not find a move that's valid in current position
			return false;
		}

		if (board[move.to.y * BOARD_SIZE_X + move.to.x]) {
			move.capture = true;
		}	

		movePiece(move.from, move.to, move.promote);
	}

	// Updates move information
	if (stateToType(state) == CHECKMATE) {
		move.checks = CheckType::Checkmate;
	} else if (!checks.empty()) {
		move.checks = CheckType::Check;
	}

	// Update checks
	return true;
}

bool Position::makeMove(MoveInfo& move) {
	return makeMove(move, toMove);
}

}

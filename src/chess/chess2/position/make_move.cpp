#include "../position.h"

namespace chess {

bool Position::findValidMove(MoveInfo& move, const Player by) const {
	if (move.to.y == -1 || move.to.x == -1) {
		// Must be absolute target square coordinates
		return false; 
	}
	
	CoordArray moves;
	getMoves(move.to, moves, by);

	Coord* chosen = nullptr;
	for (Coord* it = moves.it(); it < moves.end(); ++it) {
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
		chosen = it;
	}

	if (!chosen) {
		// No valid move found
		return false; 
	}

	move.from = {chosen->x, chosen->y};

	return true;
}

bool Position::makeMove(MoveInfo& move, const Player pl) {
	if (move.castles != CS_NONE) {
		// Castling
		if (move.castles != QSIDE && move.castles != KSIDE) {
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
		move.checkmate = true;
	} else if (!checks.isEmpty()) {
		move.check = true;
	}

	// Update checks
	return true;
}

bool Position::makeMove(MoveInfo& move) {
	return makeMove(move, toMove);
}

bool Position::makeMove(const char* notation) {
	MoveInfo move;
	const char* it = notation;
	return readMoveNotation(it, move) && makeMove(move);
}

}

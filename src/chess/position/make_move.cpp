#include "make_move.h"
#include "moves.h" // canCastle, getMoves
#include "place_piece.h" // castles, placePiece
#include "../utils.h" // stateToType

namespace chess {

bool findValidMove(const Position& pos, MoveInfo& move) {
	if (move.to.y == -1 || move.to.x == -1) {
		return false; // Must be absolute target square coordinates
	}
	
	CoordArray moves;
	getMoves(pos, move.to, moves, pos.toMove);

	Coord* chosen = nullptr;
	for (Coord* it = moves.coords; it < moves.end; ++it) {
		// Filter the right move

		Piece piece = pos.board[it->y * BOARD_SIZE_X + it->x];
		PieceType type = pieceToType(piece);

		if (type != move.type) continue;
		if (move.from.y != -1 && it->y != move.from.y) continue;
		if (move.from.x != -1 && it->x != move.from.x) continue;

		if (chosen) return false; // Ambiguous move
		chosen = it;
	}

	if (!chosen) {
		return false; // No valid move found
	}

	move.from = {chosen->x, chosen->y};
	if (pos.board[move.to.y * BOARD_SIZE_X + move.to.x]) {
		move.capture = true;
	}	

	return true;
}

bool makeMove(Position& pos, MoveInfo& move) {
	if (move.castles[0] || move.castles[1]) {
		// Castling
		if (move.castles[0] && move.castles[1]) {
			return false; // Cannot castle in both sides
		}
		int side = (move.castles[0] ? 0 : 1);
		if (!canCastle(pos, side, pos.toMove)) return false;

		castles(pos, side);
	} else {
		// Other piece placement
		if (move.type == PT_NONE) return false;

		if (!findValidMove(pos, move)) {
			// Could not find a move that's valid in current position
			return false;
		}

		placePiece(pos, move.from, move.to, move.promote);
	}

	// Updates move information
	if (stateToType(pos.state) == GS_CHECKMATE) {
		move.checkmate = true;
	} else if (pos.checks.coords != pos.checks.end) {
		move.check = true;
	}

	// Update checks
	return true;
}

}

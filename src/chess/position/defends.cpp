#include "defends.h"
#include "../utils.h" // pieceToType
#include <cmath> // abs

namespace chess {

bool pathIsClear(const Piece* board, const Coord& from, const Coord& to) {
	int dx = to.x-from.x;
	int dy = to.y-from.y;

	int xSign = numSign(dx);
	int ySign = numSign(dy);

	int ix = from.x+xSign;
	int iy = from.y+ySign;

	while (ix != to.x || iy != to.y) {
		if (board[iy * BOARD_SIZE_X + ix]) {
			return false; // Blocking piece found
		}

		ix += xSign;
		iy += ySign;
	}

	return true;
}

bool canDefend(const Piece* board, const Coord& from, const Coord& to) {
	Piece piece = board[from.y*BOARD_SIZE_Y + from.x];
	PieceType type = pieceToType(piece);

	Player player = pieceToPlayer(piece);
	int sign = getPlayerSign(player);

	int dx = to.x-from.x;
	int dy = to.y-from.y;

	if (!dx && !dy) {
		// Can't defend itself
		return false;
	}

	// For rook, bishop and queen check if path is clear
	switch (type) {
		case PT_KING:
			return abs(dx) <= 1 && abs(dy) <= 1;
		case PT_ROOK:
			return !dx || !dy;
		case PT_BISHOP:
			return abs(dx) == abs(dy);
		case PT_QUEEN:
			return abs(dx) == abs(dy) || !dx || !dy;
		case PT_KNIGHT: 
			return abs(dx * dy) == 2;
		case PT_PAWN:
			return abs(dx) == 1 && dy == sign; 
		default:
			// Unknown piece type
			return false;
	}
}

bool defends(const Piece* board, const Coord& from, const Coord& to) {
	Piece piece = board[from.y * BOARD_SIZE_Y + from.x];
	PieceType type = pieceToType(piece);
	
	// Knight ignores path
	return canDefend(board, from, to) && (type == PT_KNIGHT || pathIsClear(board, from, to));
}

void getDefenders(const Piece* board, const Coord& coord, CoordArray& defenders, const Player by) {
	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			Piece target = board[y * BOARD_SIZE_X + x];
			Player targetPl = pieceToPlayer(target);
			if (targetPl != by) continue;

			if (defends(board, {x,y}, coord)) {
				*(defenders.end++) = {x,y};
			}
		}
	}
}

}


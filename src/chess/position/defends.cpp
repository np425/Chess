#include "../position.h"
#include "utils.h"

namespace chess {

bool Position::isPathClear(const Coord& from, const Coord& to) const {
	int dx = to.x-from.x;
	int dy = to.y-from.y;

	int xSign = numToSign(dx);
	int ySign = numToSign(dy);

    int x = from.x + xSign;
    int y = from.y + ySign;

	while (x != to.x || y != to.y) {
		if (board[{x,y}]) {
			return false; // Blocking piece found
		}

        x += xSign;
        y += ySign;
	}

	return true;
}

bool Position::defends(const Coord& from, const Coord& to) const {
	Piece piece = board[from];
	PieceType type = pieceToType(piece);
	
	// Knight ignores path
	return canDefend(piece, from, to) && (type == KNIGHT || isPathClear(from, to));
}

void Position::getDefenders(const Coord& coord, Coords& defenders, Player by) const {
	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			Piece target = board[{x, y}];
			Player targetPl = pieceToPlayer(target);
			if (targetPl != by) continue;

			if (defends({x,y}, coord)) {
				defenders.push_back({x,y});
			}
		}
	}
}

void Position::updateChecks(Player pl) {
	checks.clear();
	getDefenders(board.getKingPos(pl), checks, (Player)!pl);
}

/* Can piece defend regardless of current position (how pieces move) */
bool canDefend(Piece piece, const Coord& from, const Coord& to) {
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
		case KING:
			return abs(dx) <= 1 && abs(dy) <= 1;
		case ROOK:
			return !dx || !dy;
		case BISHOP:
			return abs(dx) == abs(dy);
		case QUEEN:
			return abs(dx) == abs(dy) || !dx || !dy;
		case KNIGHT: 
			return abs(dx * dy) == 2;
		case PAWN:
			return abs(dx) == 1 && dy == sign; 
		default:
			// Unknown piece type
			return false;
	}
}

}


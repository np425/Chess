#include "../position.h"

namespace chess {

// IDEA: Maybe rewrite Coord to a proper class?

bool Position::isCheckmate(const Player pl) const {
	CoordArray moves;
	unsigned checksAmount = checks.size();
	Coord kPos = board.getKingPos(pl);

	if (checksAmount == 0) return false;
	
	if (checksAmount == 1) {
		// 1. Attack the attacker (the one who checks)
		getMoves(checks[0], moves, pl);
		if (!moves.isEmpty()) return false;

		// Coordinates for checks
		int cy = checks[0].y;
		int cx = checks[0].x;

		// 2. Block the attacker (only if the check is not a knight or a pawn)
		int diffY = cy - kPos.y;
		int diffX = cx - kPos.x;

		int signY = numSign(diffY);
		int signX = numSign(diffX);

        int y = kPos.y + signY;
        int x = kPos.x + signX;

		// From king towards the check 
        for (int iy = y, ix = x; iy != cy && ix != cx; iy += signY, ix += signX) {	
			getMoves({iy,ix}, moves, pl);
			if (!moves.isEmpty()) return false;
		}
	}
    
	// 3. If there is more than 1 check, only legal move is to move the king
	for (int ySign = -1; ySign <= 1; ++ySign) {
		int y = kPos.y + ySign;
		if (y < 0 || y >= BOARD_SIZE_Y) continue;

		for (int xSign = -1; xSign <= 1; ++xSign) {
			if (xSign == 0 && ySign == 0) continue;

			int x = kPos.x + xSign;

			if (x < 0 || x >= BOARD_SIZE_X) continue;

			if (canMove(kPos, {x,y})) {
				return false;
			}
		}
    }
	return true;
}

bool Position::isStalemate(const Player pl) const {
	CoordArray moves;

	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			if (pieceToPlayer(board[y*BOARD_SIZE_X+x]) == pl) continue;

			getMoves({y,x}, moves, pl);
			if (!moves.isEmpty()) return false;
		}
	}
	return true;
}

void Position::updateGameState() {
	// Sign is from the perspective of the opponent
	int sign = getPlayerSign((Player)!toMove);

	if (isCheckmate(toMove)) {
		state = CHECKMATE;
	} else if (isStalemate(toMove)) {
		state = STALEMATE;
	} else {
		state = PLAYING;
	}

	state *= sign;
}

bool Position::isGameOver() const {
	return state != PLAYING;
}

}

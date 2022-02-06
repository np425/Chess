#include "gamestate.h"
#include "defends.h" // getDefenders
#include "moves.h" // getMoves
#include "../utils.h" // pieceToType, numSign, pieceToPlayer, getPlayerSign

namespace chess {

bool isCheckmate(const Position& pos, const Player pl) {
   	CoordArray moves;
   	unsigned checksAmount = pos.checks.end - pos.checks.coords;

   	if (checksAmount == 0) return false;
   	
	if (checksAmount == 1) {
       	// 1. Attack the attacker (the one who checks)
		getMoves(pos, pos.checks.coords[0], moves, pl);
       	if (moves.end != moves.coords) return false;

		// Coordinates for checks
		int cy = pos.checks.coords[0].y;
		int cx = pos.checks.coords[0].x;

       	// 2. Block the attacker (only if the check is not a knight or a pawn)
		//Piece piece = pos.board[cy * BOARD_SIZE_X + cx];
       	//PieceType checkType = pieceToType(piece);

       	//if (checkType != PT_KNIGHT && checkType != PT_PAWN) {
			int diffY = cy - pos.kingPos[pl].y;
			int diffX = cx - pos.kingPos[pl].x;

			int signY = numSign(diffY);
			int signX = numSign(diffX);

           	int y = pos.kingPos[pl].y + signY;
           	int x = pos.kingPos[pl].x + signX;

			// From king towards the check 
           	for (int iy = y, ix = x; iy != cy && ix != cx; iy += signY, ix += signX) {	
				getMoves(pos, {iy,ix}, moves, pl);
               	if (moves.end != moves.coords) return false;
           	}
		//}
	}
    
   	// 3. If there is more than 1 check, only legal move is to move the king
   	for (int ySign = -1; ySign <= 1; ++ySign) {
       	int y = pos.kingPos[pl].y + ySign;
       	if (y < 0 || y >= BOARD_SIZE_Y) continue;

       	for (int xSign = -1; xSign <= 1; ++xSign) {
           	if (xSign == 0 && ySign == 0) continue;

           	int x = pos.kingPos[pl].x + xSign;

			if (x < 0 || x >= BOARD_SIZE_X) continue;

			if (canMove(pos, pos.kingPos[pl], {x, y})) {
				return false;
			}
		}
    }
   	return true;
}

bool isStalemate(const Position& pos, const Player pl) {
   	CoordArray moves;

   	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
       	for (int x = 0; x < BOARD_SIZE_X; ++x) {
           	if (pieceToPlayer(pos.board[y*BOARD_SIZE_X+x]) == pl) continue;

			getMoves(pos, {y,x}, moves, pl);
           	if (moves.coords != moves.end) return false;
       	}
   	}
   	return true;
}

void updateGameState(Position& pos) {
   	// Sign is from the perspective of the opponent
   	int sign = getPlayerSign((Player)!pos.toMove);

   	if (isCheckmate(pos, pos.toMove)) {
		pos.state = GS_CHECKMATE;
   	} else if (isStalemate(pos, pos.toMove)) {
		pos.state = GS_STALEMATE;
   	} else {
		pos.state = GS_PLAYING;
	}

	pos.state *= sign;
}

}

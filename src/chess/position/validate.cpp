#include "validate.h"
#include "defends.h" // getDefenders
#include "gamestate.h" // updateGameState
#include <cmath> // abs
#include "../utils.h" // pieceToType

namespace chess {
bool findAndValidateChecks(const Position& pos, CoordArray& checks) {
	for (int pl = PL_WHITE; pl <= PL_BLACK; ++pl) {
		checks.end = checks.coords; // Reset head
		getDefenders(pos.board, pos.kingPos[pl], checks, (Player)!pl);
		if (checks.end != checks.coords) {
			// Opposite player of the one who is about to move cannot have a check
			if (pl != pos.toMove) return false;
		} 
	}
	return true;
}

bool validatePawnsPlacement(const Piece* board) {
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

bool findAndValidateKings(const Position& pos, Coord kingPos[2]) {
	kingPos[0] = {-1,-1};
	kingPos[1] = {-1,-1};

	// Find kings
	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			if (pieceToType(pos.board[y*BOARD_SIZE_X+x]) != PT_KING) continue;

			Player kingOwner = pieceToPlayer(pos.board[y*BOARD_SIZE_X+x]);

			if (pos.kingPos[kingOwner].x != -1) return false; // Player has too many kings
			
			kingPos[kingOwner] = {x,y};
		}
	}

	if (pos.kingPos[0].y == -1 || pos.kingPos[1].y == -1) {
		return false; // Missing king for a player
	}

	// Make sure kings are in legal range
	int dy = abs(pos.kingPos[0].y - pos.kingPos[1].y);
	int dx = abs(pos.kingPos[0].x - pos.kingPos[1].x);

	if (dy <= 1 && dx <= 1) return false; // Kings are too close
		
	return true;
}

bool validatePassant(const Position& pos, const Player pl) {
	if (pos.passant.y == -1 || pos.passant.x == -1) {
		return true; // No passant
	}

	int sign = getPlayerSign(pos.toMove);
	
	int coord = (pos.passant.y + sign) * BOARD_SIZE_X + pos.passant.x;
	Piece piece = pos.board[coord];
	PieceType type = pieceToType(piece);
	Player targetPl = pieceToPlayer(piece);

	return type == PT_PAWN && targetPl == (Player)!pl;
}

bool validatePosition(Position& pos) {
	if (!findAndValidateKings(pos, pos.kingPos)) return false;

	if (!validatePawnsPlacement(pos.board)) return false;

	if (!validatePassant(pos, pos.toMove)) return false;

	if (!findAndValidateChecks(pos, pos.checks)) return false;

	updateGameState(pos);
	return true;
}

}

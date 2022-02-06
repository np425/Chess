#include "place_piece.h"
#include "defends.h" // getDefenders
#include "gamestate.h" // updateGameState
#include "../utils.h" // pieceToType, getPlayerSign

namespace chess {
void placePiece(Position& pos, const Coord& from, const Coord& to, const PieceType promote) {
	int originCoord = from.y*BOARD_SIZE_X+from.x;
	Piece piece = pos.board[originCoord];

	PieceType type = pieceToType(piece);
	int plSign = getPlayerSign(pos.toMove);

	bool twoUp = false;

	// Update game metadata & handle passant
	switch (type) {
		case PT_PAWN: // Update passant
			if (from.y+2*plSign == to.y) { // Two up
				twoUp = true;
				// Update passant: set it "behind" the move (-1 Y)
				pos.passant = {from.x, from.y+plSign};
			} else if (pos.passant.y == from.y && pos.passant.x == to.x) { // En passant
				pos.board[from.y*BOARD_SIZE_X+to.x] = PL_NONE;
			}
			break;
		case PT_KING: // Eliminate castling and update king pos
			pos.canCastle[pos.toMove][0] = pos.canCastle[pos.toMove][1] = false;
			pos.kingPos[pos.toMove] = to; // Update king position
			break;
		case PT_ROOK:
			if (from.x == 0) {
				// Left rook
				pos.canCastle[pos.toMove][0] = false;
			} else if (from.x == BOARD_SIZE_X - 1) {
				// Right rook
				pos.canCastle[pos.toMove][1] = false;
			}
			break;
		default:
			break;
	}
	
	// Invalidate en passant opportunity
	if (!twoUp) pos.passant = {-1, -1};

	// Handle promotion
	Piece movePiece = (promote ? plSign * promote : piece);

	// Move the piece
	pos.board[to.y*BOARD_SIZE_X+to.x] = movePiece;
	pos.board[originCoord] = PT_NONE;

	// Update player to move
	pos.toMove = (Player)!pos.toMove;

	// Update for enemy: checks
	pos.checks.end = pos.checks.coords; // Reset head
	getDefenders(pos.board, pos.kingPos[pos.toMove], pos.checks, (Player)!pos.toMove);

	// Update game state for player
	updateGameState(pos);
}

void castles(Position& pos, const int side) {
	int rx = side * (BOARD_SIZE_X - 1);
	int ry = pos.toMove * (BOARD_SIZE_Y - 1);
	int xSign = (side ? 1 : -1);

	int kCoord = ry*BOARD_SIZE_X+pos.kingPos[pos.toMove].x;
	int rCoord = ry*BOARD_SIZE_X+rx;

	// Move pieces
	pos.board[kCoord + xSign] = pos.board[rCoord];
	pos.board[kCoord + 2 * xSign] = pos.board[kCoord];
	pos.board[rCoord] = PT_NONE;
	pos.board[kCoord] = PT_NONE;

	// Update king position
	pos.kingPos[pos.toMove].x += (2 * xSign);

	// Invalidate castling variables
	pos.canCastle[pos.toMove][0] = false;
	pos.canCastle[pos.toMove][1] = false;

	// Invalidate en passant opportunity
	pos.passant = {-1,-1};

	// Update player
	pos.toMove = (Player)!pos.toMove;

	// Update checks
	pos.checks.end = pos.checks.coords; // Reset head
	getDefenders(pos.board, pos.kingPos[pos.toMove], pos.checks, (Player)!pos.toMove);

	// Update game state for player
	updateGameState(pos);
}

}

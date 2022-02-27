#include "../position.h"

namespace chess {

void Position::placePiece(const Coord& from, const Coord& to, const PieceType promote) {
	int originCoord = from.y*BOARD_SIZE_X+from.x;
	Piece piece = board[originCoord];

	PieceType type = pieceToType(piece);
	int plSign = getPlayerSign(toMove);

	bool twoUp = false;

	// Update game metadata & handle passant
	switch (type) {
		case PT_PAWN: 
			// Update passant
			if (from.y+2*plSign == to.y) { // Two up
				// Update passant: set it "behind" the move (-1 Y)
				twoUp = true;
				passant = {from.x, from.y+plSign};
			} else if (passant.y == from.y && passant.x == to.x) { 
				// En passant capture
				board[from.y*BOARD_SIZE_X+to.x] = PL_NONE;
			}
			break;
		case PT_KING: 
			// Eliminate castling and update king position
			castlePerms[toMove] = CS_NONE;
			kingPos[toMove] = to; // Update king position
			break;
		case PT_ROOK:
			if (from.x == 0) {
				// Left rook
				castlePerms[toMove] &= ~CS_QSIDE;
			} else if (from.x == BOARD_SIZE_X - 1) {
				// Right rook
				castlePerms[toMove] &= ~CS_KSIDE;
			}
			break;
		default:
			break;
	}
	
	// Invalidate en passant opportunity
	if (!twoUp) passant = {-1, -1};

	// Handle promotion
	Piece movePiece = (promote ? plSign * promote : piece);

	// Move the piece
	board[to.y*BOARD_SIZE_X+to.x] = movePiece;
	board[originCoord] = PT_NONE;

	nextTurn();
}

void Position::castles(const int side) {
	int rx = side * (BOARD_SIZE_X - 1);
	int ry = toMove * (BOARD_SIZE_Y - 1);
	int xSign = (side ? 1 : -1);

	int kCoord = ry*BOARD_SIZE_X+kingPos[toMove].x;
	int rCoord = ry*BOARD_SIZE_X+rx;

	// Move pieces
	board[kCoord + xSign] = board[rCoord];
	board[kCoord + 2 * xSign] = board[kCoord];
	board[rCoord] = PT_NONE;
	board[kCoord] = PT_NONE;

	// Update king position
	kingPos[toMove].x += (2 * xSign);

	// Invalidate castling variables
	castlePerms[toMove] = CS_NONE;

	// Invalidate en passant opportunity
	passant = {-1,-1};

	nextTurn();
}

}

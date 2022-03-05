#include "../position.h"

namespace chess {

void Position::movePiece(const Coord& from, const Coord& to, const PieceType promote) {
	Piece piece = board[from.y*BOARD_SIZE_X+from.x];

	PieceType type = pieceToType(piece);
	int plSign = getPlayerSign(toMove);

	bool twoUp = false;

	// Update game metadata & handle passant
	switch (type) {
		case PAWN: 
			// Update passant
			if (from.y+2*plSign == to.y) { // Two up
				// Update passant: set it "behind" the move (-1 Y)
				twoUp = true;
				passant = {from.x, from.y+plSign};
			} else if (passant.y == from.y && passant.x == to.x) { 
				// En passant capture
				board.clearPiece({from.y, to.x});
			}
			break;
		case KING: 
			// Eliminate castling
			castlePerms[toMove] = CS_NONE;
			break;
		case ROOK:
			if (from.x == 0) {
				// Left rook
				castlePerms[toMove] &= ~QSIDE;
			} else if (from.x == BOARD_SIZE_X - 1) {
				// Right rook
				castlePerms[toMove] &= ~KSIDE;
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
	board.clearPiece(from);
	board.movePiece(movePiece, to);

	nextTurn();
}

void Position::castles(const CastlingSide side) {
	Coord kPos = board.getKingPos(toMove);

	int rx = side * (BOARD_SIZE_X - 1);
	int ry = toMove * (BOARD_SIZE_Y - 1);
	int xSign = (side ? 1 : -1);

	int kCoord = ry*BOARD_SIZE_X+kPos.x;
	int rCoord = ry*BOARD_SIZE_X+rx;

	// Move pieces
	board.movePiece(kCoord, {kCoord + 2 * xSign, ry});
	board.movePiece(rCoord, {kCoord + xSign, ry});

	// Invalidate castling variables
	castlePerms[toMove] = CS_NONE;

	// Invalidate en passant opportunity
	passant = {-1,-1};

	nextTurn();
}

void Position::nextTurn() {
	// Update player
	toMove = (Player)!toMove;

	// Update checks
	updateChecks(toMove);

	// Update game state for player
	updateGameState();
}

}

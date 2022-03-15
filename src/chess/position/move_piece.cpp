#include "../position.h"

namespace chess {

void Position::movePiece(const Coord& from, const Coord& to, PieceType promote) {
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
			castlePerms[toMove] = CASTLES_NONE;
			break;
		case ROOK:
			if (from.x == 0) {
				// Left rook
				castlePerms[toMove] &= ~CASTLES_QSIDE;
			} else if (from.x == BOARD_SIZE_X - 1) {
				// Right rook
				castlePerms[toMove] &= ~CASTLES_KSIDE;
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
	board.placePiece(movePiece, to);

	nextTurn();
}

void Position::castles(CastlingSide side, Player pl) {
    Coord kPos = board.getKingPos(pl);

    int rx = side * (BOARD_SIZE_X - 1);
    int ry = pl * (BOARD_SIZE_Y - 1);
    int xSign = (side ? 1 : -1);

    // Move pieces
    board.movePiece(kPos, {kPos.x + 2 * xSign, ry});
    board.movePiece({rx,ry}, {kPos.x + xSign, ry});

    // Invalidate castling variables
    castlePerms[pl] = CASTLES_NONE;
}

void Position::castles(CastlingSide side) {
    castles(side, toMove);

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

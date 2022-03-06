#include "board.h"

namespace chess {

const Piece* Board::getBoard() const {
	return board;
}

Coord Board::getKingPos(const Player pl) const {
	return kingPos[pl];
}

Piece Board::operator[](unsigned idx) const {
	return board[idx];
}

Board::Board(const Piece* pieces) {
	changeBoard(pieces);
}

void Board::changeBoard(const Piece* newBoard) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		board[i] = newBoard[i];
	}	
}

void Board::movePiece(Piece piece, const Coord& to) {
	clearPiece(to);

	PieceType pt = pieceToType(piece);
	Player pl = pieceToPlayer(piece);

	board[to.y * BOARD_SIZE_X + to.x] = piece;

	if (pt == KING) {
		kingPos[pl] = to;
	}
}

void Board::movePiece(const Coord& from, const Coord& to) {
	movePiece(board[from.y * BOARD_SIZE_X + from.x], to);
	board[from.y * BOARD_SIZE_X + from.x] = VOID;
}

void Board::clearPiece(const Coord& coord) {
	Piece piece = board[coord.y * BOARD_SIZE_X + coord.x];
	PieceType pt = pieceToType(piece);
	Player pl = pieceToPlayer(piece);

	if (pt == KING) {
		kingPos[pl] = {-1,-1};
	}

	board[coord.y * BOARD_SIZE_X + coord.x] = VOID;
}

bool Board::validate() {
	// Pawns on the last rank
	for (int pl = 0; pl <= 1; ++pl) {
		int y = pl * (BOARD_SIZE_Y - 1);
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			if (pieceToType(board[y * BOARD_SIZE_X + x]) == PAWN) {
				return false;
			}
		}
	}

	// King positions
	kingPos[0] = {-1,-1};
	kingPos[1] = {-1,-1};

	// Find kings
	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			if (pieceToType(board[y*BOARD_SIZE_X+x]) != KING) {
				continue;
			}

			Player kingOwner = pieceToPlayer(board[y*BOARD_SIZE_X+x]);

			if (kingPos[kingOwner].x != -1) {
				// Player has too many kings
				return false; 
			}
			
			kingPos[kingOwner] = {x,y};
		}
	}

	if (kingPos[0].x == -1 || kingPos[1].x == -1) {
		// Missing king for a player
		return false; 
	}

	// Make sure kings are one or more squares away from each other
	int dx = abs(kingPos[0].x - kingPos[1].x);
	int dy = abs(kingPos[0].y - kingPos[1].y);

	if (dx <= 1 && dy <= 1) {
		// Kings are too close
		return false; 
	}
		
	return true;
}

}

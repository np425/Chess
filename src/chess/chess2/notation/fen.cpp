#include "fen.h"
#include "shared_notation.h" // readPositiveInteger, charToPieceType, readCoord
#include <cctype> // isupper, tolower, isspace

namespace chess {
unsigned readFENSquare(const char*& it, Piece& piece) {
	unsigned char chr = toupper(*it);

	if (!charToPieceType(chr, (PieceType&)piece)) {
		// If not a piece, then must be empty squares
		if (chr < '1' || chr > '8') return 0;

		piece = PT_NONE;
		++it;
		return chr - '0';
	}
	
	// A piece
	int sign = (isupper(*it) ? 1 : -1);

	piece *= sign;
	++it;
	return 1;
}
	
bool readBoard(const char*& it, Piece* board) {
	// Board reading from white's perspective
	for (int y = BOARD_SIZE_Y - 1; y >= 0; --y) {
		for (int x = 0; x < BOARD_SIZE_X; ) {
			Piece& piece = board[y*BOARD_SIZE_X+x];
			unsigned read = readFENSquare(it, piece);

			if (!read || x + read > BOARD_SIZE_X) {
				return false; // Unable to read a piece or too many pieces read
			}

			// Populate board with appropriate amount of pieces
			// First square already set
			--read;
			++x;

			while (read != 0) {
				board[y*BOARD_SIZE_X+x] = piece;
				++x;
				--read;
			}
		}
		// Skip whitespaces and slashes (slashes are optional)
		while (isspace(*it) || *it == '/') ++it;
	}		
	return true;
}

bool readPlayer(const char*& it, Player& player) {
	switch (tolower(*it)) {
		case 'w':
			player = PL_WHITE;
			break;
		case 'b':
			player = PL_BLACK;
			break;
		default:
			return false;
	}

	++it;
	return true;
}

bool readCastlingRights(const char*& it, bool canCastle[2][2]) {
	// Invalidate castling
	canCastle[0][0] = false;
	canCastle[0][1] = false;
	canCastle[1][0] = false;
	canCastle[1][1] = false;

	if (*it == '-') { // No castling 
		++it;
		return true; 
	}

	bool foundOne = false;
	while (true) {
		Player player = (isupper(*it) ? PL_WHITE : PL_BLACK);
		unsigned char chr = tolower(*it);

		int side;

		switch (chr) {
			case 'k':
				side = 1;
				foundOne = true;
				break;
			case 'q':
				side = 0;
				foundOne = true;
				break;
			default:
				// Make sure there is at least one castling right if no - is given
				return foundOne;
		}

		// Allows repetitive castling rights
		canCastle[player][side] = true;
		++it;
	}	
}

bool readPassant(const char*& it, Coord& sqr) {
	sqr = {-1, -1};

	if (*it == '-') { // No passant
		++it;
		return true;
	}

	return readX(it, sqr.x) && readY(it, sqr.y);
}

bool readFENPosition(const char*& it, Position& pos) {
	while (isspace(*it)) ++it; // Skip whitespace

	// 1. Board
	if (!readBoard(it, pos.board)) return false;
	while (isspace(*it)) ++it;

	// 2. Player to move
	if (!readPlayer(it, pos.toMove)) return false;
	while (isspace(*it)) ++it;
				
	// 3. Castling rights
	if (!readCastlingRights(it, pos.canCastle)) return false;
	while (isspace(*it)) ++it;

	// 4. Passant
	if (!readPassant(it, pos.passant)) return false;
	
	return true;
}

bool readFEN(const char*& it, Position& pos, Count& count) {
	// 1. Board
	// 2. Player to move
	// 3. Castling rights
	// 4. Passant
	if (!readFENPosition(it, pos)) return false;
	while (isspace(*it)) ++it; 
			
	// 5. Half moves
	if (!readPositiveInteger(it, count.half)) return false;
	while (isspace(*it)) ++it;

	// 6. Full moves
	unsigned fullMoves;
	if (!readPositiveInteger(it, fullMoves)) return false;

	// Convert full moves to all moves
	count.moves = (fullMoves-1) * 2; 
	if (pos.toMove == PL_BLACK) {
		// If player to move is black, that means white has already moved
		++count.moves;
	}

	return true;
}

}

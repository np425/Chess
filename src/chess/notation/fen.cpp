#include "fen.h"
#include "shared_notation.h" // readPositiveInteger, charToPieceType, readCoord
#include <cctype> // isupper, tolower, isspace

namespace chess {

unsigned readFENSquare(const char*& it, Piece& piece) {
	unsigned char chr = toupper(*it);

	if (!charToPieceType(chr, (PieceType&)piece)) {
		// If not a piece, then must be empty squares
		if (chr < '1' || chr > '8') return 0;

		piece = VOID;
		++it;
		return chr - '0';
	}
	
	// A piece
	int sign = (isupper(*it) ? 1 : -1);

	piece *= sign;
	++it;
	return 1;
}
	
bool readBoard(const char*& it, Board& board) {
	// Board reading from white's perspective
	for (int y = BOARD_SIZE_Y - 1; y >= 0; --y) {
		for (int x = 0; x < BOARD_SIZE_X; ) {
			Piece piece;
			unsigned read = readFENSquare(it, piece);

			if (!read || x + read > BOARD_SIZE_X) {
				return false; // Unable to read a piece or too many pieces read
			}

			// Populate board with appropriate amount of pieces
			while (read != 0) {
				board.movePiece(piece, {x,y});
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
			player = WHITE;
			break;
		case 'b':
			player = BLACK;
			break;
		default:
			return false;
	}

	++it;
	return true;
}

bool readCastlingRights(const char*& it, CastlingPerms castlePerms[2]) {
	// Invalidate castling
	castlePerms[0] = CS_NONE;
	castlePerms[1] = CS_NONE;

	if (*it == '-') { // No castling 
		++it;
		return true; 
	}

	bool foundOne = false;
	while (true) {
		Player player = (isupper(*it) ? WHITE : BLACK);
		unsigned char chr = tolower(*it);

		CastlingSide side;

		switch (chr) {
			case 'k':
				side = KSIDE;
				foundOne = true;
				break;
			case 'q':
				side = QSIDE;
				foundOne = true;
				break;
			default:
				// Make sure there is at least one castling right if no - is given
				return foundOne;
		}

		// Allows repetitive castling rights
		castlePerms[player] |= side;
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

bool readFEN(const char*& it, Board& board, PositionInfo& pos) {
	while (isspace(*it)) {
		++it; 
	}

	// 1. Board
	if (!readBoard(it, board)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}

	// 2. Player to move
	if (!readPlayer(it, pos.toMove)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}
				
	// 3. Castling rights
	if (!readCastlingRights(it, pos.castlePerms)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}

	// 4. Passant
	if (!readPassant(it, pos.passant)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}
			
	// 5. Half moves
	if (!readPositiveInteger(it, pos.halfMoves)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}

	// 6. Full moves
	if (!readPositiveInteger(it, pos.fullMoves)) {
		return false;
	}

	return true;
}

}

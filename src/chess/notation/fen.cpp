#include "fen.h"
#include "char_types.h"
#include <cctype>

namespace chess {

Position& FENParser::getPos() {
	return pos;
}

int FENParser::readSquare(Piece& piece) {
	piece = charToType(*it);
	if (!piece) {
		// Empty squares
		if (*it < '1' || *it > '8') {
			return 0;
		}

		++it;
		return *it - '0';
	}
	
	// A piece
	int sign = numToSign(piece);
	piece *= sign;

	++it;
	return 1;
}

bool FENParser::readBoard(Board& board) {
	// Board reading from white's perspective
	for (int y = BOARD_SIZE_Y - 1; y >= 0; --y) {
		for (int x = 0; x < BOARD_SIZE_X; ) {
			Piece piece;
			int read = readSquare(piece);

			if (!read || x + read > BOARD_SIZE_X) {
				// Unable to read a piece or too many pieces read
				return false;
			} 

			// Populate board with appropriate amount of pieces
			while (read != 0) {
				board.placePiece(piece, {x,y});
				++x;
				--read;
			}
		}

		// Skip whitespaces and slashes (slashes are optional)
		while (isspace(*it) || *it == '/') {
			++it;
		}
	}		
	return true;
}

int FENParser::readCastlingRights(CastlingPerms perms[2]) {
	// Invalidate castling
	perms[WHITE] = CASTLES_NONE;
	perms[BLACK] = CASTLES_NONE;

	if (*it == '-') { 
		// No castling 
		++it;
		return 1; 
	}

	int read = 0;
	while (true) {
		CastlingSide side = charToCastlingSide(*it);

		if (side == CASTLES_NONE) {
			return read;
		}

		Player player = (isupper(*it) ? WHITE : BLACK);

		if (perms[player] & side) {
			// No repetitive castling rights
			return -1;
		}

		perms[player] |= side;
		++it;
		++read;
	}	
}

bool FENParser::readPassant(Coord& sqr) {
	// Invalidate passant
	sqr = {-1, -1};

	if (*it == '-') { // No passant
		++it;
		return true;
	}

	return readX(sqr.x) && readY(sqr.y);
}

bool FENParser::parse() {
	validExpr = false;

	Board board;
	PositionInfo posInfo;

	while (isspace(*it)) {
		++it; 
	}

	// 1. Board
	if (!readBoard(board)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}

	// 2. Player to move
	if (!readPlayer(posInfo.toMove)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}
				
	// 3. Castling rights
	if (readCastlingRights(posInfo.castlePerms) < 1) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}

	// 4. Passant
	if (!readPassant(posInfo.passant)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}
			
	// 5. Half moves
	if (!readUnsignedInt(posInfo.halfMoves)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}

	// 6. Full moves
	if (!readUnsignedInt(posInfo.fullMoves)) {
		return false;
	}

	pos.changePosition(board, posInfo);
	validExpr = true;

	return validExpr;
}

}


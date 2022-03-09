#include "fen.h"
#include <cctype> 

namespace chess {

ExprEval readFENSquare(const char*& it, Piece& piece, unsigned& count) {
	piece = charToType(*it);
	if (!piece) {
		// Empty squares
		if (*it < '1' || *it > '8') {
			return ExprEval::None;
		}

		++it;
		count = *it - '0';
		return ExprEval::Valid;
	}
	
	// A piece
	int sign = numToSign(piece);
	piece *= sign;

	++it;

	return ExprEval::Valid;
}
	
ExprEval readBoard(const char*& it, Board& board) {
	bool readSquare = false;

	// Board reading from white's perspective
	for (int y = BOARD_SIZE_Y - 1; y >= 0; --y) {
		for (int x = 0; x < BOARD_SIZE_X; ) {
			Piece piece;
			unsigned read;
			ExprEval exprEval = readFENSquare(it, piece, read);

			if (exprEval != ExprEval::Valid || x + read > BOARD_SIZE_X) {
				// Unable to read a piece or too many pieces read
				if (!readSquare && exprEval == ExprEval::None) {
					return ExprEval::None;
				} else {
					return ExprEval::Malformed;
				}	
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
	return ExprEval::Valid;
}

ExprEval readPlayer(const char*& it, Player& player) {
	switch (tolower(*it)) {
		case 'w':
			player = WHITE;
			break;
		case 'b':
			player = BLACK;
			break;
		default:
			return ExprEval::None;
	}

	++it;
	return ExprEval::Valid;
}

ExprEval readCastlingRights(const char*& it, CastlingPerms castlePerms[2]) {
	// Invalidate castling
	castlePerms[0] = CASTLES_NONE;
	castlePerms[1] = CASTLES_NONE;

	if (*it == '-') { 
		// No castling 
		++it;
		return ExprEval::Valid; 
	}

	bool foundOne = false;
	while (true) {
		Player player = (isupper(*it) ? WHITE : BLACK);

		CastlingSide side = charToCastlingSide(*it);

		if (side == CASTLES_NONE) {
			return (foundOne ? ExprEval::Valid : ExprEval::Malformed);
		}

		// Allows repetitive castling rights
		castlePerms[player] |= side;
		++it;
	}	
}

ExprEval readPassant(const char*& it, Coord& sqr) {
	// Invalidate passant
	sqr = {-1, -1};

	if (*it == '-') { // No passant
		++it;
		return ExprEval::Valid;
	}

	if (readX(it, sqr.x) != ExprEval::Valid) {
		return ExprEval::Malformed;
	}

	if (readY(it, sqr.y) != ExprEval::Valid) {
		return ExprEval::Malformed;
	}

	return ExprEval::Valid;
}

ExprEval readFEN(const char*& it, Board& board, PositionInfo& pos) {
	while (isspace(*it)) {
		++it; 
	}

	// 1. Board
	ExprEval exprEval = readBoard(it, board);
	if (exprEval != ExprEval::Valid) {
		return exprEval;
	}
	while (isspace(*it)) {
		++it;
	}

	// 2. Player to move
	exprEval = readPlayer(it, pos.toMove);
	if (exprEval != ExprEval::Valid) {
		return ExprEval::Malformed;
	}
	while (isspace(*it)) {
		++it;
	}
				
	// 3. Castling rights
	exprEval = readCastlingRights(it, pos.castlePerms);
	if (exprEval != ExprEval::Valid) {
		return ExprEval::Malformed;
	}
	while (isspace(*it)) {
		++it;
	}

	// 4. Passant
	exprEval = readPassant(it, pos.passant);
	if (exprEval != ExprEval::Valid) {
		return ExprEval::Malformed;
	}
	while (isspace(*it)) {
		++it;
	}
			
	// 5. Half moves
	exprEval = readInteger(it, pos.halfMoves);
	if (exprEval != ExprEval::Valid) {
		return ExprEval::Malformed;
	}
	while (isspace(*it)) {
		++it;
	}

	// 6. Full moves
	exprEval = readInteger(it, pos.fullMoves);
	if (exprEval != ExprEval::Valid) {
		return ExprEval::Malformed;
	}

	return ExprEval::Valid;
}

}

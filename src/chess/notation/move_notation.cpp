#include "move_notation.h"
#include "char_types.h"
#include <cctype> 

namespace chess {

MoveParser::MoveParser(MoveInfo* move, const char* expr) : BasicNotationParser(expr), move(move) {
}

MoveInfo* MoveParser::getMove() const {
	return move;
}

bool MoveParser::readPromoteType(PieceType& promote) {
	// Letter case here does not matter, because there's no ambiguity
	promote = charToPromoteType(*it);
	if (!promote) {
		return false;
	}

	++it;
	return true;
}

bool MoveParser::readPromoteSymbol() {
	return readChar('=');
}

int MoveParser::readCoordPoints(Coord& coord) {
	int read = 0;
	if (readX(coord.x)) {
		++read;
	}
	if (readY(coord.y)) {
		++read;
	}
	return read;
}
	
bool MoveParser::readChecks(CheckType& checks) {
	if (readChar('+')) {
		checks = CheckType::Check;
	} else if (readChar('#')) {
		checks = CheckType::Checkmate;
	} else {
        checks = CheckType::None;
		return false;
	}
	return true;
}

int MoveParser::readComment() {
	if (readChar(';')) {
		// Comment to the end of line
		while (*it != '\n' && *it) {
			++it;
		}
	} else if (readChar('{')) {
		// Comment to the next curly brace, wraps
		while (*it != '}' && *it) {
			++it;
		}
		if (!readChar('}')) {
			// Missing ending }
			return -1; 
		}
	} else {
		return 0;
	}

	return 1;
}	

bool MoveParser::readCaptureSymbol() {
	return readChar('x');
}

bool MoveParser::readCastling(CastlingSide& side) {
	// 0-0->(+,#); 0-0-0->(+,#)
	if (!readInsensitiveString("O-O")) {
		return false;
	}

	if (!readInsensitiveString("-O")) {
		side = CASTLES_KSIDE;
		return false; 
	}

	side = CASTLES_QSIDE;
	return true;
}

bool MoveParser::readMove(MoveInfo& theMove) {
    theMove.castles = CASTLES_NONE;
    theMove.capture = false;
	
	// Invalidate coordinates
	theMove.from = {-1, -1};
    theMove.to = {-1, -1};

	// For keeping track if given piece symbolic
	bool explicitPiece = true; 

	// Piece names have to be in uppercase to avoid ambiguity with pawns
	theMove.type = charToType(*it);

	if (islower(*it) || !theMove.type) {
		// If not a piece symbol then assume either a pawn theMove or castles
		theMove.type = PAWN;
		explicitPiece = false;
	} else {
		// Skip piece character
		++it; 
	}

	int read = readCoordPoints(theMove.from);
	if (read > 0) {
		if (readX(theMove.to.x)) {
			return readY(theMove.to.y);
		} else if (readCaptureSymbol()) {
            theMove.capture = true;
			return readX(theMove.to.x) && readY(theMove.to.y);
		} else if (read == 2) {
            theMove.to = theMove.from;
            theMove.from = {-1, -1};
		} else {
			// Malformed piece
			return false;
		}
	} else if (explicitPiece && readCaptureSymbol()) {
        theMove.capture = true;
		return readX(theMove.to.x) && readY(theMove.to.y);
	} else if (!explicitPiece) {
		// If not marked with piece, then it has to be castling
		return readCastling(theMove.castles);
	} else {
		// No piece
		return false;
	}

	return true;
}	

bool MoveParser::parse() {
	validExpr = false;

	while (isspace(*it)) {
		++it;
	}
		
	// 1. Move: piece or castling notation
	if (!readMove(*move)) {
		return false; 
	}
		
	// 2. Optional promotion for pawn
	if (readPromoteSymbol()) {
		if (move->type != PAWN || !readPromoteType(move->promote)) {
			return false;
		}
	} else {
		move->promote = VOID;
	}
		
	// 3. Optional checks (check, checkmate notation)
	readChecks(move->checks);

	// 4. Optional comment
	const char* tempIt = it;
	while (isspace(*it)) {
		++it;
	}

	int read = readComment();
	if (read <= 0) {
		if (read == -1) {
			return false;
		}
		it = tempIt;
	}

	validExpr = true;
	return validExpr;
}	

}

#include "move_notation.h"
#include "shared_notation.h" // readX, readY, charToPieceType, readCoord
#include <cctype> // tolower, isspace

namespace chess {

bool readPromoteType(const char*& it, PieceType& promote) {
	// Letter case here does not matter, because there's no ambiguity
	switch (toupper(*it)) {
		case 'R':
			promote = ROOK;
			break;
		case 'B':
			promote = BISHOP;
			break;
		case 'N':
			promote = KNIGHT;
			break;
		case 'Q':
			promote = QUEEN;
			break;
		default:
			return false; // Wrong piece
	}
	++it;
	return true;
}

bool readPromote(const char*& it) {
	if (*it != '=') return false; // No promote
	++it;
	return true;
}
	
bool readFinalChecks(const char*& it, MoveInfo& move) {
	// Invalidate checks
	move.check = false;
	move.checkmate = false;

	if (*it == '+') move.check = true;
	else if (*it == '#') move.checkmate = true;
	else return false;

	++it; // Skip checks
	return true;
}

bool readOptComment(const char*& it) {
	if (*it == ';') {
		// Comment to the end of line
		++it;
		while (*it != '\n' && *it) ++it;
	} else if (*it == '{') {
		// Comment to the next }, wraps
		while (*it != '}' && *it) ++it;
		if (!*it) return false; // Missing ending }
		
		++it; // Skip '}'
	}
	return true;
}	

bool readCapture(const char*& it, bool& capture) {
	if (*it != 'x') return false; // no capture
	capture = true;

	++it;
	return true;
}

bool readCastling(const char*& it, CastlingSide& side) {
	// 0-0->(+,#); 0-0-0->(+,#)

	if (tolower(*it) != 'o') return false;

	++it;
	if (*it != '-') return false;
	
	++it;
	if (tolower(*it) != 'o') return false;

	++it;
	if (*it != '-') {
		// Found king-side castling
		side = KSIDE;
		return true; 
	}

	// Expect o: queen-side castle
	++it;
	if (tolower(*it) != 'o') return false;

	++it; // Skip o

	side = QSIDE;
	return true;
}

bool readPiecePlacement(const char*& it, MoveInfo& move) {
	move.castles = CS_NONE;
	move.capture = false;
	
	// Invalidate coordinates
	move.from = {-1,-1};
	move.to = {-1,-1};

	bool marked = true; // For keeping track is piece type symbolic was given

	// Piece names have to be in uppercase to avoid ambiguity with pawns
	if (!charToPieceType(*it, move.type)) {
		// If not a piece symbol then either a pawn move or castles
		move.type = PAWN; 
		marked = false;
	} else ++it; // Skip piece character
		
	if (int read = readCoord(it, move.from)) {
		if (readX(it, move.to.x)) {
			return readY(it, move.to.y);
		} else if (readCapture(it, move.capture)) {
			return readX(it, move.to.x) && readY(it, move.to.y);
		} else if (read == 2) {
			move.to = {move.from.x, move.from.y};
			move.from = {-1,-1};
		} else return false;
	} else if (marked && readCapture(it, move.capture)) {
		return readX(it, move.to.x) && readY(it, move.to.y);
	} else if (!marked) {
		// If not marked with piece type could be castling
		return readCastling(it, move.castles);
	} else return false;

	return true;
}	

bool readMoveNotation(const char*& it, MoveInfo& move) {
	while (isspace(*it)) ++it; // Skip whitespace
		
	if (!readPiecePlacement(it, move)) {
		return false; // Unknown piece or wrong placement
	}
		
	move.promote = VOID;
	if (readPromote(it) && (move.type != PAWN || !readPromoteType(it, move.promote))) {
		// Malformed promotion symbolic, or non-pawn trying to promote
		return false;
	}
		
	readFinalChecks(it, move);

	// Final comments		
	while (isspace(*it)) ++it; // Skip whitespace before comment
	return readOptComment(it);
}	

}

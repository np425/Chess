#include "move_notation.h"
#include <cctype> 

namespace chess {

ExprEval readPromoteType(const char*& it, PieceType& promote) {
	// Letter case here does not matter, because there's no ambiguity
	promote = charToPromoteType(*it);
	if (promote == NO_PIECE) {
		return ExprEval::None;
	}
	++it;
	return ExprEval::Valid;
}

ExprEval readPromote(const char*& it, PieceType& promote) {
	promote = NO_PIECE;
	if (*it != '=') {
		// No promote
		return ExprEval::None;
	}
	++it;
	if (readPromoteType(it, promote) != ExprEval::Valid) {
		return ExprEval::Malformed;
	} 
	return ExprEval::Valid;
}
	
ExprEval readFinalChecks(const char*& it, CheckType& checks) {
	// Invalidate checks
	checks = CheckType::None;

	if (*it == '+') {
		checks = CheckType::Check;
	} else if (*it == '#') {
		checks = CheckType::Checkmate;
	} else {
		return ExprEval::None;
	}

	++it; // Skip checks
	return ExprEval::Valid;
}

ExprEval readComment(const char*& it) {
	if (*it == ';') {
		// Comment to the end of line
		++it;
		while (*it != '\n' && *it) {
			++it;
		}
	} else if (*it == '{') {
		// Comment to the next }, wraps
		while (*it != '}' && *it) {
			++it;
		}
		if (!*it) {
			// Missing ending }
			return ExprEval::Malformed; 
		}
		
		++it; // Skip '}'
	} else {
		return ExprEval::None;
	}

	return ExprEval::Valid;
}	

ExprEval readCapture(const char*& it, bool& capture, Coord& sqr) {
	if (*it != 'x') {
		// No capture
		return ExprEval::None; 
	}
	capture = true;

	++it;

	if (readX(it, sqr.x) != ExprEval::Valid || readY(it, sqr.y) != ExprEval::Valid) {
		return ExprEval::Malformed;
	}

	return ExprEval::Valid;
}

ExprEval readCastling(const char*& it, CastlingSide& side) {
	// 0-0->(+,#); 0-0-0->(+,#)
	if (readStringInsensitive(it, "O-O") != ExprEval::Valid) {
		return ExprEval::None;
	}

	if (*it != '-') {
		// Found king-side castling
		side = CASTLES_KSIDE;
		return ExprEval::Valid; 
	}

	// Expect o: queen-side castle
	++it;
	if (toupper(*it) != 'o') {
		return ExprEval::Malformed;
	}

	// Skip o
	++it; 

	side = CASTLES_QSIDE;
	return ExprEval::Valid;
}

ExprEval readPiecePlacement(const char*& it, MoveInfo& move) {
	move.castles = CASTLES_NONE;
	move.capture = false;
	
	// Invalidate coordinates
	move.from = {-1,-1};
	move.to = {-1,-1};

	// For keeping track if given piece symbolic
	bool explicitPiece = true; 

	// Piece names have to be in uppercase to avoid ambiguity with pawns
	move.type = charToType(*it);

	if (move.type == NO_PIECE) {
		// If not a piece symbol then either a pawn move or castles
		move.type = PAWN; 
		explicitPiece = false;
	} else {
		// Skip piece character
		++it; 
	}

	unsigned read;
	if (int read = readCoord(it, move.from)) {
		if (readX(it, move.to.x) == ExprEval::Valid) {
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
	const char* tempIt = it;

	while (isspace(*tempIt)) ++tempIt; // Skip whitespace before comment
	ExprType eval = readComment(tempIt);

	if (eval == ExprType::MALFORMED) {
		return false;
	} else if (eval == ExprType::VALID) {
		it = tempIt;
	}

	return true;
}	

}

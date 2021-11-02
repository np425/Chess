#include "notation.h" 
#include "chess.h" // PieceTyppe
#include "utils.h" // textToLower

// ----------- Helper function prototypes

bool readCapture(char*& it, bool &capture);
bool readOptFinalChecks(char*& it, bool &check, bool &checkmate);

bool readOptCaptureXY(char*& it, bool& capture, int& x, int& y);
bool readOptPromoteFinalChecks(char*& it, PieceType& promote, bool& check, bool& checkmate);

// ----------- Main functions
bool translateNotation(char* notation, MoveInfo &move) {
	textToLower(notation); // Lowercase notation

	char* it = notation;

    // Deduce piece types
    if (*it == 'r') move.pieceType = PT_ROOK;
    else if (*it == 'n') move.pieceType = PT_KNIGHT;
    else if (*it == 'b') move.pieceType = PT_BISHOP;
    else if (*it == 'q') move.pieceType = PT_QUEEN;
    else if (*it == 'k') move.pieceType = PT_KING;
    else if (readX(it, move.from.x)) { // If X is given, assume piece is a pawn
        // Expect x->X->Y->(=p)->(+,#), Y->(=p)->(+,#), Y->(x)->X->Y->(=p)->(+,#)
        move.pieceType = PT_PAWN;
        if (readCapture(it, move.capture)) { // x->X->Y->(=p)->(+,#)
			if (!readX(it, move.to.x) || !readY(it, move.to.y)) 
				return false;
            else
				return readOptPromoteFinalChecks(it, move.promote, move.check, move.checkmate);
        } else if (readY(it, move.from.y)) { // Y->(=p)->(+,#), Y->(x)->X->Y->(=p)->(+,#)
			if (readOptCaptureXY(it, move.capture, move.to.x, move.to.y)) {
				return readOptPromoteFinalChecks(it, move.promote, move.check, move.checkmate);
            } else if (readOptPromoteFinalChecks(it, move.promote, move.check, move.checkmate)) {
                // Invalidate origin coordinates
                move.to.x = move.from.x;
                move.to.y = move.from.y;
                move.from = {-1,-1};
                return true;
			} else 
				return false;
        } else 
			return false;
    } else if (*it == 'o') { // Castling
        // O-O(+,#); O-O-O(+,#)

        //Expect -
        ++it;
        if (*it != '-') 
			return false;

        // Expect o: king side castle
        ++it;
        if (*it != 'o') 
			return false;

        ++it;
		// Optional king side checks
        if (readOptFinalChecks(it, move.check, move.checkmate)) {
			move.castles[1] = true;
			return true;
		}
            
		// Expect -
        else if (*it != '-') 
			return false;

		// Expect o: queen side castle
        ++it;
        if (*it != 'o') 
			return false;

		// Optional queen side checks
        return (move.castles[0] = readOptFinalChecks(it, move.check, move.checkmate));
    } else 
		return false; // Unknown first character

    // For all pieces except pawns (pawns are processed separately) continue here:

    // Expects:
    // x: X->Y->(+,#);
    // X: (x)->X->Y->(+,#), Y(+,#), Y->(x)->X->Y->(+,#);
    // Y: (x)->X->Y->(+,#)

    ++it;
    if (readCapture(it, move.capture)) { // Takes
        // Expect X->Y->(+,#)
		if (!readX(it, move.to.x) || !readY(it, move.to.y))
			return false;
		else 
			return readOptFinalChecks(it, move.check, move.checkmate);
    } else if (readX(it, move.from.x)) { // X
        // Expect (x)->X->Y->(+,#) or Y->(+,#) or Y->(x)->X->Y->(+,#)
        if (readY(it, move.from.y)) {
            if (readOptFinalChecks(it, move.check, move.checkmate)) {
                // Invalidate origin coordinates
                move.to.x = move.from.x;
                move.to.y = move.from.y;
                move.from = {-1,-1};
                return true;
            } else if (readOptCaptureXY(it, move.capture, move.to.x, move.to.y)) {
				return readOptFinalChecks(it, move.check, move.checkmate);
			} else
				return false;
        } else if (readOptCaptureXY(it, move.capture, move.to.x, move.to.y)) {
			return readOptFinalChecks(it, move.check, move.checkmate);
		} else
			return false;
    } else if (readY(it, move.from.y)) { // Y coordinate
        // Expect (x)->X->Y->(+,#)
        if (readOptCaptureXY(it, move.capture, move.to.x, move.to.y)) {
			return readOptFinalChecks(it, move.check, move.checkmate);
		} else
			return false;
    } else 
		return false;
}

// ----------- Elementary reading functions (input lowercase)
bool readX(char*& it, int &x) {
    if (*it < 'a' || *it > 'h') return false;
	x = *it - 'a'; // Gives numbers starting from 0

	++it;
    return true;
}

bool readY(char*& it, int &y) {
    if (*it < '1' || *it > '8') return false;
    y = *it - '1'; // Gives numbers starting from 0

	++it;
    return true;
}

bool readCapture(char*& it, bool &capture) {
    if (*it != 'x') return false;
    capture = true;
	
	++it;
    return true;
}

// Optional Checks, has to be the end of notation
bool readOptFinalChecks(char*& it, bool &check, bool &checkmate) {
    if (*it == '+') check = true;
    else if (*it == '#') checkmate = true;
	else return !(*it);

	++it;

    return !(*it);
}

bool readOptPromote(char*& it, PieceType& promote) {
	if (*it != '=') return true;
	else ++it;

	switch (*it) {
		case 'r': promote = PT_ROOK; break;
		case 'b': promote = PT_BISHOP; break;
		case 'n': promote = PT_KNIGHT; break;
		case 'q': promote = PT_QUEEN; break;
		default: --it; return false;
	}

	++it;
	return true;
}

// ----------- Composite reading functions (input lowercase)
bool readOptCaptureXY(char*& it, bool& capture, int& x, int& y) {
	readCapture(it, capture);
	
	if (!readX(it, x)) return false;
	else if (!readY(it, y)) it -= (1 + capture);
	else return true;

	return false;
}

bool readOptPromoteFinalChecks(char*& it, PieceType& promote, bool& check, bool& checkmate) {
	readOptPromote(it, promote);

	if (!readOptFinalChecks(it, check, checkmate)) it -= 2;
	else return true;

	return false;
}


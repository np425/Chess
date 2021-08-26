#include "notation.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <string>
using namespace std;

// TODO: Add promotion symbolic

//void strToLower(string &str) {
//	for (char &chr : str) {
//		if (chr >= 'A' && chr <= 'Z') chr += 32;
//	}
//}

bool readX(const string& notation, string::iterator cur, int &x) {
	if (cur == notation.end() || !(*cur >= 'a' && *cur <= 'h')) return false; 
	x = *cur - 'a';
	return true;
}

bool readY(const string& notation, string::iterator cur, int &y) {
	if (cur == notation.end() || !(*cur >= '1' && *cur <= '8')) return false; 
	y = *cur - '1';
	return true;
}

bool readTakes(const string& notation, string::iterator cur, bool& takes) {
	if (cur == notation.end() || *cur != 'x') return false;
	takes = true;
	return true;
}

bool readOptFinalChecks(const string& notation, string::iterator cur, bool& check, bool& checkmate) {
	// Optional Checks, has to be the end
	if (cur == notation.end()) return true;
	else if (*cur == '+') check = true;
	else if (*cur == '#') checkmate = true;
	else return false;
	
	return (cur+1 == notation.end());
}


// Fast(Multi) functions
/////////////////////////////////////////
bool readYOptChecks(const string& notation, string::iterator cur, int& y, bool&check, bool& checkmate) {
	if (!readY(notation, cur, y)) return false;
	++cur;
	return (readOptFinalChecks(notation, cur, check, checkmate));
}

bool readXYOptChecks(const string& notation, string::iterator cur, int& x, int& y, bool& check, bool& checkmate) {
	// Expect X->Y(+,#)
	if (!readX(notation, cur, x)) return false;
	++cur;
	return (readYOptChecks(notation, cur, y, check, checkmate));
}

bool readOptTakesXYOptChecks(const string& notation, string::iterator cur, bool takes, int& x, int& y, bool& check, bool& checkmate) {
	if (readTakes(notation, cur, takes)) ++cur;
	return (readXYOptChecks(notation, cur, x, y, check, checkmate));
}
///////////////////////////////////////////

#define _BAD_NOTATION { info.badNotation = true; return; }

// piece x, y; moves x, y
void translateNotation(string notation, NotationInfo& info) {
	//strToLower(notation);
    transform(notation.begin(), notation.end(), notation.begin(),
        [](unsigned char c) { return tolower(c); });
	string::iterator cur = notation.begin();

	// Deduce pieces
	if (*cur == 'r') info.pieceType = P_ROOK;
	else if (*cur == 'n') info.pieceType = P_KNIGHT;
	else if (*cur == 'b') info.pieceType = P_BISHOP;
	else if (*cur == 'q') info.pieceType = P_QUEEN;
	else if (*cur == 'k') info.pieceType = P_KING;
	else if (readX(notation, cur, info.px)) {
		// Pawns processed separately
		// Expect x->X->Y->(+,#), Y(+,#), Y->(x)->X->Y->(+,#)
		// TODO: Promotion notation
		info.pieceType = P_PAWN;
		++cur;
		if (readTakes(notation, cur, info.takes)) {
			++cur;
			if (!readXYOptChecks(notation, cur, info.mx, info.my, info.check, info.checkmate)) _BAD_NOTATION
		} else if (readY(notation, cur, info.py)) {
			++cur;
			if (readOptFinalChecks(notation, cur, info.check, info.checkmate)) {
				// Invalidate algebraic notation
				info.mx = info.px;
				info.my = info.py;
				info.px = info.py = -1;
			// Algebraic notation
			} else if (!readOptTakesXYOptChecks(notation, cur, info.takes, info.mx, info.my, info.check, info.checkmate)) _BAD_NOTATION
		} else _BAD_NOTATION
		return;
	} else if (*cur == '=') { // Draw offer
		info.drawOffer = true;
		if (cur+1  == notation.end()) _BAD_NOTATION
		return;
	} else if (*cur == 'o') { // Castling
		// O-O(+,#); O-O-O(+,#)

		//Expect -
		++cur;
		if (cur == notation.end() || *cur != '-') _BAD_NOTATION
		
		// Expect o, king side castle
		++cur;
		if (cur == notation.end() || *cur != 'o') _BAD_NOTATION
		
		info.kCastles = true;
		++cur;
	
		if (readOptFinalChecks(notation, cur, info.check, info.checkmate)) return;
		// Expect -O, queen side castle
		else if (*cur != '-') _BAD_NOTATION

		++cur;
		if (cur == notation.end() || *cur != 'o') _BAD_NOTATION	

		// Invalidate king castling
		info.kCastles = false;
		info.qCastles = true;	

		// Optional checks
		if (!readOptFinalChecks(notation, cur, info.check, info.checkmate)) _BAD_NOTATION
	// Unknown first character
	} else _BAD_NOTATION

	// Algebraic notation: 
	// X or Y or both

	// Expects: 
	// x: X->Y->(+,#);
	// X: (x)->X->Y->(+,#), Y(+,#), Y->(x)->X->Y->(+,#);
	// Y: (x)->X->Y->(+,#)

	++cur;
	// Takes
	if (readTakes(notation, cur, info.takes)) {
		// Expect X->Y->(+,#)
		++cur;
		if (!readXYOptChecks(notation, cur, info.mx, info.my, info.check, info.checkmate)) _BAD_NOTATION
	// X coordinate
	} else if (readX(notation, cur, info.px)) {
		// Expect (x)->X->Y(+,#) or Y(+,#) or Y->(x)->X->Y->(+,#)
		++cur;
	
		if (readY(notation, cur, info.py)) {
			++cur;
			if (readOptFinalChecks(notation, cur, info.check, info.checkmate)) {
				// Invalidate algebraic coordinates
				info.mx = info.px;
				info.my = info.py;
				info.px = info.py = -1;
			} else if (!readOptTakesXYOptChecks(notation, cur, info.takes, info.mx, info.my, info.check, info.checkmate)) _BAD_NOTATION
		} else if (!readOptTakesXYOptChecks(notation, cur, info.takes, info.mx, info.my, info.check, info.checkmate)) _BAD_NOTATION
	// Y coordinate
	} else if (readY(notation, cur, info.py)) {
		// Expect (x)->X->Y->(+,#)
		++cur;
		if (!readOptTakesXYOptChecks(notation, cur, info.takes, info.mx, info.my, info.check, info.checkmate)) _BAD_NOTATION
	} else _BAD_NOTATION

}


// piece x, y; moves x, y
// void translateNotation(string notation, NotationInfo& info) {
//	strToLower(notation);
//	string::iterator cur = notation.begin();
//
//	// Deduce pieces
//	if (*cur == 'r') info.piece = R;
//	else if (*cur == 'n') info.piece = N;
//	else if (*cur == 'b') info.piece = B;
//	else if (*cur == 'q') info.piece = Q;
//	else if (*cur == 'k') info.piece = K;
//	else if (*cur >= 'a' && *cur <= 'h') { // Pawn
//		// X: x->X->Y(+,#), Y->(+,#);X->Y(+,#)
//
//		info.px = *cur - 'a';
//		info.piece = P;
//		
//		// Expect takes, Y
//		++cur;
//		if (cur == notation.end()) { info.badNotation = true; return; }
//
//		if (*cur == 'x') { 
//			info.takes = true;
// 			
//			// Expect X->Y(+,#)
//			goto expectxy;
//		} else if (*cur >= '1' && *cur <= '8') {
//			info.my = *cur - '1';
//
//			// Expect X, (+,#)
//			if (*cur >= 'a' && *cur <= 'h') {
//				info.py = *cur - 'a';
//
//				// Expect Y(+,#)
//				goto expecty;
//			}
//			
//			// Expect (+,#)
//			// Invalidate algebraic coordinate
//			info.mx = info.px;
//			info.px = -1;
//
//			goto expectchecks;
//		} else { info.badNotation = true; return; }
//	// Draw offer
//	} else if (*cur == '=') {
//		info.drawOffer = true;
//		if (cur+1  == notation.end()) info.badNotation = true;
//		return;
//	// Castles
//	} else if (*cur == 'o') {
//		// O-O(+,#); O-O-O(+,#)
//
//		//Expect -
//		++cur;
//		if (cur == notation.end() || *cur != '-') { info.badNotation = true; return; }
//		
//		// Expect o, king side castle
//		++cur;
//		if (cur == notation.end() || *cur != 'o') { info.badNotation = true; return; }	
//		
//		info.kCastles = true;
//		++cur;
//		if (cur == notation.end()) return;
//
//		// Optional checks
//		if (*cur == '+') {
//			info.check = true;
//			if (cur+1 != notation.end()) info.badNotation=true;
//			return;
//		}
//		else if (*cur == '#') {
//			info.checkmate = true;
//			if (cur+1 != notation.end()) info.badNotation=true;
//			return;
//		}	
//
//		// Expect -0, Queen side castle
//		if (*cur != '-') { info.badNotation = true; return; }
//		++cur;
//
//		// Expect 0
//		if (cur == notation.end() || *cur != 'o') { info.badNotation=true; return; }
//		
//		// Invalidate king castling
//		info.kCastles = false;
//		info.qCastles = true;	
//
//		// Optional checks
//		goto expectchecks;
//	// Unknown first character
//	} else { info.badNotation = true; return; }
//
//	// Algebraic notation: 
//	// X or Y or both
//
//	// Expects: 
//	// x: X->Y->(+,#);
//	// X: (x)->X->Y->(+,#), Y(+,#), Y->(x)->X->Y->(+,#);
//	// Y: (x)->X->Y->(+,#)
//
//	++cur;
//	if (cur == notation.end()) { info.badNotation = true; return; }
//
//	// Takes
//	if (*cur == 'x') {
//		info.takes = true;
//	
//		// Expect X->Y(+,#)
//expectxy:
//		++cur;
//		if (cur == notation.end()) { info.badNotation = true; return; }	
//
//		if (!(*cur >= 'a' && *cur <= 'h')) { info.badNotation = true; return; }
//		info.mx = *cur - 'a';
//
//		// Expect Y(+,#)
//expecty:
//		++cur;
//		if (cur == notation.end()) { info.badNotation = true; return; }	
//
//		if (!(*cur >= '1' && *cur <= '8')) { info.badNotation=true; return; }
//		info.my = *cur - '1';
//		
//		// Optional checks
//expectchecks:
//		++cur; 
//		if (cur == notation.end()) return;	
//		
//		if (*cur == '+') {
//			info.check = true;
//			if (cur+1 != notation.end()) info.badNotation=true;
//			return;
//		}
//		else if (*cur == '#') {
//			info.checkmate = true;
//			if (cur+1 != notation.end()) info.badNotation=true;
//			return;
//		}	
//		else { info.badNotation = true; return; }
//	}
//	// X coordinate
//	if (*cur >= 'a' && *cur <= 'z') {
//		info.px = *cur - 'a';
//		++cur;
//
//		// Expect takes, X coordinate, Y coordinate
//		if (cur == notation.end()) { info.badNotation = true; return; }	
//
//		if (*cur == 'x') {
//			info.takes = true;
//		
//			// Expect X->Y(+,#)
//			goto expectxy;
//		} else if (*cur >= 'a' && *cur <= 'h') {
//			info.mx = *cur - 'a';
//		
//			// Expect Y(+,#)
//			goto expecty;
//		} else if (*cur >= '1' && *cur <= '8') {
//			// Invalidate algebraic coordinate
//			info.py = *cur - '1';
//	
//			// Expect takes, X, (+,#)
//			++cur;
//			if (*cur == 'x') {
//				info.takes = true;
//
//				// Expect X->Y(+,#)
//				goto expectxy;
//			} else if (*cur >= 'a' && *cur <= 'h') {
//				info.mx = *cur - 'a';
//
//				// Expect Y(+,#)
//				goto expecty;
//			}
//			// Expect (+,#)
//			// Invalidate algebraic coordinates
//			info.mx = info.px;
//			info.my = info.py;
//			info.px = info.py = -1;
//
//			goto expectchecks;
//		} else { info.badNotation = true; return; }
//	}
//	// Y coordinate
//	if (*cur >= '1' && *cur <= '8') {
//		info.py = *cur - '1';
//		++cur;
//		
//		// Expect takes, X coordinate
//		if (cur == notation.end()) { info.badNotation = true; return; }	
//		
//		if (*cur == 'x') {
//			info.takes = true;
//		
//			// Expect X->Y(+,#)
//			goto expectxy;
//		} else if (*cur >= 'a' && *cur <= 'h') {
//			info.mx = *cur - 'a';
//		
//			// Expect Y(+,#)
//			goto expecty;
//		} else { info.badNotation = true; return; }
//	} else { info.badNotation = true; return; }
//}
//

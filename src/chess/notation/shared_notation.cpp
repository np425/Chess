#include "shared_notation.h"
#include <cctype> // tolower

namespace {
// Unsigned power functions (no negative powers)
unsigned u_pow(unsigned base, unsigned power) {
	unsigned res = 1;

	while (power > 0) {
		--power;
		res *= base;
	}
	return res;
}	

}

namespace chess {	
PieceType charToType(const char chr) { 
	switch (toupper(chr)) {
		case 'R':
			return ROOK;
		case 'N':
			return KNIGHT;	
		case 'B':
			return BISHOP;
		case 'Q':
			return QUEEN;
		case 'K':
			return KING;
		case 'P': // pe4 allowed
			return PAWN;
		default:
			return NO_PIECE;
	}
}

PieceType charToPromoteType(const char chr) {
	switch (toupper(chr)) {
		case 'R': 
			return ROOK;
		case 'B':
			return BISHOP;
		case 'N':
			return KNIGHT;
		case 'Q':
			return QUEEN;
		default:
			return NO_PIECE;	
	}

}

CastlingSide charToCastlingSide(const char chr) {
	switch (toupper(chr)) {
		case 'K':
			return CASTLES_KSIDE;
		case 'Q':
			return CASTLES_QSIDE;
		default:
			return CASTLES_NONE;
	}
}

ExprEval readStringInsensitive(const char*& it, const char* str) {
	if (!*it) {
		return ExprEval::None;
	}

	int i = 0;
	while (str[i]) {
		if (toupper(str[i]) != toupper(it[i])) {
			return ExprEval::Malformed;
		}
		++i;
	}

	it += i;
	return ExprEval::Valid;
}

ExprEval readX(const char*& it, int& x) {
	unsigned char chr = tolower(*it);
	if (chr < 'a' || chr > 'h') {
		return ExprEval::None;
	}
	x = chr - 'a';

	++it;
	return ExprEval::Valid;
}

ExprEval readY(const char*& it, int& y) {
	unsigned char chr = tolower(*it);
	if (chr < '1' || chr > '8') {
		return ExprEval::None;
	}
	y = chr - '1';

	++it;
	return ExprEval::Valid;
}

unsigned readCoord(const char*& it, Coord& coord) {
	unsigned read = 0;
	if (readX(it, coord.x) == ExprEval::Valid) {
		++read;
	}
	if (readY(it, coord.y) == ExprEval::Valid) {
		++read;
	}
	return read;
}

ExprEval readInteger(const char*& it, unsigned& num) {
	unsigned l = 0;
	num = 0;

	while (*(it+l) >= '0' && *(it+l) <= '9') {
		++l;
	}

	if (!l) {
		// No number
		return ExprEval::None; 
	}

	while (l) { // Construct number
		unsigned digit = *it - '0';
		num += (u_pow(10, l-1) * digit);
		--l;
		++it;
	}
	return ExprEval::Valid;
}

}

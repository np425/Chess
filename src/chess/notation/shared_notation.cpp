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

bool readChar(const char*& it, const char chr) {
	if (*it != chr) {
		return false;
	}

	++it;
	return true;
}

bool readInsensitiveChar(const char*& it, const char chr) {
	if (toupper(*it) != toupper(chr)) {
		return false;
	}

	++it;
	return true;
}

int readStringInsensitive(const char*& it, const char* str) {
	if (!*it) {
		return 0;
	}

	int i = 0;
	while (str[i]) {
		if (toupper(str[i]) != toupper(it[i])) {
			return -1;
		}
		++i;
	}

	it += i;
	return i;
}

int readX(const char*& it, int& x) {
	unsigned char chr = tolower(*it);
	if (chr < 'a' || chr > 'h') {
		return 0;
	}
	x = chr - 'a';

	++it;
	return 1;
}

int readY(const char*& it, int& y) {
	unsigned char chr = tolower(*it);
	if (chr < '1' || chr > '8') {
		return 0;
	}
	y = chr - '1';

	++it;
	return 1;
}

int readCoord(const char*& it, Coord& coord) {
	unsigned read = 0;
	if (readX(it, coord.x) > 0) {
		++read;
	}
	if (readY(it, coord.y) > 0) {
		++read;
	}
	return read;
}

int readPosInt(const char*& it, unsigned& num) {
	unsigned l = 0;
	num = 0;

	while (*(it+l) >= '0' && *(it+l) <= '9') {
		++l;
	}

	if (!l) {
		// No number
		return 0; 
	}

	for (int i = l; i; --i) { // Construct number
		unsigned digit = *it - '0';
		num += (u_pow(10, l-1) * digit);
		++it;
	}
	return l;
}

}

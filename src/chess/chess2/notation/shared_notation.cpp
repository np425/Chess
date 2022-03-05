#include "shared_notation.h"
#include <cctype> // tolower

namespace chess {	
// Unsigned power functions (no negative powers)
unsigned u_pow(unsigned base, unsigned power) {
	unsigned res = 1;

	while (power > 0) {
		--power;
		res *= base;
	}
	return res;
}

bool charToPieceType(const char chr, PieceType& pieceType) { 
	switch (chr) {
		case 'R':
			pieceType = ROOK;
			break;
		case 'N':
			pieceType = KNIGHT;
			break;
		case 'B':
			pieceType = BISHOP;
			break;
		case 'Q':
			pieceType = QUEEN;
			break;
		case 'K':
			pieceType = KING;
			break;
		case 'P': // pe4 allowed
			pieceType = PAWN;
			break;
		default:
			return false;
	}
	return true;
}

bool readX(const char*& it, int& x) {
	unsigned char chr = tolower(*it);
	if (chr < 'a' || chr > 'h') return false;
	x = chr - 'a';

	++it;
	return true;
}

bool readY(const char*& it, int& y) {
	unsigned char chr = tolower(*it);
	if (chr < '1' || chr > '8') return false;
	y = chr - '1';

	++it;
	return true;
}

int readCoord(const char*& it, Coord& coord) {
	int read = 0;
	if (readX(it, coord.x)) ++read;
	if (readY(it, coord.y)) ++read;
	return read;
}

bool readPositiveInteger(const char*& it, unsigned& num) {
	unsigned l = 0;
	num = 0;

	while (*(it+l) >= '0' && *(it+l) <= '9') {
		++l;
	}

	if (!l) return false; // No number

	while (l) { // Construct number
		unsigned digit = *it - '0';
		num += (u_pow(10, l-1) * digit);
		--l;
		++it;
	}
	return true;
}

}

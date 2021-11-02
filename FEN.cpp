#ifndef FEN_H
#define FEN_H

#include "FEN.h" 
#include "chess.h" // global variables, types
#include "notation.h" // readX, readY
#include "utils.h" // charToLower, textToPositiveInteger, skipSpaces

// ----------- Helper function prototypes
int readPiece(char*& it, Piece& piece);
void skipSlash(char*& it);

bool readPlayer(char*& it, Player& player);

bool readPassant(char*& it, Square& sqr);
bool readCastlingRights(char*& it, bool castlingRights[2][2]);

bool readPositiveInteger(char*& it, unsigned& number);

// ----------- Main functions

// Spaces and slashes are redundant
bool loadFEN(char* fen) {
	char* it = fen;

	// 1. Pieces reading (from White's perspective)
	for (int y = BOARD_SIZE_Y - 1; y >= 0; --y) {
		for (int x = 0; x < BOARD_SIZE_X; ) {
			int read = readPiece(it, board[y][x]);
			if (!read) return false; // Unable to read a piece
			else x += read;
		}
		skipSlash(it); // Slashes are unnecessary
	}
	skipSpaces(it);

	// 2. Next player to move
	if (!readPlayer(it, toMove)) 
		return false;
	else
		skipSpaces(it);

	// 3. Castling rights
	if (!readCastlingRights(it, canCastle))
		return false;
	else
		skipSpaces(it);

	// 4. Passant
	if (!readPassant(it, passant))
		return false;
	else
		skipSpaces(it);

	// 5. Halfmoves
	if (!readPositiveInteger(it, halfMoves))
		return false;
	else
		skipSpaces(it);

	// 6. Fullmoves
	if (!readPositiveInteger(it, fullMoves))
		return false;
	else 
		skipSpaces(it);

	return true;
}

// ----------- Notation Reading Functions

// Returns how many pieces it has read
int readPiece(char*& it, Piece& piece) {
	// Uppercase = white pieces, lowercase = black pieces
	int sign = (isUppercase(*it) ? 1 : -1); 

	char chr = charToLower(*it);

	switch (chr) {
		case 'r':
			piece = sign * PT_ROOK;
			break;
		case 'n':
			piece = sign * PT_KNIGHT;
			break;
		case 'b':
			piece = sign * PT_BISHOP;
			break;
		case 'q':
			piece = sign * PT_QUEEN;
			break;
		case 'k':
			piece = sign * PT_KING;
			break;
		case 'p':
			piece = sign * PT_PAWN;
			break;
		default:
			if (chr >= '1' && chr <= '8') {
				piece = PT_NONE;
				++it;
				return chr - '0';
			} else 
				return 0; // Unknown piece
			
	}
	++it;
	return 1;
}

bool readPlayer(char*& it, Player& player) {
	switch (*it) {
		case 'w':
			player = PL_WHITE;
			break;
		case 'b':
			player = PL_BLACK;
			break;
		default:
			return false; // Unknown player
	}
	++it;
	return true;
}

bool readCastlingRights(char*& it, bool castlingRights[2][2]) {
	castlingRights[0][0] = false;
	castlingRights[0][1] = false;
	castlingRights[1][0] = false;
	castlingRights[1][1] = false;

	if (*it == '-') {
		++it;
		return true;
	}
	bool foundOne = false;

	while (true) {
		Player player = (isUppercase(*it) ? PL_WHITE : PL_BLACK);
		char chr = charToLower(*it);

		int side = -1;

		// Order does not matter
		switch (chr) {
			case 'k':
				side = 1;
				foundOne = true;
				break;
			case 'q':
				side = 0;
				foundOne = true;
				break;
			default:
				// Make sure there is at least one castling right if no -
				return foundOne;
		}

		if (castlingRights[player][side] == true) {
			return false; // Side not found or it's already set
		} else {
			castlingRights[player][side] = true;
			++it;
		}
	}
}

bool readPassant(char*& it, Square& sqr) {
	sqr = {-1,-1};

	if (*it == '-') {
		++it;
		return true;
	}

	if (!readX(it, sqr.x) || !readY(it, sqr.y)) return false;
	else return true;
}

bool readPositiveInteger(char*& it, unsigned& number) {
	char* it2 = textToPositiveInteger(it, number);
	bool returnVal = (it != it2);
	it = it2;
	return returnVal;
}

void skipSlash(char*& it) {
	if (*it == '/') ++it;
}

#endif

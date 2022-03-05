#ifndef CHESS_BASIC_TYPES_H
#define CHESS_BASIC_TYPES_H

#include "fixed_array.h"

#define BOARD_SIZE_X 8
#define BOARD_SIZE_Y 8
#define BOARD_SIZE (BOARD_SIZE_X * BOARD_SIZE_Y)
#define MAX_ATTACKERS 16
#define FIRST_TO_MOVE WHITE

namespace chess {

enum PieceType {
	VOID = 0, ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN
};

enum Player {
	WHITE = 0, BLACK, PL_NONE
};

enum GameStateType {
	PLAYING = 0,
	WIN, // Resignation
	CHECKMATE,
	DRAW,
	STALEMATE
};

enum CastlingSide {
	QSIDE   = 0, 
	KSIDE   = 1 << 0, 
	CS_NONE = 1 << 1
};

/* + for White, - for Black, 0 for neutral */
typedef int Piece;
typedef int GameState;
typedef int CastlingPerms;

struct Coord {
	int x;
	int y;
};

typedef FixedArray<Coord, MAX_ATTACKERS> CoordArray;

struct MoveInfo {
	// bool drawOffer;
	// bool resignation;

	PieceType type;
	PieceType promote;

	// Prior piece coordinates: where piece is
	Coord from;

	// Move coordinates: where piece is moving
	Coord to;

	// If piece is capturing a piece
	bool capture;

	bool check;
	bool checkmate;
	CastlingSide castles;
};

}

#endif

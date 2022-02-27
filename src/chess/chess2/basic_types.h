#ifndef CHESS_BASIC_TYPES_H
#define CHESS_BASIC_TYPES_H

#include "fixed_array.h"

#define BOARD_SIZE_X 8
#define BOARD_SIZE_Y 8
#define BOARD_SIZE (BOARD_SIZE_X * BOARD_SIZE_Y)
#define MAX_ATTACKERS 16

namespace chess {

enum PieceType {
	PT_NONE = 0, PT_ROOK, PT_KNIGHT, PT_BISHOP, PT_QUEEN, PT_KING, PT_PAWN
};

enum Player {
	PL_WHITE = 0, PL_BLACK, PL_NONE
};

enum GameStateType {
	GS_PLAYING = 0,
	GS_WIN, // Resignation
	GS_CHECKMATE,
	GS_DRAW,
	GS_STALEMATE
};

enum CastlingSide {
	CS_NONE  = 0, 
	CS_QSIDE = 1 << 0, 
	CS_KSIDE = 1 << 1
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

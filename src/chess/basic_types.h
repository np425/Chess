#ifndef CHESS_BASIC_TYPES_H
#define CHESS_BASIC_TYPES_H

#include <map>
#include <vector>
#include <string>

#define BOARD_SIZE_X 8
#define BOARD_SIZE_Y 8
#define BOARD_SIZE (BOARD_SIZE_X * BOARD_SIZE_Y)

namespace chess {

enum PieceType {
	VOID=0, ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN
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
	CASTLES_NONE  = 0,
	CASTLES_QSIDE = 1, 
	CASTLES_KSIDE = 2,
};

/* + for White, - for Black, 0 for neutral */
typedef int Piece;
typedef int GameState;
typedef int CastlingPerms;

struct Coord {
	int x;
	int y;
};

enum class CheckType {
	None, Check, Checkmate
};

struct MoveInfo {
	// bool drawOffer;
	// bool resignation;

	PieceType type;
	PieceType promote;

	// Previous piece coordinates: where piece is
	Coord from;

	// Move coordinates: where piece is moving
	Coord to;

	// If piece is capturing a piece
	bool capture;

	CheckType checks;
	CastlingSide castles;
};

struct NotatedMove {
	MoveInfo move;
	std::string notation;
};

typedef std::pair<std::string, std::string> Tag;
typedef std::map<std::string, std::string> Tags;
typedef std::vector<Coord> Coords;
typedef std::vector<NotatedMove> Moves;

struct PositionInfo {
	Player toMove = WHITE;
	GameState state = PLAYING;
	CastlingPerms castlePerms[2] = {
		CASTLES_KSIDE | CASTLES_QSIDE, 
		CASTLES_KSIDE | CASTLES_QSIDE
	};
	Coord passant = {-1,-1};
	unsigned fullMoves = 0;
	unsigned halfMoves = 0;
};

}

#endif

#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#define BOARD_SIZE_X 8
#define BOARD_SIZE_Y 8

#define FIRST_TO_MOVE PL_WHITE
#define CHESS_TAG_LENGTH 100
#define CHESS_MAX_TAGS 30
#define MAX_FULL_MOVES 200
#define NOTATED_MOVE_LENGTH 100
#define MAX_ATTACKERS 16

/*

Chess data types and settings

*/

// TODO: Add 50-move rule & half move rule
// TODO: Rewrite passant to be behind the move
// TODO: Do not allow moves to go to 8th rank
// TODO: Move notations in seperate array

namespace chess {

enum PieceType {
	PT_NONE = 0, PT_ROOK, PT_KNIGHT, PT_BISHOP, PT_QUEEN, PT_KING, PT_PAWN
};

enum Player {
	PL_WHITE = 0, PL_BLACK = 1, PL_NONE = 2
};

// Only 3 states for now
enum GameStateType {
	GS_PLAYING = 0, 
	GS_WIN, // For resignation
	GS_CHECKMATE, 
	GS_DRAW, // For draw agreement, threefold repetition or fifty move rule
	GS_STALEMATE
};
	
// For reference to board
struct Coord {
	int x;
	int y;
};
	
// For move (algebraic) notation
struct MoveInfo { 
	PieceType type;
	PieceType promote;

	// Prior coordinates: which piece is moving
	Coord from;

	// Move coordinates: where the piece is moving
	Coord to;

	bool capture;

	// bool drawOffer;

	bool check;
	bool checkmate;
	bool castles[2];
};

typedef int Piece; // White+, Black-
typedef int GameState; // Checkmate/Stalemate from white+, from black-

// Created solely in case notation is annotated and cannot be recreated
struct NotatedMove {
	MoveInfo move;
	char notation[NOTATED_MOVE_LENGTH];
};

struct CoordArray {
	Coord coords[MAX_ATTACKERS];
	Coord* end = coords;
};

// Previous moves array, hold NotatedMove (MoveInfo + notation) instead of Position
// To save memory
// TODO: Separate notations in different arrays
struct MovesArray { 
	NotatedMove moves[MAX_FULL_MOVES*2]; // half moves
	NotatedMove* end = moves;
};

typedef char TagName[CHESS_TAG_LENGTH];

// PGN Tags
struct TagsArray { 
	TagName tags[CHESS_MAX_TAGS*2];
	TagName* end = tags;
}; 

// Board and metadata about position
struct Position {
	Piece board[BOARD_SIZE_Y * BOARD_SIZE_X];
	Player toMove; // Current player
	GameState state;
	Coord kingPos[2]; // For fast king lookup
	Coord passant; // Move "behind" the previous pawn two-up move
	bool canCastle[2][2];
	CoordArray checks; // For fast checks lookup
};

struct Count {
	unsigned half;
	unsigned moves;
};

// Current position, Tags & Previous moves array
struct Chess {
	Position pos;
	TagsArray tags;
	MovesArray moves;
	Count count;
};

}

#endif

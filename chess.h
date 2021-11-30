#ifndef CHESS_H
#define CHESS_H

/*
Chess definitions & game mechanics
*/

#include <cmath>

// ----------- Player definitions
enum Player { 
	PL_WHITE = 0, PL_BLACK = 1, PL_NONE = 2
};

// ----------- Piece definitions
enum PieceType {
	PT_NONE = 0, PT_ROOK, PT_KNIGHT, PT_BISHOP, PT_QUEEN, PT_KING, PT_PAWN
};

typedef int Piece; // Negative sign for Black, Positive sign for White

// ----------- Board definitions
constexpr int BOARD_SIZE_X = 8;
constexpr int BOARD_SIZE_Y = 8;

struct Square {
	int y;
	int x;
};

extern Piece board[BOARD_SIZE_Y][BOARD_SIZE_X];

// ----------- Game definitions
constexpr int MAX_ATTACKERS = 16; // Max pieces that can attack a square

enum GameStateType {
	GS_PLAYING = 0, GS_CHECKMATE = 1, GS_STALEMATE = 2
};

typedef int GameState;  

// ----------- Game variables
extern Player toMove;

extern Square checks[MAX_ATTACKERS];
extern Square* checksEnd; 

extern Square passant; 

extern bool canCastle[2][2];

extern GameState gameState;

extern unsigned fullMoves;
extern unsigned halfMoves;

// ----------- Helper functions
inline PieceType pieceToType(const Piece piece) {
	return (PieceType)abs(piece);
}

inline Player pieceToPlayer(const Piece piece) {
	if (piece > 0) 
		return PL_WHITE;
	else if (piece < 0) 
		return PL_BLACK;
	else 
		return PL_NONE;
}

inline bool inBounds(const int y, const int x) {
	return (y >= 0 && y < BOARD_SIZE_Y && x >= 0 && x < BOARD_SIZE_X);
}

// Assume no PL_NONE
inline int getPlayerSign(const Player p) {
	return (p == PL_WHITE ? 1 : -1);
}

inline int numSign(const int num) {
	if (num > 0) 
		return 1;
	else if (num < 0) 
		return -1;
	else 
		return 0;
}

inline GameStateType gameStateToType(const GameState gameState) {
	return (GameStateType)abs(gameState);
}

inline Player gameStateToPlayer(const GameState gameState) {
	if (gameState > 0) 
		return PL_WHITE;
	else if (gameState < 0) 
		return PL_BLACK;
	else 
		return PL_NONE;
}

// ----------- Helper function prototypes
bool validateChecks();

bool isCheckmate(const Player p);
bool isStalemate(const Player p);

Square* getKnightProtects(const Square& targ, Square* it, const Player pl);
Square* getPawnProtects(const Square& targ, Square* it, const Player pl);
Square* getBRQKProtects(const Square& targ, Square* it, const Player pl);

bool canMove(const Square& from, const Square& to, const Player pl);
bool isPinned(const Square& from, const Square& to, const Player pl);

void updateGameState();

// ----------- Main functions
bool validateBoard();

Square* getMoves(const Square& targ, Square* it, const Player pl = toMove);
Square* getProtects(const Square& targ, Square* it, const Player pl);

bool attemptCastles(const int side);
void completeMove(const Square& from, const Square& to, PieceType promote = PT_NONE);


#endif

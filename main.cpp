#include <iostream>
#include "chess.h" 
#include "notation.h" 
#include "FEN.h"   

const char* COLOUR_END = "\033[0m";

constexpr Piece BOARD_SETUP[] = {1,2,3,4,5,3,2,1,6,6,6,6,6,6,6,6};

//warning: ISO C++ forbids converting a string constant to ‘char*’ [-Wwrite-strings]
char* INITIAL_POS_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// ----------- Helper function prototypes
void displayBoard();

char pieceToChar(const Piece piece);
const char* setPieceColour(const Piece piece);
const char* playerToString(const Player pl);
const char* endTypeToString(const GameStateType type);

// ----------- Main functions prototypes
bool setupGame();
void setupBoard();
void processMoves();
bool handleMove(const MoveInfo& move);

// ----------- Main functions
int main() {
	std::ios::sync_with_stdio(false);

	if (!setupGame()) {
		std::cerr << "Could not setup the game!" << std::endl;
		return 1;
	}

	if (!validateBoard()) {
		std::cerr << "Invalid initial position!" << std::endl;
		return 1;
	}

	processMoves();

	GameStateType endType = gameStateToType(gameState);
	Player player = gameStateToPlayer(gameState);

	std::cout << "Game ended with " << endTypeToString(endType) << " by " << playerToString(player) << std::endl;

	return 0;
}

void processMoves() {
	char notation[30];
	MoveInfo move;
	bool lastMove = true;

	while (true) {
		std::cout << "\x1B[2J\x1B[H"; // Clear screen (Unix only)
		displayBoard();
		if (gameState != GS_PLAYING)
			break;

		do {
			if (!lastMove) std::cout << "Invalid move" << std::endl;

			std::cout << playerToString(toMove) << " to move: ";
			std::cin.getline(notation, 30);
		} while (!translateNotation(notation, move) || !(lastMove = handleMove(move)));
	}
}

bool handleMove(const MoveInfo& move) {
	// Castling
	if (move.castles[0] || move.castles[1]) {
		int side = (move.castles[0] ? 0 : 1);
		return attemptCastles(side);
	} 

	// Normal moves
	Square foundPiece = {-1,-1};
	Square moves[MAX_ATTACKERS];
	Square* end = getMoves(move.to, moves);

	for (Square* it = moves; it != end; ++it) {
		if (pieceToType(board[it->y][it->x]) != move.pieceType) continue;
		if (move.from.y != -1 && it->y != move.from.y) continue;
		if (move.from.x != -1 && it->x != move.from.x) continue;

		if (foundPiece.y == -1) foundPiece = *it; 
		else return false; // Ambiguous move
	}

	if (foundPiece.y != -1) {
		completeMove(foundPiece, move.to, move.promote);
		return true;
	} else return false;
}


bool setupGame() {
	//setupBoard();
	//toMove = PL_WHITE;
	//canCastle[PL_WHITE][0] = true;
	//canCastle[PL_WHITE][0] = true;
	//canCastle[PL_BLACK][0] = true;
	//canCastle[PL_BLACK][0] = true;
	//passant = {-1,-1};
	//halfMoves = 0;
	//fullMoves = 0;

	return loadFEN(INITIAL_POS_FEN);
}

void setupBoard() {
	constexpr int setupSize = sizeof(BOARD_SETUP)/sizeof(*BOARD_SETUP);
	constexpr int BOARD_SIZE = BOARD_SIZE_Y*BOARD_SIZE_X;

	int i = 0;
	// White
	while (i < setupSize) {
		board[i/BOARD_SIZE_X][i%BOARD_SIZE_X] = +BOARD_SETUP[i];
		++i;
	}

	// Empty squares
	while (i < BOARD_SIZE-setupSize) {
		board[i/BOARD_SIZE_X][i%BOARD_SIZE_X] = PT_NONE;
		++i;
	}

	// Black
	while (i < BOARD_SIZE) {
		int y = BOARD_SIZE_Y-1-i/BOARD_SIZE_X;
		int x = i%BOARD_SIZE_X;
		int coord = y * BOARD_SIZE_X + x;
		board[i/BOARD_SIZE_X][i%BOARD_SIZE_X] = -BOARD_SETUP[coord];
		++i;
	}
}

// ----------- Helper functions
void displayBoard() {
	std::cout << "  A B C D E F G H" << std::endl;
	std::cout << "-------------------" << std::endl;
	for (int y = BOARD_SIZE_Y-1; y >= 0; --y) {
		std::cout << y+1 << "|";
		std::cout << setPieceColour(board[y][0]) << pieceToChar(board[y][0]) << COLOUR_END;
		for (int x = 1; x != BOARD_SIZE_X; ++x) {
			std::cout << " " << setPieceColour(board[y][x]) << pieceToChar(board[y][x]) << COLOUR_END;
		}
		std::cout << "|" << y+1;
		std::cout << std::endl;
	}
	std::cout << "-------------------" << std::endl;
	std::cout << "  A B C D E F G H" << std::endl << std::endl;
}

char pieceToChar(const Piece piece) {
	PieceType pType = pieceToType(piece);
	switch (pType) {
		case PT_NONE:
			return '_';
		case PT_ROOK:
			return 'R';
		case PT_BISHOP:
			return 'B';
		case PT_KNIGHT:
			return 'N';
		case PT_QUEEN:
			return 'Q';
		case PT_KING:
			return 'K';
		case PT_PAWN:
			return 'P';
		default:
			std::cerr << "Unknown piece type: " << pType << std::endl;
			std::abort();
	}
}

const char* setPieceColour(const Piece piece) {
	Player player = pieceToPlayer(piece);
	switch (player) {
		case PL_WHITE:
			return "\033[37m"; // White
		case PL_BLACK:
			//return "\033[30m"; // Black
			return "\033[31m"; // Red
		default: // Empty squares
			return "";
	}
}

const char* playerToString(const Player pl) {
	switch (pl) {
		case PL_WHITE:
			return "White";
		case PL_BLACK:
			return "Black";
		default:
			std::cerr << "Unknown player" << pl << std::endl;
			std::abort();
	}
}

const char* endTypeToString(const GameStateType type) {
	switch (type) {
		case GS_CHECKMATE:
			return "Checkmate";
		case GS_STALEMATE:
			return "Stalemate";
		default:
			std::cerr << "Unknown end type" << type << std::endl;
			std::abort();
	}
}

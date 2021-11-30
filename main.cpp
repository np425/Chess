#include <iostream>
#include "chess.h" 
#include "notation.h"
#include "FEN.h"  
#include "display.h"

//warning: ISO C++ forbids converting a string constant to ‘char*’ [-Wwrite-strings]
char* INITIAL_POS_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// ----------- Main functions prototypes
void setupGame();
void setupBoard();
void processMoves();
bool handleMove(const MoveInfo& move);

// ----------- Main functions
int main() {
	std::ios::sync_with_stdio(false);

	setupGame();

	processMoves();

	GameStateType endType = gameStateToType(gameState);
	Player player = gameStateToPlayer(gameState);

	std::cout << "Game ended with " << endTypeToString(endType) << " by " << playerToString(player) << std::endl;

	return 0;
}

void processMoves() {
	char notation[30];
	MoveInfo move;
	bool prevMove = true;

	while (true) {
	#ifdef __unix__
		std::cout << "\x1B[2J\x1B[H"; // Clear screen (Unix only)
	#endif
		displayBoard();
		if (gameState != GS_PLAYING)
			break;

		do {
			if (!prevMove) std::cout << "Invalid move" << std::endl;

			std::cout << playerToString(toMove) << " to move: " << std::flush;
			std::cin >> std::ws;
			std::cin.getline(notation, 30);

			prevMove = true;
		} while (!translateNotation(notation, move) || !handleMove(move));
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

bool handleChoice() {
	char fenString[90];
	while (true) {
		std::cout << "> ";
		int choice;
		if (!(std::cin >> choice)) {
			std::cout << "Invalid option. Try again!" << std::endl;
			continue;
		}
		switch (choice) {
			case 1:
				return loadFEN(INITIAL_POS_FEN);
			case 2:
				std::cout << "Enter FEN: " << std::flush;
				std::cin >> std::ws;
				std::cin.getline(fenString, 90);
				return loadFEN(fenString);
			default:
				std::cout << "Invalid option. Try again!" << std::endl;
				break;
		}
	}
}

void setupGame() {
	while (true) {
		std::cout << "1: Start game with standard position" << std::endl;
		std::cout << "2: Start game with selected FEN" << std::endl;
		std::cout << std::endl;
		
		if (!handleChoice()) {
			std::cerr << "Could not setup the game!" << std::endl;
			std::cout << "Please try again!" << std::endl;
		} else if (!validateBoard()) {
			std::cerr << "Invalid initial position!" << std::endl;
			std::cout << "Please try again!" << std::endl;
		} else break;
	}
}


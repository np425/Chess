#include <iostream>
#include <stdexcept>
#include "chess/standard.h"
#include "display.h"

// ----------- Main functions prototypes
void setupGame(StandardChess& chess);
void processMoves(StandardChess& chess);

// ----------- Main functions
int main() {
	std::ios::sync_with_stdio(false);

    StandardChess chess;

    setupGame(chess);

    processMoves(chess);

	StandardChess::GameStateType endType = chess.gameStateToType();
	StandardChess::Player player = chess.gameStateToPlayer();

	std::cout << "Game ended with " <<  StandardChess::gameStateTypeToString(endType) << " by " << StandardChess::playerToString(player) << std::endl;

	return 0;
}

void processMoves(StandardChess& chess) {
	char notation[30];

	while (true) {
	#ifdef __unix__
		std::cout << "\x1B[2J\x1B[H"; // Clear screen (Unix only)
	#endif
        displayBoard(chess);
		if (chess.gameOver())
			break;

        bool prevMove = false;

		do {
			if (prevMove) std::cout << "Invalid move" << std::endl;

			std::cout << StandardChess::playerToString(chess.playerToMove()) << " to move: " << std::flush;
			std::cin >> std::ws;
			std::cin.getline(notation, 30);

			prevMove = true;
		} while (!chess.loadNotation(notation));
	}
}

bool handleChoice(StandardChess& chess) {
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
                return chess.loadFEN(StandardChess::INITIAL_FEN);
			case 2:
				std::cout << "Enter FEN: " << std::flush;
				std::cin >> std::ws;
				std::cin.getline(fenString, 90);
				return chess.loadFEN(fenString);
			default:
				std::cout << "Invalid option. Try again!" << std::endl;
				break;
		}
	}
}

void setupGame(StandardChess& chess) {
	while (true) {
		std::cout << "1: Start game with standard position" << std::endl;
		std::cout << "2: Start game with selected FEN" << std::endl;
		std::cout << std::endl;

        if (!handleChoice(chess)) {
            std::cerr << "Could not setup the game!" << std::endl;
            std::cout << "Please try again!" << std::endl;
        } else {
            try {
                chess.validateBoard();
                break;
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
        }
	}
}


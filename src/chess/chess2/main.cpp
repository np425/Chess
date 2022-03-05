#include <iostream>
#include "position.h"
#include "display.h"
#include "chess.h"

using namespace chess;

enum InitialMenuChoice {
	DEFAULT_POS = 1, CUSTOM_FEN, PGN_FILE
};

InitialMenuChoice handleInitialMenu() {
	int choice;
	while (true) {
		std::cout << "Would you like to: " << std::endl;
		std::cout << "1. Start a game with default position?" << std::endl;
		std::cout << "2. Start/validate a game with a custom FEN?" << std::endl;
		std::cout << "3. Continue/validate a game from a PGN file?" << std::endl;
		std::cout << "Enter number for selected option: ";

		std::cin >> choice;
		std::cin.ignore();
		if (choice >= 1 && choice <= 3) return (InitialMenuChoice)choice;
		else std::cout << "I didn't understand, please try again." << std::endl;
	}
}

ChessGame promptInitialPosition() {
	while (true) {
		switch (handleInitialMenu()) {
			case DEFAULT_POS: {
				return ChessGame();
			}
			case CUSTOM_FEN: {
				std::cout << "To be continued, try something else!" << std::endl;
				break;
			}
			case PGN_FILE: {
				std::cout << "To be continued, try something else!" << std::endl;
				break;
			} default: {
				std::cout << "Invalid option, try again!" << std::endl;
			}
		}
	}
}

int main() {
	std::cout << "Welcome to Chess!" << std::endl;
	ChessGame chess = promptInitialPosition();

	if (!chess.validate()) {
		std::cerr << "Invalid position" << std::endl;
	}

	char notation[20];
	displayBoard(chess.getBoard());

	while (!chess.isGameOver()) {
		std::cout << playerToString(chess.getPlayer()) << " to move: ";
		std::cin >> notation;
		if (!chess.makeMove(notation)) {
			std::cerr << "Invalid move, please try again" << std::endl;
		} else {
			displayBoard(chess.getBoard());
		}
	}

	std::cout << "Game ended with " << stateToString(chess.getState())
	          << " by " << playerToString(stateToPlayer(chess.getPlayer())) << std::endl;

	return 0;
}

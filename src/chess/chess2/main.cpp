#include <iostream>
#include <fstream>
#include "position.h"
#include "display.h"
#include "chess.h"

using namespace chess;
#define MAX_FILE_NAME 100

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

bool loadPGNFromFile(const char* fileName, ChessGame& chess) {
	std::ifstream f(fileName);
	if (!f) {
		std::cerr << "Failed to open file!" << std::endl;
		return false;
	}

	char pgn[PGN_MAX_LENGTH];
	char* it = pgn;
	while (f >> std::noskipws >> *(it++));

	return chess.loadPGN(pgn);
}

void promptInitialPosition(ChessGame& chess) {
	while (true) {
		bool error = false;

		switch (handleInitialMenu()) {
			case DEFAULT_POS: {
				break;
			}
			case CUSTOM_FEN: {
				char notation[FEN_MAX_LENGTH];
				std::cout << "Enter FEN: " << std::endl;
				std::cin.getline(notation, FEN_MAX_LENGTH);
				if (!chess.loadFEN(notation)) {
					std::cerr << "Failed to load FEN, please try again!" << std::endl;
					error = true;
				}
				break;
			}
			case PGN_FILE: {
				char fileName[MAX_FILE_NAME];
				std::cout << "Enter file name: " << std::endl;
				std::cin.getline(fileName, MAX_FILE_NAME);
				if (!loadPGNFromFile(fileName, chess)) {
					std::cerr << "Failed to load PGN, please try again!" << std::endl;
					error = true;
				}
				break;
			} default: {
				std::cout << "Invalid option, try again!" << std::endl;
				error = true;
			}
		}
	
		if (error) {
			continue;
		}

		if (chess.validate()) {
			return;
		} else {
			std::cerr << "Failed to validate position" << std::endl;
		}
	}
}

int main() {
	std::cout << "Welcome to Chess!" << std::endl;
	ChessGame chess;

	//char fen[FEN_MAX_LENGTH];
	//std::cout << "Enter FEN: " << std::endl;
	//std::cin.getline(fen, FEN_MAX_LENGTH);
	//if (!chess.loadFEN(fen)) {
	//	std::cerr << "Failed to load FEN, please try again!" << std::endl;
	//}

	promptInitialPosition(chess);

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

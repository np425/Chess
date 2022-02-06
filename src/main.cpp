#include "chess/chess.h"
#include "chess/display.h"
#include <iostream>
#include <fstream>

#define MAX_FILE_NAME 100

using namespace chess;

enum InitialChoice {
	DEFAULT_POS = 1, CUSTOM_FEN, PGN_FILE
};

int handleInitialMenu() {
	int choice;
	while (true) {
		std::cout << "Would you like to: " << std::endl;
		std::cout << "1. Start a game with default position?" << std::endl;
		std::cout << "2. Start/validate a game with a custom FEN initial position?" << std::endl;
		std::cout << "3. Continue/validate a game from a PGN file?" << std::endl;
		std::cout << "Enter number for selected option: ";

		std::cin >> choice;
		std::cin.ignore();
		if (choice >= 1 && choice <= 3) return choice;
		else std::cout << "I didn't understand, please try again." << std::endl;
	}
}

bool readPGNFromFile(const char* fileName, Chess& chess) {
	// TODO: Allow reading multiple PGNs

	std::ifstream f(fileName);
	if (!f) {
		std::cerr << "Failed to open the file" << std::endl;
		return false;
	}

	char pgn[PGN_MAX_LENGTH];
	char* it = pgn;
	while (f >> std::noskipws >> *(it++));

	if (!readPGN((const char*&)it, chess.tags, chess.moves, chess.count.moves, chess.pos.state)) {
		std::cerr << "Failed to read PGN @ " << it-pgn << std::endl;
		return false;
	}
	return true;
}

void promptInitialPosition(Chess& chess) {
	while (true) {
		switch (handleInitialMenu()) {
			case DEFAULT_POS: {
				const char* it = STARTING_FEN;
				if (!readFEN(it, chess.pos, chess.count)) {
					std::cerr << "Failed to load initial FEN string @ " 
					          << it-STARTING_FEN << std::endl;
					break;
				}
				if (!validatePosition(chess.pos)) {
					std::cerr << "Failed to validate initial FEN position" << std::endl;
					break;
				}
				return;
			}
			case CUSTOM_FEN: {
				char fen[FEN_MAX_LENGTH];
				std::cout << "Enter FEN:" << std::endl;
				std::cin.getline(fen, FEN_MAX_LENGTH);
				const char* it = fen;
				if (!readFEN(it, chess.pos, chess.count)) {
					std::cerr << "Failed to load FEN string @ " << it-fen << std::endl;
					break;
				}
				if (!validatePosition(chess.pos)) {
					std::cerr << "Failed to validate FEN position" << std::endl;
					break;
				}
				return;
			}
			case PGN_FILE: {
				char fileName[MAX_FILE_NAME];
				std::cout << "Enter file name:" << std::endl;
				std::cin.getline(fileName, MAX_FILE_NAME);
				if (!readPGNFromFile(fileName, chess)) {
					break;
				}
				const char *it = STARTING_FEN;
				if (!readFEN(it, chess.pos, chess.count)) {
					std::cerr << "Failed to load FEN string @ " << it-STARTING_FEN << std::endl;
					break;
				}
				if (!validatePosition(chess.pos)) {
					std::cerr << "Failed to validate default position" << std::endl;
					break;
				}
				// TODO: Implement
				if (!buildPosFromMoves(chess.moves, chess.pos)) {
					std::cerr << "Failed to validate moves" << std::endl;
					break;
				}
				return;
			}
		}
		std::cout << "Please try again" << std::endl;
	}
}

void handleMoves(Chess& chess) {
	displayBoard(chess.pos.board);

	while (chess.pos.state == GS_PLAYING) {
		std::cout << playerToString(chess.pos.toMove) << " to move: ";

		char notation[100];
		std::cin.getline(notation, 100);
		std::cout << notation << std::endl;

		MoveInfo move;
		const char* it = notation;
		if (!readMoveNotation(it, move)) {
			std::cerr << "Failed to load move notation @ " << it-notation << std::endl;
			std::cout << "Please try again" << std::endl;
			continue;
		}

		if (!makeMove(chess.pos, move)) {
			std::cerr << "Invalid move" << std::endl;
			std::cout << "Please try again" << std::endl;
			continue;
		}

		displayBoard(chess.pos.board);
	}
}

void printResult(Chess& chess) {
	std::cout << "Game ended with " << stateToString(chess.pos.state) << std::endl;
}

int main() {
	Chess chess;

	std::cout << "Welcome to Chess!" << std::endl;

	promptInitialPosition(chess);

	handleMoves(chess);

	printResult(chess);

	return 0;
}

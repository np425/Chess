#include <iostream>
#include <fstream>
#include <cstring>
#include "chess/chess.h"
#include "chess/display.h"

using namespace chess;
#define MAX_FILE_NAME 100

enum InitialMenuChoice {
	DEFAULT_POS = 1, CUSTOM_FEN, PGN_FILE, DESCRIBE_GAME
};

enum TagMenuChoice {
	ADD_TAG = 1, QUIT_TAG_MENU
};

enum EndMenuChoice {
	SAVE_PGN = 1, PLAY_AGAIN, QUIT_GAME
};

EndMenuChoice handleEndMenu() {
	int choice;
	while (true) {
		std::cout << "Would you like to: " << std::endl;
		std::cout << "1. Save PGN to file?" << std::endl;
		std::cout << "2. Play again?" << std::endl;
		std::cout << "3. Quit the game?" << std::endl;
		std::cout << "Enter a number for selected option: ";

		std::cin >> choice;
		std::cin.ignore();
		if (choice >= SAVE_PGN && choice <= QUIT_GAME) {
			return (EndMenuChoice)choice;
		} else {
			std::cout << "I didn't understand, please try again." << std::endl;
		}
	}
}

TagMenuChoice handleTagMenu() {
	int choice;
	while (true) {
		std::cout << "Would you like to: " << std::endl;
		std::cout << "1. Add a tag?" << std::endl;
		std::cout << "2. Go back to main menu" << std::endl;
		std::cout << "Enter a number for selected option: ";

		std::cin >> choice;
		std::cin.ignore();
		if (choice >= ADD_TAG && choice <= QUIT_TAG_MENU) {
			return (TagMenuChoice)choice;
		} else {
			std::cout << "I didn't understand, please try again." << std::endl;
		}
	}
}

InitialMenuChoice handleInitialMenu() {
	int choice;
	while (true) {
		std::cout << "Would you like to: " << std::endl;
		std::cout << "1. Start a game with the default position?" << std::endl;
		std::cout << "2. Start/validate a game with a custom FEN?" << std::endl;
		std::cout << "3. Continue/validate a game from a PGN file?" << std::endl;
		std::cout << "4. Describe the game? " << std::endl;
		std::cout << "Enter a number for selected option: ";

		std::cin >> choice;
		std::cin.ignore();
		if (choice >= DEFAULT_POS && choice <= DESCRIBE_GAME) {
			return (InitialMenuChoice)choice;
		} else {
			std::cout << "I didn't understand, please try again." << std::endl;
		}
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

bool validateTagName(const char* it) {
	while (*it) {
		if (*it == '"' || *it == '[' || *it == ']') {
			return false;
		}
		++it;
	}
	return true;
}

bool validateTagValue(const char* it) {
	return validateTagName(it);
}

void describeGame(ChessGame& chess) {
	while (true) {
		std::cout << "Current game tags: " << std::endl;
		displayTags(chess.getTags());
		
		bool again = false;
		
		switch (handleTagMenu()) {
			case ADD_TAG: {
				Tag tag;

				std::cout << "Enter tag name: " << std::endl;
				std::cin >> tag.name;
				if (!validateTagName(tag.name)) {
					std::cerr << "Invalid tag name, please try again." << std::endl;
					again = true;
					break;
				}

				std::cout << "Enter tag value: " << std::endl;
				std::cin >> tag.value;
				if (!validateTagValue(tag.value)) {
					std::cerr << "Invalid tag value, please try again." << std::endl;
					again = true;
					break;
				}

				again = true;
				chess.updateTag(tag);
				break;
			} case QUIT_TAG_MENU: {
				break;
			} default: {
				std::cerr << "Invalid option, try again!" << std::endl;
				again = true;
				break;
			}
		}

		if (again) {
			continue;
		} else {
			break;
		}
	}
}

void promptInitialPosition(ChessGame& chess) {
	while (true) {
		bool again = false;

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
					again = true;
				}
				break;
			}
			case PGN_FILE: {
				char fileName[MAX_FILE_NAME];
				std::cout << "Enter file name: " << std::endl;
				std::cin.getline(fileName, MAX_FILE_NAME);
				if (!loadPGNFromFile(fileName, chess)) {
					std::cerr << "Failed to load PGN, please try again!" << std::endl;
					again = true;
				}
				break;
			} case DESCRIBE_GAME: {
				describeGame(chess);
				again = true;
				break;
			} default: {
				std::cout << "Invalid option, try again!" << std::endl;
				again = true;
			}
		}
	
		if (again) {
			continue;
		}

		if (chess.validate()) {
			return;
		} else {
			std::cerr << "Failed to validate position" << std::endl;
		}
	}
}

bool savePGNToFile(const char* fileName, const ChessGame& chess) {
	std::ofstream f(fileName);

	if (!f) {
		std::cerr << "Failed to open the file!" << std::endl;
		return false;
	}

	for (const Tag* it = chess.getTags().it(); it < chess.getTags().end(); ++it) {
		f << "[" << it->name << " \"" << it->value << "\"]" << std::endl;
	}

	f << std::endl;

	unsigned moveNum = 0;
	const MovesArray& moves = chess.getPreviousMoves();
	for (const NotatedMove* it = moves.it(); it < moves.end(); ++it) {
		if (moveNum % 2 == 0) {
			f << (moveNum / 2) + 1 << ". ";
		}
		f << it->notation << " ";
		++moveNum;
	}

	// Game can only end by stalemate or checkmate
	GameState state = chess.getState();
	switch (stateToType(state)) {
		case WIN:
		case CHECKMATE: {
			Player player = stateToPlayer(state);
			if (player == WHITE) {
				f << "1-0";
			} else {
				f << "0-1";
			}
			break;
		}
		case DRAW:
		case STALEMATE:
		case PLAYING:
			f << "1/2-1/2";
			break;
		default:
			f << "???";
	}

	return true;
}

int main() {
	std::cout << "Welcome to Chess!" << std::endl;
	while (true) {
		ChessGame chess;

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

		bool quitGame = false;
		bool repeatMenu = false;

		do {
			repeatMenu = false;

			switch (handleEndMenu()) {
				case SAVE_PGN: {
					char fileName[MAX_FILE_NAME];
					std::cout << "Enter file name: " << std::endl;
					std::cin.getline(fileName, MAX_FILE_NAME);
					if (!savePGNToFile(fileName, chess)) {
						std::cerr << "Failed to save PGN to the file, please try again!" << std::endl;
					}
					repeatMenu = true;
					break;
				} case PLAY_AGAIN: {
					repeatMenu = true;
					break;
				} case QUIT_GAME: {
					quitGame = true;
					break;
				}
			}
		} while (repeatMenu);

		if (quitGame) break;
	}

	return 0;
}

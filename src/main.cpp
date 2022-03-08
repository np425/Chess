#include <iostream>
#include <fstream>
#include <cstring>
#include "chess/chess.h"
#include "chess/display.h"

using namespace chess;
#define MAX_FILE_NAME 100

#define FEN_L_ARG "--FEN"
#define FEN_S_ARG "-F"

#define PGN_L_ARG "--PGN"
#define PGN_S_ARG "-P"

#define DEF_L_ARG "--DEF"
#define DEF_S_ARG "-D"

#define TAG_L_ARG "--TAGS"
#define TAG_S_ARG "-T"

#define SAVE_L_ARG "--SAVE"
#define SAVE_S_ARG "-S"

enum InitialMenuChoice {
	DEFAULT_POS = 1, CUSTOM_FEN, PGN_FILE, DESCRIBE_GAME
};

enum TagMenuChoice {
	ADD_TAG = 1, QUIT_TAG_MENU
};

enum EndMenuChoice {
	SAVE_PGN = 1, PLAY_AGAIN, QUIT_GAME
};

enum CLIArg {
	ARG_NONE = 0, ARG_DEF_POS = 1, ARG_FEN, ARG_PGN, ARG_TAG, ARG_SAVE
};

char saveFileName[MAX_FILE_NAME];
bool saveToFileArg = false;

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

	f << std::endl;

	return true;
}

CLIArg readArgumentName(char* str) {
	if (strcmp(str, FEN_L_ARG) == 0 || strcmp(str, FEN_S_ARG) == 0) {
		return ARG_FEN;
	} else if (strcmp(str, PGN_L_ARG) == 0 || strcmp(str, PGN_S_ARG) == 0) {
		return ARG_PGN;
	} else if (strcmp(str, DEF_L_ARG) == 0 || strcmp(str, DEF_S_ARG) == 0) {
		return ARG_DEF_POS;
	} else if (strcmp(str, TAG_L_ARG) == 0 || strcmp(str, TAG_S_ARG) == 0) {
		return ARG_TAG;
	} else if (strcmp(str, SAVE_L_ARG) == 0 || strcmp(str, SAVE_S_ARG) == 0) {
		return ARG_SAVE;
	} else {
		return ARG_NONE;
	}
}

bool loadArgumentFEN(int argc, char** argv, int& i, ChessGame& chess) {
	char notation[100];
	char* it = notation;
	int x = i+1;

	*it = 0;
	while (x < argc && readArgumentName(argv[x]) == ARG_NONE) {
		for (char* argIt = argv[x]; *argIt; ++argIt) {
			*(it++) = *argIt;
		}
		*(it++) = ' ';
		++x;
	}

	if (x == i+1) {
		// No notation has been read
		return false;
	}

	*(--it) = 0;
	i = x;
	return chess.loadFEN(notation);
}

bool loadArgFileName(int argc, char** argv, int& i, char* fileName) {
	char* it = fileName;
	int x = i+1;

	while (x < argc && readArgumentName(argv[x]) == ARG_NONE) {
		for (char* argIt = argv[x]; *argIt; ++argIt) {
			*(it++) = *argIt;
		}
		*(it++) = ' ';
		++x;
	}

	if (x == i+1) {
		// No file name has been read
		return false;
	}

	*(--it) = 0;
	i = x;
	return true;
}

bool loadArgumentPGN(int argc, char** argv, int& i, ChessGame& chess) {
	char fileName[100];
	return loadArgFileName(argc, argv, i, fileName) && loadPGNFromFile(fileName, chess);
}

bool loadArgumentTag(int argc, char** argv, int& i, char*& argIt, ChessGame& chess) {
	Tag tag;

	char* it = tag.name;
	while (i < argc && readArgumentName(argv[i]) == ARG_NONE) {
		while (*argIt && *argIt != ':') {
			*(it++) = *(argIt++);
		}

		if (it != tag.name) {
			*(it++) = ' ';
		}

		if (*argIt == ':') {
			++argIt;
			break;
		}

		++i;
		argIt = argv[i];
	}

	if (it == tag.name) {
		return false;
	}
	*(--it) = 0;

	it = tag.value;
	while (i < argc && readArgumentName(argv[i]) == ARG_NONE) {
		while (*argIt && *argIt != ',') {
			*(it++) = *(argIt++);
		}

		if (it != tag.value) {
			*(it++) = ' ';
		}

		if (*argIt == ',') {
			++argIt;
			break;
		}

		++i;
		argIt = argv[i];
	}

	if (it == tag.name) {
		return false;
	}

	*(--it) = 0;

	chess.updateTag(tag);

	return true;
}

bool loadArgumentTags(int argc, char** argv, int& i, ChessGame& chess) {
	int x = i+1;
	char* argIt = argv[x];
	while (x < argc && readArgumentName(argv[x]) == ARG_NONE) {
		if (!loadArgumentTag(argc, argv, x, argIt, chess)) {
			return false;
		}
	}

	if (x == i+1) {
		// No file name has been read
		return false;
	}

	i = x;
	return true;
}

bool loadArgumentSave(int argc, char** argv, int& i) {
	if (!loadArgFileName(argc, argv, i, saveFileName)) {
		return false;
	}

	saveToFileArg = true;
	return true;
}

bool handleArguments(int argc, char** argv, ChessGame& chess) {
	bool posSelected = false;	

	// Skips first argument, which is program name
	int i = 1;

	while (i < argc) {
		CLIArg argName = readArgumentName(argv[i]);
		if (argName == ARG_DEF_POS) {
			if (posSelected) {
				std::cerr << "Can't select multiple initial positions" << std::endl;
				return false;
			}
			// TODO: Change changePosition to updatePosition()
			chess.changePosition();
			posSelected = true;
		} else if (argName == ARG_FEN) {
			if (posSelected) {
				std::cerr << "Can't select multiple initial positions" << std::endl;
				return false;
			}
			if (!loadArgumentFEN(argc, argv, i, chess)) {
				std::cerr << "Failed to load FEN" << std::endl;
				return false;
			}
			posSelected = true;
		} else if (argName == ARG_PGN) {
			if (posSelected) {
				std::cerr << "Can't select multiple initial positions" << std::endl;
				return false;
			}
			if (!loadArgumentPGN(argc, argv, i, chess)) {
				std::cerr << "Failed to load FEN" << std::endl;
				return false;
			}
			posSelected = true;
		} else if (argName == ARG_TAG) {
			if (!loadArgumentTags(argc, argv, i, chess)) {
				std::cerr << "Failed to load tags" << std::endl;
				return false;
			}
		} else if (argName == ARG_SAVE) {
			if (saveToFileArg) {
				std::cerr << "Only one file can be selected for saving" << std::endl;
				return false;
			}
			if (!loadArgumentSave(argc, argv, i)) {
				std::cerr << "Failed to read file name" << std::endl;
				return false;
			}
		} else {
			std::cerr << "Unknown argument: " << argv[i] << std::endl;
			return false;
		}
	}
	return true;
}

void playChess(ChessGame& chess) {
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
}

int main(int argc, char **argv) {
	bool hasArgs = argc > 1;

	if (!hasArgs) {
		std::cout << "Welcome to Chess!" << std::endl;
	}

	while (true) {
		ChessGame chess;

		if (!hasArgs) {
			promptInitialPosition(chess);
		} else if (!handleArguments(argc, argv, chess)) {
			return 1;
		} 

		if (!chess.validate()) {
			std::cerr << "Failed to validate position" << std::endl;
		}

		playChess(chess);

		if (hasArgs && saveToFileArg) {
			if (!savePGNToFile(saveFileName, chess)) {
				std::cerr << "Failed to save PGN to the file, please try again!" << std::endl;
				return 1;
			}
			return 0;
		}

		bool quitGame = false;
		bool repeatMenu = false;

		do {
			repeatMenu = false;

			switch (handleEndMenu()) {
				case SAVE_PGN: {
					std::cout << "Enter file name: " << std::endl;
					std::cin.getline(saveFileName, MAX_FILE_NAME);
					if (!savePGNToFile(saveFileName, chess)) {
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

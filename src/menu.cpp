#include "menu.h"
#include "pgn_file.h"
#include <iostream>
#include "chess/notation/fen.h"
#include "chess/display.h"

using namespace chess;

bool validateTagField(const std::string& name) {
	for (auto it = name.begin(); it < name.end(); ++it) {
		if (*it == '"' || *it == '[' || *it == ']') {
			return false;
		}
	}	
	return true;
}

MenuWrapper::MenuWrapper(ChessGame* game) : game(game) {
}

bool MenuWrapper::promptFEN() {
	std::string notation;
	std::cout << "Enter FEN: " << std::endl;
	std::getline(std::cin, notation);

	FENParser parser(game);

	if (!parser.parseStr(notation.c_str())) {
		return false;
	}

	return true;
}

bool MenuWrapper::promptReadPGN() {
	std::string fileName;
	std::cout << "Enter file name: " << std::endl;
	std::getline(std::cin, fileName);

	return loadPGNFromFile(fileName, *game);
}

bool MenuWrapper::promptSavePGN() {
	std::string fileName;
	std::cout << "Enter file name: " << std::endl;
	std::getline(std::cin, fileName);

	return savePGNToFile(fileName, *game);
}

bool MenuWrapper::promptTag() {
	Tag tag;

	std::cout << "Enter tag name: " << std::endl;
	std::cin >> tag.first;

	if (!validateTagField(tag.first)) {
		return false;
	}

	std::cout << "Enter tag value: " << std::endl;
	std::cin >> tag.second;
	if (!validateTagField(tag.second)) {
		return false;
	}

	game->updateTag(tag);
	return true;
}

TagMenuChoice MenuWrapper::readTagMenuChoice() {
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

EndMenuChoice MenuWrapper::readEndMenuChoice() {
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

InitialMenuChoice MenuWrapper::readStartMenuChoice() {
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

void MenuWrapper::openStartMenu() {
	while (true) {
		switch (readStartMenuChoice()) {
			case DEFAULT_POS:
                game->setPosition();
				return;
			case CUSTOM_FEN: {
				if (!promptFEN()) {
					std::cerr << "Failed to load FEN, please try again!" << std::endl;
					break;
				} 
				return;
			case PGN_FILE:
				if (!promptReadPGN()) {
					std::cerr << "Failed to load PGN, please try again!" << std::endl;
					break;
				}
				return;
			} case DESCRIBE_GAME: {
				openTagsMenu();
				break;
			} default: {
				std::cout << "Invalid option, try again!" << std::endl;
			}
		}
	} 
}

EndMenuChoice MenuWrapper::openEndMenu() {
	while (true) {
		EndMenuChoice choice = readEndMenuChoice();

		switch (choice) {
			case SAVE_PGN: 
				if (!promptSavePGN()) {
					std::cerr << "Failed to save PGN" << std::endl;
				}
				break;
			case PLAY_AGAIN: 
			case QUIT_GAME:
				return choice;
			default:
				std::cerr << "Unknown option, please try again!" << std::endl;
		}
	} 
}

void MenuWrapper::openTagsMenu() {
	while (true) {
		std::cout << "Current game tags: " << std::endl;
		displayTags(game->getTags());
		
		switch (readTagMenuChoice()) {
			case ADD_TAG:
				if (!promptTag()) {
					std::cerr << "Failed to load tag" << std::endl;
				}
				break;
			case QUIT_TAG_MENU:
				return;
			default:
				std::cerr << "Invalid option, try again!" << std::endl;
		}
	}
}

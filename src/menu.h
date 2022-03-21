#ifndef MENU_H
#define MENU_H

#include "chess/chess.h"

enum InitialMenuChoice {
	DEFAULT_POS = 1, CUSTOM_FEN, PGN_FILE, DESCRIBE_GAME
};

enum TagMenuChoice {
	ADD_TAG = 1, QUIT_TAG_MENU
};

enum EndMenuChoice {
	SAVE_PGN = 1, PLAY_AGAIN, QUIT_GAME
};

class MenuWrapper {
	chess::ChessGame* game;

	bool promptFEN();
	bool promptReadPGN();
	bool promptSavePGN();
	bool promptTag();

public:
	explicit MenuWrapper(chess::ChessGame*);

    static EndMenuChoice readEndMenuChoice();
	static TagMenuChoice readTagMenuChoice();
	static InitialMenuChoice readStartMenuChoice();

	void openTagsMenu();
	void openStartMenu();
	EndMenuChoice openEndMenu();
};

bool validateTagField(const std::string&);

#endif

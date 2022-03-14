#include "cli_args.h"
#include <iostream>
#include <cstring>
#include "chess/notation/fen.h"
#include "chess/chess.h"
#include "pgn_file.h"

using namespace chess;

CLIArg strToArg(const char* str) {
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

CLIArgParser::CLIArgParser(ChessGame* game, const int argc, const char** argv) 
: game(game), argc(argc), argv(argv), it(argv), end(argv+argc) {
}

ChessGame* CLIArgParser::getGame() const {
	return game;
}

std::string CLIArgParser::getSaveFileName() const {
	return save;
}

bool CLIArgParser::valid() const {
	return validArgs;
}

bool CLIArgParser::hasArgs() const {
	return argc > 1;
}

bool CLIArgParser::hasSaveFileName() const {
	return !save.empty();
}

bool CLIArgParser::parse() {
	bool posSelected = false;
	bool saveToFileArg = false;
	validArgs = false;

	// Skips first argument, which is program name
	++it;

	if (it >= end) {
		return false;
	}

	while (it < end) {
		CLIArg argName = strToArg(*it);
		++it;
		switch (argName) {
			case ARG_DEF_POS:
				if (posSelected) {
					std::cerr << "Can't select multiple initial positions" << std::endl;
					return false;
				}
				game->changePosition();
				posSelected = true;
				break;
			case ARG_FEN:
				if (posSelected) {
					std::cerr << "Can't select multiple initial positions" << std::endl;
					return false;
				}
				if (!readFEN()) {
					std::cerr << "Failed to load FEN" << std::endl;
					return false;
				}
				posSelected = true;
				break;
			case ARG_PGN:
				if (posSelected) {
					std::cerr << "Can't select multiple initial positions" << std::endl;
					return false;
				}
				if (!readPGN()) {
					std::cerr << "Failed to load PGN" << std::endl;
					return false;
				}
				posSelected = true;
				break;
			case ARG_TAG:
				if (!readTags()) {
					std::cerr << "Failed to load tags" << std::endl;
					return false;
				}
				break;
			case ARG_SAVE:
				if (saveToFileArg) {
					std::cerr << "Only one file can be selected for saving" << std::endl;
					return false;
				}
				if (!readFileName(save)) {
					std::cerr << "Failed to read save file name" << std::endl;
					return false;
				}
				saveToFileArg = true;
			default:
				std::cerr << "Unknown argument: " << *(it-1) << std::endl;
				return false;
		}
	}
	validArgs = true;
	return true;
}

bool CLIArgParser::readTags() {
	Tags tags;
	const char* argIt = *it;

	while (it < end && strToArg(*it) == ARG_NONE) {
		Tag tag;
		if (!readTag(tag, argIt)) {
			return false;
		}

		if (tags.find(tag.first) == tags.end()) {
			tags.insert(tag);
		} else {
			tags[tag.first] = tag.second;
		}
	}

	if (tags.empty()) {
		// No tag has been read
		return false;
	}

	game->addTags(tags);

	return true;
}

bool CLIArgParser::readTag(Tag& tag, const char*& argIt) {
	auto tagIt = tag.first.begin();

	while (it < end && strToArg(argIt) == ARG_NONE) {
		while (*argIt && *argIt != ':') {
			*(tagIt++) = *(argIt++);
		}

		if (tagIt != tag.first.begin()) {
			*(tagIt++) = ' ';
		}

		if (*argIt == ':') {
			++argIt;
			break;
		}

		argIt = *(++it);
	}

	if (tag.first.empty()) {
		return false;
	}

	tagIt = tag.second.begin();
	while (it < end && strToArg(argIt) == ARG_NONE) {
		while (*argIt && *argIt != ',') {
			*(tagIt++) = *(argIt++);
		}

		if (tagIt != tag.second.begin()) {
			*(tagIt++) = ' ';
		}

		if (*argIt == ',') {
			++argIt;
			break;
		}

		argIt = *(++it);
	}

	if (tag.second.empty()) {
		return false;
	}

	return true;
}

bool CLIArgParser::readPGN() {
	std::string fileName;
	if (!readFileName(fileName)) {
		return false;
	}
	
	return loadPGNFromFile(fileName, *game);
}

bool CLIArgParser::readFEN() {
	std::string notation;

	while (it < end && strToArg(*it) == ARG_NONE) {
		for (const char* argIt = *it; *argIt; ++argIt) {
			notation += *argIt;
		}
		notation += ' ';
		++it;
	}

	if (notation.empty()) {
		// No notation has been read
		return false;
	}

	// Remove space
	notation.pop_back();

	Position pos;
	FENParser fen(&pos, notation.c_str());

	if (!fen.parse()) {
		return false;
	}

	game->changePosition(pos);
	return true;
}

bool CLIArgParser::readFileName(std::string& fileName) {
	while (it < end && strToArg(*it) == ARG_NONE) {
		for (const char* argIt = *it; *argIt; ++argIt) {
			fileName += *argIt;
		}

		fileName += ' ';

		++it;
	}

	if (fileName.empty()) {
		// No file name has been read
		return false;
	}

	// Remove space
	fileName.pop_back();

	return true;
}


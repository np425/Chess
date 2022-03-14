#ifndef CLI_ARGS_H
#define CLI_ARGS_H

#include "chess/chess.h"

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

enum CLIArg {
	ARG_NONE = 0, ARG_DEF_POS = 1, ARG_FEN, ARG_PGN, ARG_TAG, ARG_SAVE
};

CLIArg strToArg(const char*);

class CLIArgParser {
	std::string save;
	chess::ChessGame* game;
	const int argc;
	const char** argv;
	const char** it;
	const char** end;
	bool validArgs = false;

	bool readFEN();
	bool readFileName(std::string&);
	bool readPGN();
	bool readTag(chess::Tag&, const char*&);
	bool readTags();

public:
	CLIArgParser(chess::ChessGame*, const int, const char**);

	chess::ChessGame* getGame() const;
	std::string getSaveFileName() const;

	bool valid() const;
	bool hasArgs() const;
	bool hasSaveFileName() const;

	bool parse();
};

#endif

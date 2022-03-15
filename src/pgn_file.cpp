#include "chess/notation/pgn.h"
#include "pgn_file.h"
#include <fstream>
#include <sstream>

using namespace chess;

bool loadPGNFromFile(const std::string& fileName, ChessGame& game) {
	std::ifstream f(fileName);
	if (!f) {
		return false;
	}

	std::string pgn;
	std::stringstream buffer;
	buffer << f.rdbuf();
	pgn = buffer.str();

	PGNParser parser(&game, pgn.c_str());
	return parser.parse();
}

bool savePGNToFile(const std::string& fileName, const ChessGame& game) {
	std::ofstream f(fileName);

	if (!f) {
		return false;
	}

	Tags tags = game.getTags();
	for (auto & tag : tags) {
		f << "[" << tag.first << " \"" << tag.second << "\"]" << std::endl;
	}

	if (!tags.empty()) {
		f << std::endl;
	}

	unsigned moveNum = 0;

	Moves moves = game.getMoves();
	for (auto it = moves.begin(); it != moves.end(); ++it) {
		if (moveNum % 2 == 0) {
			f << (moveNum / 2) + 1 << ". ";
		}
		f << it->notation << " ";
		++moveNum;
	}

	// Game can only end by stalemate or checkmate
	GameState state = game.getState();
	Player player = stateToPlayer(state);

	switch (stateToType(state)) {
		case WIN:
		case CHECKMATE: 
			if (player == WHITE) {
				f << "1-0";
			} else {
				f << "0-1";
			}
			break;
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

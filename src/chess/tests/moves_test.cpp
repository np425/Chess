#include "../pgn.h"
#include "../fen.h"
#include "../position/validate.h"
#include "../position/make_move.h"
#include "../display.h" // displayMoveInfo
#include "../utils.h" // stateToPlayer
#include <iostream> // std::cerr, std::cout
#include <fstream> // std::ifstream
#include <iomanip> // std::left, std::setw
#include <cctype> // isspace

#define FILE_LENGTH 1000000
const char* const STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// TODO: Test invalid FEN positions
// TODO: Test move notation whitespaces
// TODO: Error could be in pinned, when there is enemy piece in between origin square and target square

const char* const FILE_CASES[] = {
	/*"1.pgn", "37401598675-yurics97_vs_np_42-2022-01-31.pgn",*/ "chess_com_games_2022-02-02.pgn"
};

using namespace chess;

bool testMoves(Chess& chess) {
	for (NotatedMove* it = chess.moves.moves; it < chess.moves.end; ++it) {
		unsigned moveNum = it-chess.moves.moves+1;
		//std::cout << "Move no. " << moveNum << std::endl;
		//std::cout << std::left << std::setw(17) << "Loading move:"
		//	          << it->notation << std::endl;
		if (!makeMove(chess.pos, it->move)) {
			std::cerr << "Failed to load move no. " << moveNum << ": " << it->notation << std::endl;
			displayPosInfo(chess.pos);
			return false;
		} 
			
		//std::cout << "Loaded move successfully!" << std::endl;
		//std::cout << "----" << std::endl;
		//displayPosInfo(chess.pos);
		//std::cout << "----" << std::endl << std::endl;
	}
	return true;
}

void testFile(const char* fileName) {
	std::ifstream f(fileName);
	if (!f) {
		std::cerr << "Failed to open the file: " << fileName << std::endl;
		return;
	}

	char content[FILE_LENGTH];
	char* it = content;
	
	while (f >> std::noskipws >> *(it++));
	*it = 0; // Add null terminator

	it = content;

	unsigned pgnCount = 0;

	Chess chess;
	const char* fenIt = STARTING_FEN;
	if (!readFEN(fenIt, chess.pos, chess.count)) {
		std::cerr << "Failed to load FEN" << std::endl;
		return;
	}
	
	if (!validatePosition(chess.pos)) {
		std::cerr << "Failed to validate initial position" << std::endl;
		return;
	} else std::cout << "Initial position validated" << std::endl;

	while (readPGN((const char*&)it, chess.tags, chess.moves, 
		   chess.count.moves, chess.pos.state)) {
		++pgnCount;
	
		GameState expected = chess.pos.state;
		
		std::cout << "PGN no. " << pgnCount << " loaded @ " << it-content << std::endl;
	
		if (!testMoves(chess)) {
			std::cerr << "Failed to load position " << std::endl;
			while (isspace(*it)) ++it;	
			continue;
		}

		GameState got = stateToType(chess.pos.state);
		Player player = stateToPlayer(chess.pos.state);
		int sign = (player == PL_WHITE ? 1 : -1);

		if (got == GS_CHECKMATE) got = GS_WIN;
		else if (got == GS_STALEMATE || got == GS_PLAYING) got = GS_DRAW;
		got *= sign;
	
		if (expected != got) {
			std::cerr << "- Expected result: " << stateToString(expected) << std::endl;
			std::cerr << "- But got: " << stateToString(got) << std::endl;
			displayPosInfo(chess.pos);
		} else {
			std::cout << "- Game ended with " << stateToString(chess.pos.state) << std::endl;
		}

		while (isspace(*it)) ++it;	

		fenIt = STARTING_FEN; // Reset to initial position
		readFEN(fenIt, chess.pos, chess.count);
		validatePosition(chess.pos);

		// Reset tags
		chess.tags.end = chess.tags.tags;

		// Reset moves
		chess.moves.end = chess.moves.moves;

		std::cout << std::endl;
	} 
	
	if (*it) {
		std::cerr << "PGN no. " << pgnCount+1 << " failed to load @ " 
		          << it-content << std::endl;
		return;
	}
	
	return;
}

int main() {
	unsigned casesLength = sizeof(FILE_CASES)/sizeof(*FILE_CASES);
	for (unsigned i = 0; i < casesLength; ++i) {
		std::cout << "-------------TEST FILE " << FILE_CASES[i] 
		          << " -----------------" << std::endl;
		testFile(FILE_CASES[i]);
		std::cout << std::endl << std::endl;
	}
	return 0;
}

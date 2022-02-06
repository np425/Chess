#include "../pgn.h"
#include "../display.h" // displayMoveInfo
#include <iostream> // std::cerr, std::cout
#include <fstream> // std::ifstream
#include <iomanip> // std::left, std::setw

#define PGN_LENGTH 1000

const char* const file_cases[] = {
	/*"1.pgn",*/ "37401598675-yurics97_vs_np_42-2022-01-31.pgn" 
};

using namespace chess;

void testPGNFromFile(const char* fileName) {
	std::cout << "----------------------------------------------" << std::endl;

	std::cout << "Loading PGN file: " << fileName << std::endl;

	std::ifstream f(fileName);
	if (!f) {
		std::cerr << "Failed to open file" << std::endl;
		return;
	}
	std::cout << "Opened file" << std::endl;

	char pgn[PGN_LENGTH];
	char* it = pgn;
	
	while (f >> std::noskipws >> *(it++));
	*it = 0; // Add null terminator

	std::cout << "PGN: " << std::endl;
	std::cout << pgn << std::endl;

	Chess chess;

	it = pgn;
	if (!readPGN((const char*&)it, chess.tags, chess.moves, chess.count.moves, chess.pos.state)) {
		std::cerr << "PGN failed to load @ " << *it << std::endl;
	} else std::cout << "PGN finished loading @ " << *(it-1) << std::endl;

	displayTags(chess.tags);

	//displayPGNInfo(chess.tags, chess.moves, chess.pos.state);
	
	std::cout << std::left << std::setw(17) << "Moves number:" 
	          << chess.count.moves << std::endl;

	std::cout << "----------------------------------------------" << std::endl;

	std::cout << std::endl << std::endl;
}

int main() {
	unsigned cases_length = sizeof(file_cases)/sizeof(*file_cases);
	for (unsigned i = 0; i < cases_length; ++i) {
		testPGNFromFile(file_cases[i]);
	}
	return 0;
}

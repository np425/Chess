#include "../fen.h"
#include "../display.h" // displayPosInfo
#include <iostream> // std::cerr, std::cout
#include <iomanip> // std::left, std::setw

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define MOVE_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define MOVE_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define MOVE_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

using namespace chess;

void testFEN(const char* it) {
	Chess chess;

	std::cout << "----------------------------------------------" << std::endl;

	std::cout << "Loading FEN: " << it << std::endl;

	if (!readFEN(it, chess.pos, chess.count)) {
		std::cerr << "FEN failed to load @ " << *it << std::endl;
	} else std::cout << "FEN finished loading @ " << *(it-1) << std::endl;
	
	displayPosInfo(chess.pos);

	std::cout << std::left << std::setw(17) << "Half moves:" 
	          << chess.count.half << std::endl;
	std::cout << std::left << std::setw(17) << "Moves:" 
	          << chess.count.moves << std::endl;

	std::cout << "----------------------------------------------" << std::endl;

	std::cout << std::endl << std::endl;
}

int main() {
	testFEN(STARTING_FEN);
	testFEN(MOVE_1);
	testFEN(MOVE_2);
	testFEN(MOVE_3);
	return 0;
}

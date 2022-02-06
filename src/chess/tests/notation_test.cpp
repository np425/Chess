#include "../move_notation.h"
#include "../display.h" // displayMoveInfo
#include <iostream> // std::cerr, std::cout

const char* const cases[] = {
	"e4", "e5", "e4e5", "nf3", "Nf3",
	"Qxf7+", "Qxf7#", "Qxf7+#", "4", "n4", 
	"nx4", "nx4#", "f", "p=4", "p=q", "e8=q", 
	"=", "", "n", "n=", "p=", "p", "n=Q",
	"Ne3=Q", "O", "o-O", "O-O-", "O-o-O", "o-o-o#",
	"e4 {first opening", "e4; opening", "Ne4xf7#",
	"fxe8=Q# {best opening}", "a6 {This opening is called the Ruy Lopez.}",
	"O-O# {Castling with checkmate}",
	"O-O-O+ {Better castling}", "Re1", "b5"
};

using namespace chess;

void testNotation(const char* it) {
	std::cout << "----------------------------------------------" << std::endl;

	std::cout << "Loading notation: " << it << std::endl;

	MoveInfo move;

	if (!readMoveNotation(it, move)) {
		std::cerr << "Notation failed to load @ " << *it << std::endl;
	} else {
		std::cout << "Notation finished loading @ " << *(it-1) << std::endl;
	}

	displayMoveInfo(move);

	std::cout << "----------------------------------------------" << std::endl;

	std::cout << std::endl << std::endl;
}

int main() {
	unsigned cases_length = sizeof(cases)/sizeof(*cases);
	for (unsigned i = 0; i < cases_length; ++i) {
		testNotation(cases[i]);
	}
	return 0;
}

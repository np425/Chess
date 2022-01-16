#include "display.h"
#include "chess/standard.h"
#include <iostream>

// ----------- Display functions
void displayBoard(StandardChess& chess) {
	std::cout << "  A B C D E F G H" << std::endl;
	std::cout << "-------------------" << std::endl;
	for (int y = StandardChess::BOARD_SIZE_Y-1; y >= 0; --y) {
		std::cout << y+1 << "|";
		for (int x = 0; x != StandardChess::BOARD_SIZE_X; ++x) {

		#ifdef __unix__
			std::cout << setPieceColour(chess.getBoard()[y*StandardChess::BOARD_SIZE_X+x]);
		#endif

			std::cout << pieceToChar(chess.getBoard()[y*StandardChess::BOARD_SIZE_X+x]) << " ";

		#ifdef __unix__
			std::cout << COLOUR_END;
		#endif
		}
		std::cout << "|" << y+1;
		std::cout << std::endl;
	}
	std::cout << "-------------------" << std::endl;
	std::cout << "  A B C D E F G H" << std::endl << std::endl;
}

char pieceToChar(const StandardChess::Piece piece) {
	StandardChess::PieceType pType = StandardChess::pieceToType(piece);
	switch (pType) {
        case StandardChess::PT_NONE:
			return '_';
		case StandardChess::PT_ROOK:
			return 'R';
		case StandardChess::PT_BISHOP:
			return 'B';
		case StandardChess::PT_KNIGHT:
			return 'N';
		case StandardChess::PT_QUEEN:
			return 'Q';
		case StandardChess::PT_KING:
			return 'K';
		case StandardChess::PT_PAWN:
			return 'P';
		default:
			std::cerr << "Unknown piece type: " << pType << std::endl;
			std::abort();
	}
}

#ifdef __unix__
const char* setPieceColour(const StandardChess::Piece piece) {
	StandardChess::Player player = StandardChess::pieceToPlayer(piece);
	switch (player) {
		case StandardChess::PL_WHITE:
			return COLOUR_WHITE; 
		case StandardChess::PL_BLACK:
			return COLOUR_BLACK; 
		default: // Empty squares
			return "";
	}
}
#endif


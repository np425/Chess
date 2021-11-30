#include "display.h"
#include "chess.h"
#include <iostream>

// ----------- Display functions
void displayBoard() {
	std::cout << "  A B C D E F G H" << std::endl;
	std::cout << "-------------------" << std::endl;
	for (int y = BOARD_SIZE_Y-1; y >= 0; --y) {
		std::cout << y+1 << "|";
		for (int x = 0; x != BOARD_SIZE_X; ++x) {

		#ifdef __unix__
			std::cout << setPieceColour(board[y][x]);
		#endif

			std::cout << pieceToChar(board[y][x]) << " ";

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

char pieceToChar(const Piece piece) {
	PieceType pType = pieceToType(piece);
	switch (pType) {
		case PT_NONE:
			return '_';
		case PT_ROOK:
			return 'R';
		case PT_BISHOP:
			return 'B';
		case PT_KNIGHT:
			return 'N';
		case PT_QUEEN:
			return 'Q';
		case PT_KING:
			return 'K';
		case PT_PAWN:
			return 'P';
		default:
			std::cerr << "Unknown piece type: " << pType << std::endl;
			std::abort();
	}
}

#ifdef __unix__
const char* setPieceColour(const Piece piece) {
	Player player = pieceToPlayer(piece);
	switch (player) {
		case PL_WHITE:
			return COLOUR_WHITE; 
		case PL_BLACK:
			return COLOUR_BLACK; 
		default: // Empty squares
			return "";
	}
}
#endif

const char* playerToString(const Player pl) {
	switch (pl) {
		case PL_WHITE:
			return "White";
		case PL_BLACK:
			return "Black";
		default:
			std::cerr << "Unknown player" << pl << std::endl;
			std::abort();
	}
}

const char* endTypeToString(const GameStateType type) {
	switch (type) {
		case GS_CHECKMATE:
			return "Checkmate";
		case GS_STALEMATE:
			return "Stalemate";
		default:
			std::cerr << "Unknown end type" << type << std::endl;
			std::abort();
	}
}

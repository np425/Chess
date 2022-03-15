#include "display.h"
#include <iostream>
#include <iomanip>

// TODO: Add cross-platform colours

namespace chess {

char pieceTypeToChar(PieceType type) {
	switch (type) {
		case ROOK:
			return 'R';
		case BISHOP:
			return 'B';
		case KNIGHT:
			return 'N';
		case QUEEN:
			return 'Q';
		case PAWN:
			return 'P';
		case KING:
			return 'K';
		case VOID:
			return '_';
		default:
			return 0;
	}
}

char pieceToChar(Piece piece) {
    PieceType type = pieceToType(piece);
    return pieceTypeToChar(type);
}

const char* playerToString(Player pl) {
	switch (pl) {
		case WHITE:
			return "white";
		case BLACK:
			return "black";
		case PL_NONE:
			return "none";
		default:
			return nullptr;
	}
}

const char* stateToString(GameState gameState) {
	GameStateType type = stateToType(gameState);
	switch (type) {
		case CHECKMATE:
			return "checkmate";
		case STALEMATE:
			return "stalemate";
		case PLAYING:
			return "playing";
		case WIN:
			return "win";
		case DRAW:
			return "draw";
		default:
			return nullptr;
	}
}

void displayBoard(const Board& board) {
	// File Number
	std::cout << "   | "; 
	for (int x = 0; x < BOARD_SIZE_X; ++x) {
		std::cout << (char)('A'+x) << " ";
	}
	std::cout << "|" << std::endl; 

	// Separator
	std::cout << "---|-"; 
	for (int x = 0; x < BOARD_SIZE_X; ++x) {
		std::cout << "--";
	}
	std::cout << "|---" << std::endl;

	// Board
	for (int y = BOARD_SIZE_Y-1; y >= 0; --y) {
		// Rank Number
		std::cout << " " << y+1 << " | ";

		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			Piece piece = board[y*BOARD_SIZE_X+x];

			std::cout << pieceToChar(piece) << " ";
		}

		// Rank Number
		std::cout << "| " << y+1 << std::endl;
	}

	// Separator
	std::cout << "---|-"; 
	for (int x = 0; x < BOARD_SIZE_X; ++x) {
		std::cout << "--";
	}
	std::cout << "|---" << std::endl;

	// File Number
	std::cout << "   | "; 
	for (int x = 0; x < BOARD_SIZE_X; ++x) {
		std::cout << (char)('A'+x) << " ";
	}
	std::cout << "|" << std::endl;
}


void displayPieces(const Board& board, const Coords& squares) {
	for (auto square : squares) {
		Piece piece = board[square.y * BOARD_SIZE_Y +square.x];
		std::cout << "- " << pieceTypeToChar(pieceToType(piece))
				  << "{" << square.x << "," << square.y << "}" << std::endl;
	}
}

void displayPosInfo(const Position& pos) {
	const Board board = pos.getBoard();

	displayBoard(board);

	std::cout << std::endl;

	std::cout << std::left << std::setw(17) << "Player to move:" 
			  << playerToString(pos.getPlayer()) << std::endl;

	std::cout << std::left << std::setw(17) << "Passant:" 
	          << "{" << pos.getPassant().x << "," << pos.getPassant().y 
			  << "}" << std::endl;

	std::cout << std::left << std::setw(17) << "Game state:" 
	          << stateToString(pos.getState()) << std::endl;
	
	std::cout << std::left << std::setw(17) << "White king:" 
	          << "{" << board.getKingPos(WHITE).x << "," 
			  << board.getKingPos(WHITE).y << "}" << std::endl;
	
	std::cout << std::left << std::setw(17) << "Black king:" 
			  << "{" << board.getKingPos(BLACK).x << "," 
			  << board.getKingPos(BLACK).y << "}" << std::endl;

	if (pos.getChecks().empty()) {
		std::cout << std::left << std::setw(17) << "Checks:" << "none" << std::endl;
	} else {
		std::cout << "Checks: " << std::endl;
		displayPieces(pos.getBoard(), pos.getChecks());
	}
}

void displayMoveInfo(const MoveInfo& move) {
	std::cout << std::left << std::setw(17) << "Piece Type:" 
			  << pieceTypeToChar(move.type) << std::endl;
	
	std::cout << std::left << std::setw(17) << "Promote Type:" 
	          << pieceTypeToChar(move.promote) << std::endl;
	
	std::cout << std::left << std::setw(17) << "Origin square:" << "{" 
	          << move.from.x << "," << move.from.y << "}" << std::endl;
	std::cout << std::left << std::setw(17) << "Target square:" << "{" 
	          << move.to.x << "," << move.to.y << "}" << std::endl;
	
	std::cout << std::left << std::setw(17) << "Capture:" 
	          << (move.capture ? "true" : "false") << std::endl;
	
	std::cout << std::left << std::setw(17) << "Check: " 
	          << (move.checks == CheckType::Check ? "true" : "false") << std::endl;
	std::cout << std::left << std::setw(17) << "Checkmate: " 
	          << (move.checks == CheckType::Checkmate ? "true" : "false") << std::endl;

	std::cout << "Castling permissions:" << std::endl;
	std::cout << std::left << std::setw(17) << "- Queen-side: " 
	          << (move.castles & CASTLES_QSIDE ? "true" : "false") << std::endl;
	std::cout << std::left << std::setw(17) << "- King-side: " 
	          << (move.castles & CASTLES_KSIDE ? "true" : "false") << std::endl;
}

void displayTags(const Tags& tags) {
	for (const auto & tag : tags) {
		std::cout << tag.first << ": " << tag.second << std::endl;
	}
}

}

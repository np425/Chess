#include "display.h"
#include <iostream> // std::cout, std::endl
#include <iomanip> // std::setw, std::left

// TODO: Add cross-platform colours

namespace chess {

char pieceTypeToChar(const PieceType type) {
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

const char* playerToString(const Player pl) {
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

const char* stateToString(const GameState gameState) {
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

	// Seperator
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
			PieceType type = pieceToType(piece);
			// Player player = pieceToPlayer(piece);
 
			//if (player == PL_WHITE) {
			//	std::cout << "\033[1;34m";
			//} else if (player == PL_BLACK) {
			//	std::cout << "\033[1;31m";
			//}
			
			std::cout << pieceTypeToChar(type) << " ";
			//std::cout << "\033[0m";
		}

		// Rank Number
		std::cout << "| " << y+1 << std::endl;
	}

	// Seperator
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
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		Piece piece = board[it->y * BOARD_SIZE_Y +it->x];
		std::cout << "- " << pieceTypeToChar(pieceToType(piece))
				  << "{" << it->x << "," << it->y << "}" << std::endl;
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
	for (auto it = tags.begin(); it != tags.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}
}

}

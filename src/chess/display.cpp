#include "display.h"
#include "utils.h" // pieceToType, stateToType
#include <iostream> // std::cout, std::endl
#include <iomanip> // std:;setw, std::left

// TODO: Add colours

namespace chess {

char pieceTypeToChar(const PieceType type) {
	switch (type) {
	case PT_ROOK:
		return 'R';
	case PT_BISHOP:
		return 'B';
	case PT_KNIGHT:
		return 'N';
	case PT_QUEEN:
		return 'Q';
	case PT_PAWN:
		return 'P';
	case PT_KING:
		return 'K';
	case PT_NONE:
		return '_';
	default:
		return 0;
	}
}

void displayBoard(const Piece* board) {
	std::cout << "   | "; 
	for (int x = 0; x < BOARD_SIZE_X; ++x) {
		std::cout << (char)('A'+x) << " ";
	}
	std::cout << "|" << std::endl; 
	std::cout << "---|-"; 
	for (int x = 0; x < BOARD_SIZE_X; ++x) {
		std::cout << "--";
	}
	std::cout << "|---" << std::endl;
	for (int y = BOARD_SIZE_Y-1; y >= 0; --y) {
		std::cout << " " << y+1 << " | ";
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			Piece piece = board[y*BOARD_SIZE_X+x];
			PieceType type = pieceToType(piece);
			Player player = pieceToPlayer(piece);

			if (player == PL_WHITE) std::cout << "\033[1;34m";
			else if (player == PL_BLACK) std::cout << "\033[1;31m";
			
			std::cout << pieceTypeToChar(type) << " ";
			std::cout << "\033[0m";
		}
		std::cout << "| " << y+1 << std::endl;
	}
	std::cout << "---|-"; 
	for (int x = 0; x < BOARD_SIZE_X; ++x) {
		std::cout << "--";
	}
	std::cout << "|---" << std::endl;
	std::cout << "   | "; 
	for (int x = 0; x < BOARD_SIZE_X; ++x) {
		std::cout << (char)('A'+x) << " ";
	}
	std::cout << "|" << std::endl;
}

const char* playerToString(const Player pl) {
	switch (pl) {
		case PL_WHITE:
			return "white";
		case PL_BLACK:
			return "black";
		case PL_NONE:
			return "none";
		default:
			return "????";
	}
}

const char* stateToString(const GameState gameState) {
	GameStateType type = stateToType(gameState);
	switch (type) {
	case GS_CHECKMATE:
		return "checkmate";
	case GS_STALEMATE:
		return "stalemate";
	case GS_PLAYING:
		return "playing";
	case GS_WIN:
		return "win";
	case GS_DRAW:
		return "draw";
	default:
		return "????";
	}
}

void displayPieces(const Piece* board, const CoordArray& squares) {
	for (const Coord* it = squares.coords; it < squares.end; ++it) {
		Piece piece = board[it->y*BOARD_SIZE_Y+it->x];
		std::cout << "- " << pieceTypeToChar(pieceToType(piece))
				  << "{" << it->x << "," << it->y << "}" << std::endl;
	}
}

void displayPosInfo(const Position& pos) {
	displayBoard(pos.board);

	std::cout << std::endl;

	std::cout << std::left << std::setw(17) << "Player to move:" 
			  << playerToString(pos.toMove) << std::endl;

	std::cout << std::left << std::setw(17) << "Passant:" 
	          << "{" << pos.passant.x << "," << pos.passant.y 
			  << "}" << std::endl;

	std::cout << std::left << std::setw(17) << "Game state:" 
	          << stateToString(pos.state) << std::endl;
	
	std::cout << std::left << std::setw(17) << "White king:" 
	          << "{" << pos.kingPos[PL_WHITE].x << "," 
			  << pos.kingPos[PL_WHITE].y << "}" << std::endl;
	
	std::cout << std::left << std::setw(17) << "Black king:" 
			  << "{" << pos.kingPos[PL_BLACK].x << "," 
			  << pos.kingPos[PL_BLACK].y << "}" << std::endl;

	if (pos.checks.end == pos.checks.coords) {
		std::cout << std::left << std::setw(17) << "Checks:" << "none" << std::endl;
	} else {
		std::cout << "Checks: " << std::endl;
		displayPieces(pos.board, pos.checks);
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
	          << (move.check ? "true" : "false") << std::endl;
	std::cout << std::left << std::setw(17) << "Checkmate: " 
	          << (move.checkmate ? "true" : "false") << std::endl;

	std::cout << "Castling:" << std::endl;
	std::cout << std::left << std::setw(17) << "- Queen-side: " 
	          << (move.castles[0] ? "true" : "false") << std::endl;
	std::cout << std::left << std::setw(17) << "- King-side: " 
	          << (move.castles[1] ? "true" : "false") << std::endl;
}


void displayTags(const TagsArray& tags) {
	for (const TagName* it = tags.tags; it < tags.end; it += 2) {
		std::cout << std::left << std::setw(17) << *it;
		if (it+1 == tags.end) {
			std::cout << "missing tag value";
		} else {
			std::cout << *(it+1);
		}
		std::cout << std::endl;
	}
}

void displayMoves(const MovesArray& moves) {
	for (const NotatedMove* it = moves.moves; it < moves.end; ++it) {
		std::cout << "----" << std::endl;
		std::cout << std::left << std::setw(17) << it->notation << std::endl;
		displayMoveInfo(it->move);
	}
}

void displayPGNInfo(const TagsArray& tags, const MovesArray& moves, const GameState state) {
	// Tags
	if (tags.end != tags.tags) {
		std::cout << "Tags:" << std::endl;
		displayTags(tags);
		std::cout << std::endl;
	} else {
		std::cout << std::left << std::setw(17) << "Tags:" 
	          << "none" << std::endl;
	}

	// Moves
	if (moves.end != moves.moves) {
		std::cout << "Moves:" << std::endl;
		displayMoves(moves);
		std::cout << std::endl;
	} else {
		std::cout << std::left << std::setw(17) << "Moves:" 
	          << "none" << std::endl;
	}

	// Final result
	std::cout << std::left << std::setw(17) << "Game state:" 
	          << stateToString(state) << std::endl;
}

}

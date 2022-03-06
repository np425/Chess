#include "chess.h"
#include <iostream>

namespace chess {

const TagsArray& ChessGame::getTags() const {
	return tags;
}

const MovesArray& ChessGame::getPreviousMoves() const {
	return moves;
}

ChessGame::ChessGame(const char* pgn) {
	loadPGN(pgn);
}

ChessGame::ChessGame(Board newBoard, PositionInfo posInfo, TagsArray newTags, MovesArray newMoves) 
            : Position(newBoard, posInfo) {
	for (Tag* it = newTags.it(); it < newTags.end(); ++it) {
		tags.append(*it);
	}

	for (NotatedMove* it = newMoves.it(); it < newMoves.end(); ++it) {
		moves.append(*it);
	}
}

bool ChessGame::loadPGN(const char* pgn) {
	if (!readPGN(pgn, tags, moves, fullMoves, state)) {
		return false;
	}
	if (!validate()) {
		return false;
	}
	return buildPosFromMoves();
}

bool ChessGame::buildPosFromMoves() {
	NotatedMove* move = moves.it();
	while (move < moves.end() && !isGameOver()) {
		if (!Position::makeMove(move->move)) {
			// Invalid move
			std::cerr << "Failed to load @ : " << move-moves.it() << " " << move->notation << std::endl;
			return false;
		}
		++move;
	}
	if (move < moves.end()) {
		// Game ended before all moves were read
		return false; 
	} else {
		return true;
	}
}

bool ChessGame::buildPosFromMoves(MovesArray& newMoves) {
	moves = newMoves;
	return buildPosFromMoves();
}

}

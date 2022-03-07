#include "chess.h"
#include <cstring>

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

Tag* ChessGame::findTag(const char* tagName) {
	Tag* it = tags.it();
	while (it < tags.end()) {
		if (strcmp(it->name, tagName) == 0) {
			return it;
		}
		++it;
	}
	return it;
}

void ChessGame::updateTag(Tag tag) {
	Tag* it = findTag(tag.name);
	if (it != tags.end()) {
		*it = tag;
	} else {
		tags.append(tag);
	}
}

bool ChessGame::makeMove(const char* notation) {
	NotatedMove move;
	const char* it = notation;
	if (!readMoveNotation(it, move.move) || !Position::makeMove(move.move)) {
		return false;
	}
	const char* it2 = notation;
	char* it3 = move.notation;
	while (it2 < it) {
		*(it3++) = *(it2++);
	}
	*it3 = 0;
	moves.append(move);
	return true;
}

}

#include "pgn.h"
#include "move_notation.h"
#include <cctype> 

namespace chess {

PGNParser::PGNParser(ChessGame* game) : game(game) {
}

bool PGNParser::readTag(Tag& tag) {
	std::string::iterator nameIt = tag.first.begin();
	std::string::iterator valueIt = tag.second.begin();

	if (!readChar('[')) {
		// Missing opening tag brackets	
		return false;
	}
	while (isspace(*it)) {
		++it;
	}

	// Form tag name (could be separated by spaces)
	while (*it != '"' && *it) {
		if (*nameIt == ' ') {
			++nameIt;
		}

		while (!isspace(*it) && *it != '"') {
			*(nameIt++) = *(it++);
		}

		*nameIt = ' ';
		while (isspace(*it)) {
			++it;
		}
	}

	if (tag.first.empty()) {
		// End of string, no tag name or tag exists
		return false; 
	}

	while (isspace(*it)) {
		++it;
	}

	// Opening tag value quotes
	if (!readChar('"')) {
		return false; 
	}

	// Read till second quotes
	while (*it != '"' && *it) {
		*(valueIt++) = *(it++);
	}

	// Closing tag value quotes
	if (tag.second.empty() || !readChar('"')) {
		// End of string, no tag value or no quotes
		return false; 
	} 
		
	while (isspace(*it)) {
		++it;
	}

	// Closing tag bracket
	if (!readChar(']')) {
		return false; 
	}

	return true;
}
	
bool PGNParser::readTags(Tags& tags) {
	const char* tempIt = it;

	Tag tag;
	while (readTag(tag)) {
		while (isspace(*it)) {
			++it;
		}
		tempIt = it;
		if (tags.find(tag.first) != tags.end()) {
			// Tag already exists
			return false;
		}
		tags.insert(tag);
	}

	it = tempIt;
	return true;
}

bool PGNParser::readResult(GameState& state, unsigned lastMoveNum) {
	if (readString("1/2")) {
		int sign = (lastMoveNum % 2 == 0 ? +1 : -1);
		state = sign * DRAW;
	} else if (readString("1-0")) {
		state = +WIN;
	} else if (readString("0-1")) {
		state = -WIN;
	} else {
		return false;
	}
	return true;
}

bool PGNParser::readMoveNum(unsigned& num) {
	const char* tempIt = it;

	if (!readUnsignedInt(num) || num == 0) {
		it = tempIt;
		return false; // Must be integer higher than 0
	}

	if (!readChar('.')) {
		it = tempIt;
		return false; // Must have a dot afterwards
	}

	return true;
}

bool PGNParser::readMove(NotatedMove& move) {
	// Clear to not append to move notation read prior
	move.notation.clear();

	MoveParser moveParser(&move.move);

	// Comments inclusive
	if (!moveParser.parseStr(it)) {
		return false;
	}

	const char* itEnd = moveParser.end();

	// Set notation in the moves array
	while (it != itEnd) {
		move.notation += *(it++);
	}

	return true;
}

bool PGNParser::readMoves(Moves& moves, unsigned &lastMoveNum) {
	unsigned fullMoveNum;
		
	// First move in PGN, every following move assumes in increasing-by-1 order 
	if (!readMoveNum(fullMoveNum)) {
		fullMoveNum = 1;
	}

	lastMoveNum = (fullMoveNum-1) * 2;
	while (isspace(*it)) {
		++it; 
	}
	
	// For keeping track of full moves
	unsigned count = 0; 

	NotatedMove move;
	while (readMove(move)) {
		while (isspace(*it)) {
			++it; 
		}
		++lastMoveNum;
		++count;

		// Every full move, check for optional move number
		if (count % 2 == 0 && readMoveNum(fullMoveNum)) {
			while (isspace(*it)) {
				++it; 
			}
			if (lastMoveNum != (fullMoveNum - 1) * 2) {
				// Move numbers must be increasing by one
				return false; 
			}
		} else {
			fullMoveNum = 0;
		}

		moves.push_back(move);
	}

	if (fullMoveNum) {
		// Must be a move after move number
		return false;
	}

	// After not being able to read a move
	// assume it's the end of the moves list
	return true;
}

bool PGNParser::parse() {
	Tags tags;
	Moves moves;
	GameState state;
	unsigned lastFullMove;

	while (isspace(*it)) {
		++it;
	}

	if (!readTags(tags)) {
		return false;
	}
	while (isspace(*it)) {
		++it;
	}

	if (!readMoves(moves, lastFullMove)) {
		return false;
	}

	const char *tempIt = it;

	// Result is optional
	if (!readResult(state, lastFullMove)) {
		it = tempIt;
	}

	game->addTags(tags);
	game->setMoves(moves);

	return true;
}

}

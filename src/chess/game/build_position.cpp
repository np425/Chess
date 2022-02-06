#include "build_position.h"
#include "../position/make_move.h" // makeMove

// TODO: Update full moves and half moves

namespace chess {

bool buildPosFromMoves(MovesArray& moves, Position& pos) {
	for (NotatedMove* it = moves.moves; it < moves.end; ++it) {
		if (!makeMove(pos, it->move)) return false;
	}
	return true;
}

}

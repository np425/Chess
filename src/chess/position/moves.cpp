#include "moves.h"
#include "defends.h" // defends, canDefend, getDefenders
#include "../utils.h" // pieceToType
#include <cmath> // abs
				
namespace chess {

bool pathToMoveIsClear(const Piece* board, const Coord& from, const Coord& moveFrom, 
                       const Coord& moveTo) {
	int dx = moveTo.x - from.x;
	int dy = moveTo.y - from.y;

	int xSign = numSign(dx);
	int ySign = numSign(dy);

	int ix = from.x + xSign;
	int iy = from.y + ySign;

	for (; ix != moveTo.x || iy != moveTo.y; ix += xSign, iy += ySign) {
		if (ix == moveFrom.x && iy == moveFrom.y) {
			// Skip move origin coordinate
			continue;
		}

		if (board[iy * BOARD_SIZE_X + ix]) {
			return false; // Block piece found
		}
	}

	return true;
}

bool canCastle(const Position& pos, const unsigned side, const Player pl) {
	if (!pos.canCastle[pl][side]) {
		// If moved the rook or castled, cannot castle anymore
		return false;
	}

	unsigned checksAmount = pos.checks.end - pos.checks.coords;
	if (checksAmount) {
		// No castling under check
		return false;
	}

	int ry = (BOARD_SIZE_Y-1) * pl;
	int rx = (BOARD_SIZE_X-1) * side;
	int xSign = (side ? 1 : -1);

	// Ensure empty squares in the castling direction
	for (int ix = pos.kingPos[pl].x + xSign; ix != rx; ix += xSign) {
		if (pos.board[ry * BOARD_SIZE_X + ix]) return false;
	}

	// Ensure two squares are not protected
	for (int ix = 1; ix <= 2; ++ix) {
		CoordArray defenders;
		int x = pos.kingPos[pl].x + ix * xSign;

		getDefenders(pos.board, {x,ry}, defenders, (Player)!pl);
		if (defenders.coords != defenders.end) return false;
	}

	return true;
}

// Assume origin square is a pawn
bool validPawnMove(const Position& pos, const Coord& from, const Coord& to) {
	Piece target = pos.board[from.y * BOARD_SIZE_X + from.x];
	Player pl = pieceToPlayer(target);
	int sign = getPlayerSign(pl);

	// One up
	target = pos.board[(from.y+sign)*BOARD_SIZE_X+to.x];

	int dx = to.x-from.x;
	int dy = to.y-from.y;

	// Diagonal pawn move (capture)
	if (abs(dx) == 1 && dy == sign) {	
		Player targetPl = pieceToPlayer(target);

		return targetPl == (Player)!pl || (pos.passant.x == to.x && pos.passant.y == to.y);
	}

	// Pawn up moves
	if (dx || target) {
		// Must not be move to side and must be empty target square
		return false;
	}
		
	if (dy == sign) {
		// Pawn one up move
		return true;
	}

	// Starting y for pawns
	int py = pl*(BOARD_SIZE_X-1) + sign;

	target = pos.board[(from.y+2*sign) * BOARD_SIZE_X + from.x];
	if (target) return false; // Must be empty target square
	
	// Pawn two up move
	return dy == 2 * sign && from.y == py;
}

bool isPinned(const Position& pos, const Coord& from, const Coord& to) {
	Piece piece = pos.board[from.y * BOARD_SIZE_X + from.x];
	Player pl = pieceToPlayer(piece);

	int dx = from.x - pos.kingPos[pl].x;
	int dy = from.y - pos.kingPos[pl].y;

	if ((dx != dy && dx && dy) || (!dx && !dy)) {
		// Make sure piece is placed horizontally/vertically/diagonally from the king
		// And is not the same square as the king
		return false;
	}
	
	bool expectPassant = false;
	if (pieceToType(piece) == PT_PAWN && pos.passant.x == to.x && pos.passant.y == to.y) {
		expectPassant = true;
	}

	int xSign = numSign(dx);
	int ySign = numSign(dy);

	bool foundPiece = false;

	int ix = pos.kingPos[pl].x + xSign;
	int iy = pos.kingPos[pl].y + ySign;
	// Iterate from king to the piece's direction
	for (; inBounds(ix, iy); ix += xSign, iy += ySign) {
		Piece target = pos.board[iy * BOARD_SIZE_X + ix];
		if (!target) continue; // Skip empty squares

		Player targetPl = pieceToPlayer(target);

		if (targetPl == (Player)!pl) {
			if (expectPassant && ix == pos.passant.x && iy == from.y) {
				continue; // Ignore passant piece to find pin
			} else if (foundPiece && canDefend(pos.board, {ix,iy}, pos.kingPos[pl])) {
				// TODO: Rewrite this segment
				// Make sure the piece can move in the pinned direction
				int factor = ySign * BOARD_SIZE_X + xSign;
				int toCoord = to.y * BOARD_SIZE_X + to.x;
				int kCoord = pos.kingPos[pl].y * BOARD_SIZE_X + pos.kingPos[pl].x;
				return (toCoord - kCoord) % factor != 0;
			} else break; // Enemy piece is before given piece
		} else if (targetPl == pl) {
			if (from.y == iy && from.x == ix) {
				// Found the given piece
				foundPiece = true;
			} else {
				// Friendly piece found before or after the given piece
				break; 
			}
		}
	}

	return false;
}

bool movePreventsCheck(const Position& pos, const Coord& from, const Coord& to) {
	Piece piece = pos.board[from.y * BOARD_SIZE_X + from.x];
	PieceType type = pieceToType(piece);
	Player pl = pieceToPlayer(piece);

	unsigned checksAmount = pos.checks.end - pos.checks.coords;

	if (!checksAmount) return true;

	CoordArray defenders;

	if (checksAmount == 1) {
		if (pos.checks.coords[0].x == to.x && pos.checks.coords[0].y == to.y) {
			// 1. Attack the piece
			if (type == PT_KING) {
				// If taking with the king, make sure the piece is not defended
				defenders.end = defenders.coords; // Reset head
				getDefenders(pos.board, to, defenders, (Player)!pl);
				return defenders.coords == defenders.end;
			} else {
				// If taking not with the king, it can take the piece
				return true;
			}
		} 

		if (type != PT_KING) {
			// TODO: Rewrite this segment
			// 2. Block the piece
			Coord diffCT = {pos.checks.coords[0].x-to.x, pos.checks.coords[0].y-to.y};
			Coord diffCK = {
				pos.checks.coords[0].y-pos.kingPos[pl].y,
				pos.checks.coords[0].x-pos.kingPos[pl].x
			};
			Coord signCT = {numSign(diffCT.x), numSign(diffCT.y)};
			Coord signCK = {numSign(diffCK.y), numSign(diffCK.x)};

			int factor = signCT.y * BOARD_SIZE_X + signCT.x;
			int toCoord = to.y * BOARD_SIZE_X + to.x;
			int kCoord = pos.kingPos[pl].y * BOARD_SIZE_X + pos.kingPos[pl].x;

			return signCT.x == signCK.x && signCT.y == signCK.y && (toCoord-kCoord) % factor == 0;
		}
	}

	// If there's more than 1 check, only king move is legal
	if (type == PT_KING) {
		for (int y = 0; y < BOARD_SIZE_Y; ++y) {
			for (int x = 0; x < BOARD_SIZE_X; ++x) {
				piece = pos.board[y*BOARD_SIZE_Y + x];
				PieceType targetType = pieceToType(piece);
				Player targetPl = pieceToPlayer(piece);

				if (targetPl == pl || !canDefend(pos.board, {x,y}, to)) continue;
				
				if (targetType == PT_KNIGHT || pathToMoveIsClear(pos.board, {x,y}, from, to)) {
					return false;
				}
			}
		}
		return true;
	}	

	return false;
}

bool canMove(const Position& pos, const Coord& from, const Coord& to) {
	Piece piece = pos.board[from.y * BOARD_SIZE_X + from.x];
	Player pl = pieceToPlayer(piece);
	PieceType type = pieceToType(piece);

	piece = pos.board[to.y * BOARD_SIZE_X + to.x];
	Player targetPl = pieceToPlayer(piece);

	if (targetPl == pl || (from.x == to.x && from.y == to.y)) {
		// Cannot capture your own pieces or move to the same square
		return false;
	}

	if (type == PT_PAWN) {
		if (!validPawnMove(pos, from, to)) {
			// Must be either capture or one or two up move
			return false;
		}
	} else if (!defends(pos.board, from, to)) return false;

	if (isPinned(pos, from, to)) return false;

	return movePreventsCheck(pos, from, to);
}

void getMoves(const Position& pos, const Coord& coord, CoordArray& moves, const Player by) {
	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			Piece target = pos.board[y * BOARD_SIZE_X + x];
			Player targetPl = pieceToPlayer(target);
			if (targetPl != by) continue;

			if (canMove(pos, {x,y}, coord)) {
				*(moves.end++) = {x,y};
			}
		}
	}
}

}


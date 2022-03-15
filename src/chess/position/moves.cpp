#include "../position.h"
				
namespace chess {

bool Position::isPathToMoveClear(const Coord& from, const Coord& moveFrom, const Coord& moveTo) const {
	int dx = moveTo.x - from.x;
	int dy = moveTo.y - from.y;

	int xSign = numToSign(dx);
	int ySign = numToSign(dy);

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

bool Position::canCastle(const CastlingSide side, Player pl) const {
	if (!(castlePerms[pl] & side)) {
		// If moved the rook or castled, cannot castle anymore
		return false;
	}

	if (!checks.empty()) {
		// No castling under check
		return false;
	}

	Coord kPos = board.getKingPos(pl);

	int ry = (BOARD_SIZE_Y-1) * pl;
	int rx = (BOARD_SIZE_X-1) * side;
	int xSign = (side ? 1 : -1);

	// Ensure empty squares in the castling direction
	for (int ix = kPos.x + xSign; ix != rx; ix += xSign) {
		if (board[ry * BOARD_SIZE_X + ix]) {
			return false;
		}
	}

	// Ensure two squares are not protected
	for (int ix = 1; ix <= 2; ++ix) {
		Coords defenders;
		int x = kPos.x + ix * xSign;

		getDefenders({x,ry}, defenders, (Player)!pl);
		if (!defenders.empty()) return false;
	}

	return true;
}

// Assume origin square is a pawn
bool Position::isValidPawnMove(const Coord& from, const Coord& to) const {
	Piece target = board[from.y * BOARD_SIZE_X + from.x];
	Player pl = pieceToPlayer(target);
	int sign = getPlayerSign(pl);

	// One up
	target = board[(from.y+sign)*BOARD_SIZE_X+to.x];

	int dx = to.x-from.x;
	int dy = to.y-from.y;

	// Diagonal pawn move (capture)
	if (abs(dx) == 1 && dy == sign) {	
		Player targetPl = pieceToPlayer(target);

		// Can capture or passant square
		return targetPl == (Player)!pl || (passant.x == to.x && passant.y == to.y);
	}

	// Pawn up moves
	if (dx || target) {
		// Must not be a move to the side and must be empty target square
		return false;
	}
		
	if (dy == sign) {
		// Pawn one up move
		return true;
	}

	// Starting y for pawns
	int py = pl*(BOARD_SIZE_X-1) + sign;

	target = board[(from.y + 2 * sign) * BOARD_SIZE_X + from.x];
	if (target) return false; // Must be empty target square
	
	// Pawn two up move
	return dy == 2 * sign && from.y == py;
}

bool Position::isMovePinned(const Coord& from, const Coord& to) const {
	Piece piece = board[from.y * BOARD_SIZE_X + from.x];
	Player pl = pieceToPlayer(piece);
	Coord kPos = board.getKingPos(pl);

	int dx = from.x - kPos.x;
	int dy = from.y - kPos.y;

	if ((dx != dy && dx && dy) || (!dx && !dy)) {
		// Make sure piece is placed horizontally/vertically/diagonally from the king
		// And is not the same square as the king
		return false;
	}
	
	bool expectPassant = false;
	if (pieceToType(piece) == PAWN && passant.x == to.x && passant.y == to.y) {
		expectPassant = true;
	}

	int xSign = numToSign(dx);
	int ySign = numToSign(dy);

	bool foundPiece = false;

	int ix = kPos.x + xSign;
	int iy = kPos.y + ySign;

	// Iterate from king to the piece's direction
	for (; inBounds(ix, iy); ix += xSign, iy += ySign) {
		Piece target = board[iy * BOARD_SIZE_X + ix];
		if (!target) continue; // Skip empty squares

		Player targetPl = pieceToPlayer(target);

		if (targetPl == (Player)!pl) {
			if (expectPassant && ix == passant.x && iy == from.y) {
				// Ignore passant piece to find pin
				continue; 
			} else if (foundPiece && canDefend(target, {ix,iy}, kPos)) {
				// TODO: Rewrite this segment
				// Make sure the piece can move in the pinned direction
				int factor = ySign * BOARD_SIZE_X + xSign;
				int toCoord = to.y * BOARD_SIZE_X + to.x;
				int kCoord = kPos.y * BOARD_SIZE_X + kPos.x;
				return (toCoord - kCoord) % factor != 0;
			} else {
				// Enemy piece is before given piece
				break; 
			}
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

bool Position::doesMovePreventCheck(const Coord& from, const Coord& to) const {
	Piece piece = board[from.y * BOARD_SIZE_X + from.x];
	PieceType type = pieceToType(piece);
	Player pl = pieceToPlayer(piece);

	unsigned checksAmount = checks.size();

	if (!checksAmount) return true;

	Coords defenders;

	if (checksAmount == 1) {
		if (checks[0].x == to.x && checks[0].y == to.y) {
			// 1. Attack the piece
			if (type == KING) {
				// If taking with the king, make sure the piece is not defended
				defenders.clear();
				getDefenders(to, defenders, (Player)!pl);
				return defenders.empty();
			} else {
				// If taking not with the king, it can take the piece
				return true;
			}
		} 

		if (type != KING) {
			Coord kPos = board.getKingPos(pl);
			// TODO: Rewrite this segment
			// 2. Block the piece
			Coord diffCT = {checks[0].x-to.x, checks[0].y-to.y};
			Coord diffCK = {
				checks[0].y-kPos.y,
				checks[0].x-kPos.x
			};
			Coord signCT = {numToSign(diffCT.x), numToSign(diffCT.y)};
			Coord signCK = {numToSign(diffCK.y), numToSign(diffCK.x)};

			int factor = signCT.y * BOARD_SIZE_X + signCT.x;
			int toCoord = to.y * BOARD_SIZE_X + to.x;
			int kCoord = kPos.y * BOARD_SIZE_X + kPos.x;

			return signCT.x == signCK.x && signCT.y == signCK.y && (toCoord-kCoord) % factor == 0;
		}
	}

	// If there's more than 1 check, only king move is legal
	if (type == KING) {
		for (int y = 0; y < BOARD_SIZE_Y; ++y) {
			for (int x = 0; x < BOARD_SIZE_X; ++x) {
				piece = board[y * BOARD_SIZE_Y + x];
				PieceType targetType = pieceToType(piece);
				Player targetPl = pieceToPlayer(piece);

				if (targetPl == pl || !canDefend(piece, {x,y}, to)) continue;
				
				if (targetType == KNIGHT || isPathToMoveClear({x,y}, from, to)) {
					return false;
				}
			}
		}
		return true;
	}	

	return false;
}

bool Position::canMove(const Coord& from, const Coord& to) const {
	Piece piece = board[from.y * BOARD_SIZE_X + from.x];
	Player pl = pieceToPlayer(piece);
	PieceType type = pieceToType(piece);

	piece = board[to.y * BOARD_SIZE_X + to.x];
	Player targetPl = pieceToPlayer(piece);

	if (targetPl == pl || (from.x == to.x && from.y == to.y)) {
		// Cannot capture your own pieces or move to the same square
		return false;
	}

	if (type == PAWN) {
		if (!isValidPawnMove(from, to)) {
			// Must be either capture or one or two up move
			return false;
		}
	} else if (!defends(from, to)) {
		return false;
	}

	if (isMovePinned(from, to)) {
		return false;
	}

	return doesMovePreventCheck(from, to);
}

void Position::getMoves(const Coord& coord, Coords& moves, Player by) const {
	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			Piece target = board[y * BOARD_SIZE_X + x];
			Player targetPl = pieceToPlayer(target);
			if (targetPl != by) continue;

			if (canMove({x,y}, coord)) {
				moves.push_back({x,y});
			}
		}
	}
}

}


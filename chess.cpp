#include "chess.h"
#include <iostream> // std::cerr
#include <cmath> // std::abs
#include <algorithm> // std::swap

// ----------- Global variables
Piece board[BOARD_SIZE_Y][BOARD_SIZE_X];
Square kingPos[2];

Square checks[MAX_ATTACKERS];
Square* checksEnd; 

Square passant; 
bool canCastle[2][2];

Player toMove;

GameState gameState;

// TODO: Make use of these variables
// TODO: Implement draw rules: 50-move rule, three-fold repetition
unsigned fullMoves;
unsigned halfMoves;

// ----------- Helper function prototypes
bool validateChecks();

bool isCheckmate(const Player p);
bool isStalemate(const Player p);

Square* getKnightProtects(const Square& targ, Square* it, const Player pl);
Square* getPawnProtects(const Square& targ, Square* it, const Player pl);
Square* getBRQKProtects(const Square& targ, Square* it, const Player pl);

bool canMove(const Square& from, const Square& to, const Player pl);
bool isPinned(const Square& from, const Square& to, const Player pl);

void updateGameState();

// ----------- Main functions
Square* getProtects(const Square& targ, Square* it, const Player pl) {
	it = getBRQKProtects(targ, it, pl);
	it = getKnightProtects(targ, it, pl);
	it = getPawnProtects(targ, it, pl);
	return it;
}

bool validateBoard() {
	// Ensure exactly two kings are on the board
	kingPos[0] = {-1,-1};
	kingPos[1] = {-1,-1};
	
	for (int y = 0; y != BOARD_SIZE_Y; ++y) {
		for (int x = 0; x != BOARD_SIZE_X; ++x) {
			if (pieceToType(board[y][x]) != PT_KING) continue;
				
			Player kingOwner = pieceToPlayer(board[y][x]);
			
			if (kingPos[kingOwner].y != -1) {
				std::cerr << "Multiple kings on the board." << std::endl;
				return false;
			} else kingPos[kingOwner] = {y,x};
		}
	}
	if (kingPos[0].y == -1 || kingPos[1].y == -1) {
		std::cerr << "Missing kings on the board." << std::endl;
		return false;
	}

	// Ensure kings are not together
	if (abs(kingPos[0].x - kingPos[1].x) <= 1 && abs(kingPos[0].y - kingPos[1].y) <= 1) {
		std::cerr << "Kings are together." << std::endl;
		return false; 
	}

	// Ensure no pawns on the last ranks
	for (int pl = PL_WHITE; pl <= PL_BLACK; ++pl) {
		int y = pl * (BOARD_SIZE_Y - 1);
		for (int x = 0; x != BOARD_SIZE_X; ++x) {
			if (pieceToType(board[y][x]) == PT_PAWN) {
				std::cerr << "Pawns on last ranks." << std::endl;
				return false;
			}
		}
	}

	return validateChecks();	
}

void updateGameState() {
	// Sign is from the perspective of the opponent
	int sign = getPlayerSign((Player)!toMove);
	if (isCheckmate(toMove)) 
		gameState = sign * GS_CHECKMATE;
	else if (isStalemate(toMove))
		gameState = sign * GS_STALEMATE;
	else
		gameState = GS_PLAYING;
}

Square* getMoves(const Square& targ, Square* it, const Player pl) {
	Player targPl = pieceToPlayer(board[targ.y][targ.x]);
	if (targPl == pl) return it; // Cannot take your own pieces

	Square* end = it;
	end = getBRQKProtects(targ, end, pl);
	end = getKnightProtects(targ, end, pl);
	
	int plSign = getPlayerSign(pl);

	Piece hiddenPassant = PT_NONE;
	
	// Additional pawn moves:
	int y = targ.y - plSign;
	if (y >= 0 && y < BOARD_SIZE_Y) {
		// One forward diagonally
		for (int xSign = -1; xSign <= 1; xSign += 2) {
			int x = targ.x + xSign;
			if (x < 0 || x >= BOARD_SIZE_X) continue;

			Piece piece = board[y][x];

			if (pieceToType(piece) == PT_PAWN && pieceToPlayer(piece) == pl) {
				// En passant
				if (passant.y == y && passant.x == x) {
					hiddenPassant = board[y][targ.x];
					board[y][targ.x] = PT_NONE; // Hiding passant pawn to find the pin
					*(end++) = {y,x};
				} else if (targPl != PL_NONE) {
					*(end++) = {y,x};
				}
			}
		}

		// Add pawn one forward move
		Piece piece = board[y][targ.x];
		if (pieceToType(piece) == PT_PAWN && pieceToPlayer(piece) == pl && targPl == PL_NONE) {
			*(end++) = {y,targ.x};
		}
	}

	// Add pawn two forward move
	y = targ.y - 2 * plSign;
	Piece piece = board[y][targ.x];
	int expY = pl * (BOARD_SIZE_Y-1) + plSign; // Expected Y
	if (expY == y && pieceToType(piece) == PT_PAWN && pieceToPlayer(piece) == pl && targPl == PL_NONE) {
		*(end++) = {y,targ.x};
	}

	// Filter out pieces that cannot move
	while (it < end) {
		if (canMove(*it, targ, pl)) {
			++it;
		} else {
			std::swap(*it, *(end-1));
			--end;
		}
	}

	it = end;

	// Unhide passant pawn
	if (hiddenPassant) 
		board[targ.y - plSign][targ.x] = hiddenPassant;

	return it;
}

void completeMove(const Square& from, const Square& to, const PieceType promote) {
	PieceType type = pieceToType(board[from.y][from.x]);
	int plSign = getPlayerSign(toMove);

	bool twoup = false;
	
	// Update global variables
	switch (type) { 
		case PT_PAWN:
			if (from.y + 2 * plSign == to.y) { // Two up
				twoup = true;
				// Update passant (move "behind" the pawn)
				passant = {to.y - plSign, to.x}; 
			} else if (passant.y == from.y && passant.x == to.x) { // En passant
				board[from.y][to.x] = PL_NONE;
			}
			break;
		case PT_KING: // Eliminate castling
			canCastle[toMove][0] = canCastle[toMove][1] = false;
			kingPos[toMove] = to; // Update king position
			break;
		case PT_ROOK:
			if (from.x == 0) canCastle[toMove][0] = false;
			else if (from.x == BOARD_SIZE_X - 1) canCastle[toMove][1] = false;
			break;
		default:
			break;
	}

	// Handle promotion
	Piece movePiece = (promote ? plSign * promote : board[from.y][from.x]);
	
	// Move the piece
	board[to.y][to.x] = movePiece;
	board[from.y][from.x] = PT_NONE;

	// Invalidate en passant opportunity
	if (!twoup) passant = {-1,-1};

	// Update player
	toMove = (Player)!toMove;

	// Update for enemy: checks
	checksEnd = getProtects(kingPos[toMove], checks, (Player)!toMove);

	// Update game state for player
	updateGameState();
}

bool attemptCastles(const int side) {
	if (!canCastle[toMove][side]) return false;

	int rx = side * (BOARD_SIZE_X - 1);
	int ry = toMove * (BOARD_SIZE_Y - 1);
	int xSign = (side ? 1 : -1);

	// Ensure empty squares in castling direction
	for (int x = kingPos[toMove].x; x != rx; x += xSign) {
		if (!board[ry][x]) return false;
	}
	
	Square protects[MAX_ATTACKERS];
	
	// Ensure two squares are not protected
	for (int x = 1; x <= 2; ++x) {
		if (protects != getProtects({rx, kingPos[toMove].x + x * xSign}, protects, (Player)!toMove))
			return false;
	}

	// Move pieces
	board[ry][kingPos[toMove].x + xSign] = board[ry][rx];
	board[ry][kingPos[toMove].x + 2 * xSign] = board[ry][kingPos[toMove].x];
	board[ry][rx] = PT_NONE;
	board[ry][kingPos[toMove].x] = PT_NONE;

	// Invalidate castling variables
	canCastle[toMove][0] = false;
	canCastle[toMove][1] = false;

	// Invalidate en passant opportunity
	passant = {-1,-1};
	
	// Update player
	toMove = (Player)!toMove;
	
	// Update checks
	checksEnd = getProtects(kingPos[toMove], checks, (Player)!toMove);
	
	// Update game state for player
	updateGameState();

	return true;
}

// ----------- Board validation functions
bool validateChecks() {
	// Find checks for both players, ensure no more than two checks
	Player inCheck = PL_NONE;
	for (int pl = PL_WHITE; pl <= PL_BLACK; ++pl) {
		checksEnd = getProtects(kingPos[pl], checks, (Player)!pl);
		if (checksEnd != checks) {
			if (inCheck != PL_NONE) { // Ensure both players don't have checks
				std::cerr << "Both players have checks." << std::endl;
				return false;
			} else inCheck = (Player)pl;
		}
	}

	return true;
}

// ----------- Game State Function (playing, checkmate, stalemate)
bool isCheckmate(const Player pl) {
	Square moves[MAX_ATTACKERS];
	int checksAmount = checksEnd - checks;

	if (checksAmount == 0) {
		return false;
	} else if (checksAmount == 1) { 
		// 1. Attack the attacker
		if (moves != getMoves(checks[0], moves, pl)) return false; 

		// 2. Block the attacker (only if the check is not a knight or a pawn)
		PieceType checkType = pieceToType(board[checks[0].y][checks[0].x]);
		if (checkType != PT_KNIGHT && checkType != PT_PAWN) {
			Square diff = {checks[0].y - kingPos[pl].y, checks[0].x - kingPos[pl].x};
			Square sign = {numSign(diff.y), numSign(diff.x)};

			int y = kingPos[pl].y + sign.y;
			int x = kingPos[pl].x + sign.x;

			for (int iy = y, ix = x; iy < checks[0].y && ix < checks[0].x; iy += sign.y, ix += sign.x) {
				if (moves != getMoves(checks[0], moves, pl)) return false;
			}
		}
	}

	// 3. move the king
	for (int ySign = -1; ySign <= 1; ++ySign) {
		int y = kingPos[pl].y + ySign;
		if (y < 0 || y >= BOARD_SIZE_Y) continue;
		
		for (int xSign = -1; xSign <= 1; ++xSign) {
			if (xSign == 0 && ySign == 0) continue;

			int x = kingPos[pl].x + xSign;
			Piece target = board[y][x];

			// Cannot take your own pieces
			if (x < 0 || x >= BOARD_SIZE_X || pieceToPlayer(target) == pl) continue;

			// Check if square is protected
			if (moves == getProtects({y,x}, moves, (Player)!pl)) return false;
		}
	}

	return true;
}

bool isStalemate(const Player pl) {
	Square moves[MAX_ATTACKERS];

	for (int y = 0; y < BOARD_SIZE_Y; ++y) {
		for (int x = 0; x < BOARD_SIZE_X; ++x) {
			if (pieceToPlayer(board[y][x]) == pl) continue;

			if (moves != getMoves({y,x}, moves, pl)) return false;
		}
	}
	return true;
}

// ----------- Pieces defenders functions
Square* getBRQKProtects(const Square& targ, Square* it, const Player pl) {
	for (int ySign = -1; ySign <= 1; ++ySign) {
		int y = targ.y + ySign;
		if (y < 0 || y >= BOARD_SIZE_Y) continue; // Ensure Y within bounds
		
		for (int xSign = -1; xSign <= 1; ++xSign) {
			if (!xSign && !ySign) continue; // Ensure not the same square

			int x = targ.x + xSign;
			if (x < 0 || x >= BOARD_SIZE_X) continue; // Ensure X within bounds

			// King protects
			if (pieceToPlayer(board[y][x]) == pl && pieceToType(board[y][x]) == PT_KING) {
				*(it++) = {y,x};
				break;
			}

			PieceType targType = (!xSign || !ySign ? PT_ROOK : PT_BISHOP);
			// Queen, Rook, Bishop protects
			// Iterate in the direction
			for (int iy = y, ix = x; inBounds(iy, ix); iy += ySign, ix += xSign) {
				if (!board[iy][ix]) continue;
				PieceType curType = pieceToType(board[iy][ix]);
				if (pieceToPlayer(board[iy][ix]) == pl && (curType == targType || curType == PT_QUEEN)) {
					*(it++) = {iy,ix};
				}
				break; // Break after any piece has been found
			}
		}
	}
	return it;
}

Square* getKnightProtects(const Square& targ, Square* it, const Player pl) {
	for (int z = 1; z <= 2; ++z) {
		for (int yAdd = -z; yAdd <= z; yAdd += z * 2) {
			for (int xAdd = -(z % 2) - 1; xAdd <= z % 2 + 1; xAdd += 2 * ((z % 2) + 1)) {
				int y = targ.y + yAdd;
				int x = targ.x + xAdd;
				if (!inBounds(y, x)) continue;
				if (pieceToPlayer(board[y][x]) == pl && pieceToType(board[y][x]) == PT_KNIGHT) {
					*(it++) = {y,x};
				}
			}
		}
	}
	return it;
}

Square* getPawnProtects(const Square& targ, Square* it, const Player pl) {
	int plSign = getPlayerSign(pl); 
	int y = targ.y - plSign;
	if (y < 0 || y >= BOARD_SIZE_Y) return it;
	
	for (int xSign = -1; xSign <= 1; xSign += 2) { // One forward diagonally
		int x = targ.x + xSign;
		if (x < 0 || x >= BOARD_SIZE_X) continue;
		if (pieceToType(board[y][x]) == PT_PAWN && pieceToPlayer(board[y][x]) == pl) {
			*(it++) = {y,x};
		}
	}
	return it;
}

// ----------- Move validation functions
bool isPinned(const Square& from, const Square& to, const Player pl) {
	Square diff = {from.y-kingPos[pl].y, from.x-kingPos[pl].x};
	Square sign = {numSign(diff.y), numSign(diff.x)};

	// Make sure piece is in front of the king
	if (!(diff.y == diff.x || diff.y == 0 || diff.x == 0)) return false;

	int y = kingPos[pl].y + sign.y;
	int x = kingPos[pl].x + sign.x;

	bool foundThis = false; // "from" piece
	PieceType targType = (!sign.x || !sign.y ? PT_ROOK : PT_BISHOP);

	// Iterate from king in "from" piece direction
	for (int iy = y, ix = x; inBounds(iy,ix); iy += sign.y, ix += sign.x) {
		if (!board[iy][ix]) continue;

		Piece piece = board[iy][ix];
		PieceType type = pieceToType(piece);

		if (!foundThis) {
			if (from.y == iy && from.x == ix) foundThis = true;
			else break;
		} else if (pieceToPlayer(piece) == (Player)!pl && (type == targType || type == PT_QUEEN)) {
			// Allow movements towards or backwards from the attacker
			int factor = sign.y * BOARD_SIZE_X + sign.x;
			int absTo = to.y * BOARD_SIZE_X + to.x;
			int absK = kingPos[pl].y * BOARD_SIZE_X + kingPos[pl].x;
			return !((absTo-absK) % factor == 0);
		} else break;
	}
	return false;
}

bool canMove(const Square& from, const Square& to, const Player pl) {
	int checksAmount = checksEnd - checks;

	PieceType type = pieceToType(board[from.y][from.x]);

	// Cannot take your own pieces (included in getMoves)
	// if (pieceToPlayer(board[to.y][to.x]) == pl) return false;

	// Cannot move if it is pinned to the king (special cases exist, handled)
	if (type != PT_KING && isPinned(from, to, pl)) return false;

	Square protects[MAX_ATTACKERS];

	if (checksAmount == 0) {
		return true;
	} else if (checksAmount == 1) {
		// Attack the piece
		if (checks[0].y == to.y && checks[0].x == to.x) {
			if (type == PT_KING && protects != getProtects(to, protects, (Player)!pl)) return false;
			else return true;
		// Block the piece
		} else if (type != PT_KING) {
			Square diffCT = {checks[0].y-to.y, checks[0].x-to.x};
			Square diffCK = {checks[0].y-kingPos[pl].y, checks[0].x-kingPos[pl].x};
			Square signCT = {numSign(diffCT.y), numSign(diffCT.x)};
			Square signCK = {numSign(diffCK.y), numSign(diffCK.x)};

			int factor = signCT.y * BOARD_SIZE_X + signCT.x;
			int absTo = to.y * BOARD_SIZE_X + to.x;
			int absK = kingPos[pl].y * BOARD_SIZE_X + kingPos[pl].x;

			return (signCT.y == signCK.y && signCT.x == signCK.x && (absTo - absK) % factor == 0);
		}
	} 
	if (type == PT_KING) {
		// Move the king
		return (protects == getProtects(to, protects, (Player)!pl));
	} else return false;
}

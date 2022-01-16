//
// Created by np on 2021-12-26.
//

/*
 * Available move functions
 * */

#include "../standard.h"
#include "../shared.h"

bool StandardChess::canMove(const Square& from, const Square& to, const Player pl) {
    unsigned checksAmount = checksEnd - checks;

    PieceType type = pieceToType(board[from.y*BOARD_SIZE_X+from.x]);

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
            if (type == PT_KING && protects != getProtects(to, protects, (Player)!pl))
                return false;
            else
                return true;
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

StandardChess::Square* StandardChess::getMoves(const Square& target, Square* it, const Player pl) {
    Player targetPl = pieceToPlayer(board[target.y*BOARD_SIZE_X+target.x]);
    if (targetPl == pl) return it; // Cannot take your own pieces

    Square* end = it;
    end = getBRQKProtects(target, end, pl);
    end = getKnightProtects(target, end, pl);

    int plSign = getPlayerSign(pl);

    Piece hiddenPassant = PT_NONE;

    // Additional pawn moves:
    int y = target.y - plSign;
    if (y >= 0 && y < BOARD_SIZE_Y) {
        // One forward diagonally
        for (int xSign = -1; xSign <= 1; xSign += 2) {
            int x = target.x + xSign;
            if (x < 0 || x >= BOARD_SIZE_X) continue;

            Piece piece = board[y*BOARD_SIZE_X+x];

            if (pieceToType(piece) == PT_PAWN && pieceToPlayer(piece) == pl) {
                // En passant
                if (passant.y == y && passant.x == x) {
                    hiddenPassant = board[y*BOARD_SIZE_X+target.x];
                    board[y*BOARD_SIZE_X+target.x] = PT_NONE; // Hiding passant pawn to find the pin
                    *(end++) = {y,x};
                } else if (targetPl != PL_NONE) {
                    *(end++) = {y,x};
                }
            }
        }

        // Add pawn one forward move
        Piece piece = board[y*BOARD_SIZE_X+target.x];
        if (pieceToType(piece) == PT_PAWN && pieceToPlayer(piece) == pl && targetPl == PL_NONE) {
            *(end++) = {y, target.x};
        }
    }

    // Add pawn two forward move
    y = target.y - 2 * plSign;
    Piece piece = board[y*BOARD_SIZE_X+target.x];
    int expY = pl * (BOARD_SIZE_Y-1) + plSign; // Expected Y
    if (expY == y && pieceToType(piece) == PT_PAWN && pieceToPlayer(piece) == pl && targetPl == PL_NONE) {
        *(end++) = {y, target.x};
    }

    // Filter out pieces that cannot move
    while (it < end) {
        if (canMove(*it, target, pl)) {
            ++it;
        } else {
            std::swap(*it, *(end-1));
            --end;
        }
    }

    it = end;

    // Un-hide passant pawn
    if (hiddenPassant)
        board[(target.y - plSign) * BOARD_SIZE_X + target.x] = hiddenPassant;

    return it;
}

bool StandardChess::isPinned(const Square& from, const Square& to, const Player pl) {
    Square diff = {from.y-kingPos[pl].y, from.x-kingPos[pl].x};
    Square sign = {numSign(diff.y), numSign(diff.x)};

    // Make sure piece is in front of the king
    if (!(diff.y == diff.x || diff.y == 0 || diff.x == 0)) return false;

    int y = kingPos[pl].y + sign.y;
    int x = kingPos[pl].x + sign.x;

    bool foundThis = false; // "from" piece
    PieceType targetType = (!sign.x || !sign.y ? PT_ROOK : PT_BISHOP);

    // Iterate from king in "from" piece direction
    for (int iy = y, ix = x; inBounds(iy,ix); iy += sign.y, ix += sign.x) {
        if (!board[iy*BOARD_SIZE_X+ix]) continue;

        Piece piece = board[iy*BOARD_SIZE_X+ix];
        PieceType type = pieceToType(piece);

        if (!foundThis) {
            if (from.y == iy && from.x == ix) foundThis = true;
            else break;
        } else if (pieceToPlayer(piece) == (Player)!pl && (type == targetType || type == PT_QUEEN)) {
            // Allow movements towards or backwards from the attacker
            int factor = sign.y * BOARD_SIZE_X + sign.x;
            int absTo = to.y * BOARD_SIZE_X + to.x;
            int absK = kingPos[pl].y * BOARD_SIZE_X + kingPos[pl].x;
            return (absTo - absK) % factor != 0;
        } else break;
    }
    return false;
}


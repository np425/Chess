#include "game_def.h"
#include "board_def.h"
#include <memory>
#include <cstdlib>
#include <iostream>
#include <utility>
using namespace std;

// TODO: Handle moves during checks

/* Global variables definitions */
SquareRef* pawnPass[2];

bool leftRookMoved[2];
bool rightRookMoved[2];
bool kingMoved[2];

SquareRef* kingPos[2];
bool underCheck[2];

vector<SquareRef*> checks[2];

SquareRef board[BOARD_SIZE];

////////////////////////////////////////////////////////
/* Helper functions */

// Translates movement number into movement accumulators
void translateMovement(const Movement mov, int& xacc, int& yacc) {
    //mov.ysign = movement & 1;
    //mov.y = movement >> 1 & 1;
    //mov.xsign = movement >> 2 & 1;
    //mov.x = movement >> 3 & 1;

    // Accumulator
    //mov.yacc = (mov.ysign == 0 ? 1 : -1) * mov.y;
    //mov.xacc = (mov.xsign == 0 ? 1 : -1) * mov.x;
    
    int ysign = mov & 1;
    int y = mov >> 1 & 1;
    int xsign = mov >> 2 & 1;
    int x = mov >> 3 & 1;

    // Accumulator
    yacc = (ysign == 0 ? 1 : -1) * y;
    xacc = (xsign == 0 ? 1 : -1) * x;
}

// Navigates coordinates using movement accumulators
bool navigate(int& x, int& y, const int xacc, const int yacc) {
    x += xacc;
    y += yacc;
    int coord = y * BOARD_XSIZE + x;

    if (!inBounds(x, y) || board[coord].piece) 
        return false;
    
    return true;
}

bool isPinnedToKing(const SquareRef * const sqr, const int xacc /* = 0 */, const int yacc /* = 0 */) {
    Player player = pieceToPlayer(sqr->piece);

    for (Pin pin : sqr->pins) {
        // If pinned piece is this, attacker is enemy & pinned-to piece is our own king
        if (pin.pinned == sqr && pieceToPlayer(pin.from->piece) != player && 
                    pieceToPieceType(pin.to->piece) == P_KING && pieceToPlayer(pin.to->piece) == player) {
            if (!xacc && !yacc) return true;

            int pinxacc, pinyacc;
            translateMovement(pin.movement, pinxacc, pinyacc);

            // Check movement both directions
            if (! (abs(xacc) == abs(pinxacc) && abs(yacc) == abs(pinyacc)) ) {
                return true;
            }
        }
    }

    return false;
}

bool isSquareProtected(const SquareRef * const sqr, const Player player) {
    for (SquareRef* protect : sqr->protects) {
        if (pieceToPlayer(protect->piece) != player) return true;
    }
    return false;
}

bool preventsCheck(SquareRef * moveFrom, SquareRef * moveTo) {
    // Can't block by taking
    Player player = pieceToPlayer(moveFrom->piece);

    swap(moveFrom->piece, moveTo->piece);
        
    vector<SquareRef*> protects;

    for (SquareRef* check : checks[player]) {
        protects.clear();

        getProtects(check, protects);

        for (SquareRef* protect : protects) {
            if (pieceToPieceType(protect->piece) == P_KING && pieceToPlayer(protect->piece) == player) {
                swap(moveFrom->piece, moveTo->piece);
                return false;
            }
        }
    }

    swap(moveFrom->piece, moveTo->piece);

    return true;
}


////////////////////////////////////////////////////////
/* Pin functions */

// use templates & std::array?
void getPins(SquareRef * const sqr, vector<Pin>& pins, const Movement * const movementMap, 
             const int movementSize) {

    // TODO: Test diagonals

    int coord = sqr - board;

    // Iterate through each movement
    for (int m = 0; m < movementSize; ++m) {
        int y = coord / BOARD_XSIZE;
        int x = coord % BOARD_XSIZE;

        int xacc, yacc;
        Pin pin;

        // Translate movement to iterators
        translateMovement(movementMap[m], xacc, yacc);

        pin.from = sqr;
        pin.movement = movementMap[m];
       
        // Find pinned piece (the one that can be attacked) 
        while (navigate(x, y, xacc, yacc));
        
        pin.pinned = &board[y * BOARD_XSIZE + x];

        // Find pinned-to piece (behind the piece that can be attacked)
        while (navigate(x, y, xacc, yacc));

        // Make sure the pinned-to piece exists
        if (!inBounds(x, y)) continue;
        pin.to = &board[y * BOARD_XSIZE + x];

        pins.push_back(pin);
    }
}

void getPins(SquareRef * const sqr, vector<Pin>& pins) {
    PieceType pieceType = pieceToPieceType(sqr->piece);

    switch (pieceType) {
        case P_ROOK: {
            constexpr Movement movementMap[] = { 2,3,8,12 };
            return getPins(sqr, pins, movementMap, 4);
        } case P_BISHOP: {
            constexpr Movement movementMap[] = { 10,11,14,15 };
            return getPins(sqr, pins, movementMap, 4);
        } case P_QUEEN: {
            constexpr Movement movementMap[] = { 2,3,8,12, 10,11,14,15 };
            return getPins(sqr, pins, movementMap, 8);
        } 
        case P_KNIGHT:
        case P_KING:
        case P_PAWN:
            return; // Can't pin
        default:
            cerr << "Invalid PieceType " << (int)pieceType << endl;
            abort();
    }
}

////////////////////////////////////////////////////////
/* Move projection functions */

// TODO: Checks
void getMoves(SquareRef * sqr, vector<SquareRef*>& moves, 
                 const Movement * const movementMap, const int movementSize) {
    
    Player player = pieceToPlayer(sqr->piece);

    // Iterate through each movement
    for (int m = 0; m < movementSize; ++m) {
        int coord = sqr - board;
        
        int y = coord / BOARD_XSIZE;
        int x = coord % BOARD_XSIZE;

        int xacc, yacc;
        // Translate movement to iterators
        translateMovement(movementMap[m], xacc, yacc);

        if (isPinnedToKing(sqr, xacc, yacc)) continue;

        // TODO: test if can move
        // Find pinned piece (the one that can be attacked) 
        while (navigate(x, y, xacc, yacc)) {
            if (checks[player].empty() && preventsCheck(sqr, &board[y * BOARD_XSIZE + x])) 
                moves.push_back(&board[y * BOARD_XSIZE + x]);
        }

        if (!inBounds(x, y)) continue;

        coord = y * BOARD_XSIZE + x;

        if (!board[coord].piece || pieceToPlayer(board[coord].piece) != player) {
            if (checks[coord].empty() && preventsCheck(sqr, &board[coord])) 
                moves.push_back(&board[coord]);
        }
    }
}

void getMoves(SquareRef * sqr, vector<SquareRef*>& moves) {
    PieceType pieceType = pieceToPieceType(sqr->piece);

    int coord = sqr - board;
    Player player = pieceToPlayer(sqr->piece);

    switch (pieceType) {
        case P_ROOK: {
            constexpr Movement movementMap[] = { 2,3,8,12 };
            return getMoves(sqr, moves, movementMap, 4);
        } case P_BISHOP: {
            constexpr Movement movementMap[] = { 10,11,14,15 };
            return getMoves(sqr, moves, movementMap, 4);
        } case P_QUEEN: {
            constexpr Movement movementMap[] = { 2,3,8,12, 10,11,14,15 };
            return getMoves(sqr, moves, movementMap, 8);
        } case P_KNIGHT: {
            // If the knight is pinned, it can't make any moves regardless
            if (isPinnedToKing(sqr)) return; 

            for (int z = 0; z <= 1; z += 1) {
                for (int y = -(z % 2 + 1); y <= z % 2 + 1; y += 2 * (z % 2 + 1)) {
                    for (int x = -((z + 1) % 2 + 1); x <= (z + 1) % 2 + 1; x += 2 * ((z + 1) % 2 + 1)) {
                        // Coordinate outside of the board

                        int ty = coord / BOARD_XSIZE + y;
                        int tx = coord % BOARD_XSIZE + x;

                        int target = coord + y * BOARD_XSIZE + x;
                        
                        if (!inBounds(tx, ty)) continue;

                        if (!board[target].piece || pieceToPlayer(board[target].piece) != player) {
                            if (checks[player].empty() && preventsCheck(sqr, &board[target])) 
                                moves.push_back(&board[target]);
                        }
                    }
                }
            }
            return;
        } case P_KING: {

            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    if (y == 0 && x == 0) continue;

                    int ty = coord / BOARD_XSIZE + y;
                    int tx = coord % BOARD_XSIZE + x;

                    int target = coord + y * BOARD_XSIZE + x;

                    // Coordinate outside of the board, or it's protected
                    if (!inBounds(tx, ty) || isSquareProtected(&board[target], player)) {
                        continue;
                    }

                    // Ensure square is either empty or can be taken
                    if (!board[target].piece || pieceToPlayer(board[target].piece != player)) {
                        if (checks[player].empty() && preventsCheck(sqr, &board[target])) 
                            moves.push_back(&board[target]);
                    }
                }
            }
            return;
        }
        case P_PAWN: {

            int mov = (player == P_WHITE ? +1 : -1);
            int target;
            bool canMoveOnce = false;

            // 1. movement forward once
            target = coord + mov * BOARD_XSIZE;
            if (target >= 0 && target < BOARD_SIZE && !board[target].piece && !isPinnedToKing(sqr, 0, mov)) {
                canMoveOnce = true;
                if (checks[player].empty() && preventsCheck(sqr, &board[target])) 
                    moves.push_back(&board[target]);
            }
            
            // 2. movement forward twice
            target = coord + 2 * mov * BOARD_XSIZE;

            // Only allow pawns that are at the second rank to move twice
            int cy = coord / BOARD_XSIZE;
            int my = (player == P_WHITE ? 1 : 6);
            
            if (cy == my && canMoveOnce && target >= 0 && target < BOARD_SIZE && !board[target].piece) {
                if (checks[player].empty() && preventsCheck(sqr, &board[target])) 
                    moves.push_back(&board[target]);
            }
            
            // 3. taking diagonally & en passant
            for (int x = -1; x <= 1; x += 2) {
                int target = coord + mov * BOARD_XSIZE + x;
                int tx = coord % BOARD_XSIZE + x;
                int ty = coord / BOARD_XSIZE + mov;

                if (!inBounds(tx, ty) || isPinnedToKing(sqr, x, mov)) continue;
                    
                if (!checks[player].empty() && !preventsCheck(sqr, &board[target])) continue;
                
                // En passant
                if (pawnPass[!player] == &board[coord + x] && !board[target].piece) {
                    moves.push_back(&board[target]); 
                // Normal takes diagonally
                } else if (board[target].piece && pieceToPlayer(board[target].piece) != player) {
                    moves.push_back(&board[target]);
                } 
            }
            return;
        }
        default:
            cerr << "Invalid PieceType " << (int)pieceType << endl;
            abort();
    }
}

////////////////////////////////////////////////////////
/* Square protection functions */

void getProtects(const SquareRef * const sqr, vector<SquareRef*>& protects, 
                    const Movement * const movementMap, const int movementSize) {
    
    int coord = sqr - board;

    // Iterate through each movement
    for (int m = 0; m < movementSize; ++m) {

        int y = coord / BOARD_XSIZE;
        int x = coord % BOARD_XSIZE;

        int xacc, yacc;
        // Translate movement to iterators
        translateMovement(movementMap[m], xacc, yacc);
       
        while (navigate(x, y, xacc, yacc)) {
            protects.push_back(&board[y * BOARD_XSIZE + x]);
        }

        // Can protect (defend) a piece if there is one at the end
        if (inBounds(x, y)) {
            protects.push_back(&board[y * BOARD_XSIZE + x]);
        }
    }
}

void getProtects(const SquareRef * const sqr, vector<SquareRef*>& protects) {
    PieceType pieceType = pieceToPieceType(sqr->piece);

    Player player = pieceToPlayer(sqr->piece);
    int coord = sqr - board;

    switch (pieceType) {
        case P_ROOK: {
            constexpr Movement movementMap[] = { 2,3,8,12 };
            return getProtects(sqr, protects, movementMap, 4);
        } case P_BISHOP: {
            constexpr Movement movementMap[] = { 10,11,14,15 };
            return getProtects(sqr, protects, movementMap, 4);
        } case P_QUEEN: {
            constexpr Movement movementMap[] = { 2,3,8,12, 10,11,14,15 };
            return getProtects(sqr, protects, movementMap, 8);
        } case P_KNIGHT: {
            // test, works
            for (int z = 0; z <= 1; z += 1) {
                for (int y = -(z % 2 + 1); y <= z % 2 + 1; y += 2 * (z % 2 + 1)) {
                    for (int x = -((z + 1) % 2 + 1); x <= (z + 1) % 2 + 1; x += 2 * ((z + 1) % 2 + 1)) {
                        //cout << "x: " << x << ", y: " << y << endl;
                        
                        int ty = coord / BOARD_XSIZE + y;
                        int tx = coord % BOARD_XSIZE + x;

                        int target = coord + y * BOARD_XSIZE + x;

                        // Make sure coordinate is within bounds
                        if (inBounds(tx, ty)) {
                            protects.push_back(&board[target]);
                        }
                    }
                }
            }
            return;
        } case P_KING: {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    if (y == 0 && x == 0) continue;

                    int ty = coord / BOARD_XSIZE + y;
                    int tx = coord % BOARD_XSIZE + x;

                    int target = coord + y * BOARD_XSIZE + x;

                    // Coordinate outside of the board
                    if (inBounds(tx, ty)) {
                        protects.push_back(&board[target]);
                    }
                }
            }
            return;
        }
        case P_PAWN: {

            int mov = (player == P_WHITE ? +1 : -1);
            for (int x = -1; x <= 1; x += 2) {
                int target = coord + mov * BOARD_XSIZE + x;
                
                int ty = coord / BOARD_XSIZE + mov;
                int tx = coord % BOARD_XSIZE + x;

                // Coordinate outside of the board
                if (inBounds(tx, ty)) {
                    protects.push_back(&board[target]);
                }
            }
            return;
        }
        default:
            cerr << "Invalid PieceType " << (int)pieceType << endl;
            abort();
    }
}

////////////////////////////////////////////////////////
/* Conversion functions */

string coordToString(const int coord) {
    if (coord < 0 || coord >= BOARD_SIZE) {
        cerr << "Coordinate " << coord << " exceeds limits" << endl;
        abort();
    }

    int y = coord / BOARD_XSIZE;
    int x = coord % BOARD_XSIZE;

    return string(1, 'a' + x) + string(1, '1' + y);
}

string squareToString(const SquareRef * const square) {
    return coordToString(square - board);
}

string pinToString(const Pin& pin) {
    return squareToString(pin.from) + "=>" + squareToString(pin.pinned) + "=>" + squareToString(pin.to);
}

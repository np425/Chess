#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <cctype>
#include <sstream>
#include "board_def.h"
#include "notation.h"


using namespace std;

// TODO: Fix protects and moves
// TODO: Visually display colors on the board (colors.h)
// TODO: Add castles to moves projection, fix comments on projection
// TODO: Call get available moves after every move 
// TODO: Fix pins after every move
// TODO: Handle checks
// TODO: PGN
// TODO: Rewrite vectors as lists
// TODO: Simplify meta board stuff: pins & protects, irrelevent information what piece there is
// TODO: Effectively rewrite the code

// TODO: Separate pins from available moves, search for pins first before looking for available moves

/* Type definitions */
enum GameStateType {
    G_PLAYING = 0, G_WIN_CHECKMATE, G_DRAW_STALEMATE, G_DRAW, // TODO: moves repetition, draw offer
};

typedef int GameState; // Negative sign for black, positive sign for white

/* Global variables */
bool hasMove[2];
// TODO: Implement checks

/* Functions */
void setupBoard();
void processMoves();
void updateBoard();

bool attemptMove(const NotationInfo& notation, GameState& gameState, const Player player);

/* Helper functions */
void displayBoard();
void displayProjections();
void displayProjections(const Player player);
void displayPins();
void displayProtects();
void displayPins(const Player player);
void displayNotationInfo(const NotationInfo& notationInfo);

/* Colors */
template <class T>
string COL_BLACK(T x) { 
    string str = "\033[1;30m";
    str += x;
    str += "\033[0m";
    return str;
}

template <class T>
string COL_WHITE(T x) { 
    string str = "\033[1m";
    str += x;
    str += "\033[0m";
    return str;
}

int main() {
	cout << "Made by np" << endl;
	setupBoard();
    //displayBoard();
    //displayProjections();
    //displayPins();
    processMoves();
	return 0;
}


void setupBoard() {
    // TODO: Cleanup after last play

    // Initialization of variables / cleanup
    for (int i = 0; i <= 1; ++i) {
        pawnPass[i] = nullptr;

        leftRookMoved[i] = false;
        rightRookMoved[i] = false;
        kingMoved[i] = false;
    }

	// White
	for (int i = 0; i < PIECE_MASK_SIZE; ++i) {
		board[i].piece = +PIECE_MASK[i];
	}

	// Black
	for (int i = 0; i < PIECE_MASK_SIZE; ++i) {
		int y = (BOARD_SIZE-1-i) / BOARD_XSIZE;
		int x = i % BOARD_XSIZE;
		int coord = y * BOARD_XSIZE + x;
		board[coord].piece = -PIECE_MASK[i];
	}
	
	cout << "Setup done" << endl;
}

void processMoves() {
    Player curPlayer = P_WHITE;
    int gameState = G_PLAYING;
        
    cout << "----- BOARD: ------" << endl << endl;
    displayBoard();
    cout << endl;
        
    while (gameState == G_PLAYING) {
        updateBoard();
        
        //cout << "----- PROJECTIONS: ------" << endl << endl;
        //displayProjections();
        //cout << "----- PROTECTS: ------" << endl << endl;
        //displayProtects();
        //cout << "----- PINS: ------" << endl << endl;
        //displayPins();

        int notationSign = (curPlayer == P_WHITE ? 1 : -1);

        string notation;
        cout << playerToString(curPlayer) << " to move: ";
        cin >> notation;

        NotationInfo notationInfo;
        translateNotation(notation, notationInfo);

        //cout << "----- NOTATION INFO: ------" << endl << endl;
        //displayNotationInfo(notationInfo);

        // Bad notation
        if (notationInfo.badNotation) {
            cout << "Incorrect chess notation, please try again" << endl;
            continue;
        }

        // Draw offer, TODO: test
        if (notationInfo.drawOffer) {
            string response;
            cout << playerToString(curPlayer) << " sent a draw offer do you accept? (y/n)";

            while (true) {
                cin >> response;
                // to lower
                transform(response.begin(), response.end(), response.begin(),
                    [](unsigned char c) { return tolower(c); });
                
                if (response == "y" || response == "yes") {
                    gameState = notationSign * G_DRAW;
                    break;
                } else if (response == "n" || response == "no") {
                    break;
                }
                // else continue;
            } 
        }

        if (!attemptMove(notationInfo, gameState, curPlayer)) {
            cout << "Invalid move" << endl;
            continue;
        }
        
        system("clear");

        cout << "----- BOARD: ------" << endl << endl;
        displayBoard();
        cout << endl;
        
        curPlayer = (Player)!curPlayer;
    }

    GameState gameStateType = (GameState)abs(gameState);
    Player player = (Player)(gameState < 0);

    switch (gameStateType) {
        case G_WIN_CHECKMATE:
            cout << playerToString(player) << " has won the game!" << endl;
            break;
        case G_DRAW_STALEMATE:
            cout << playerToString(player) << " has stalemated his opponent hahaha loser!" << endl;
            break;
        case G_DRAW:
            cout << "game ended with " << playerToString(player) << " sending a draw offer." << endl;
            break;
        default:
            cerr << "Unknown game state" << gameState << endl;
            abort();
    }
}

bool attemptMove(const NotationInfo& notation, GameState& gameState, const Player player) {
    // TODO: Incorporate into move projections
    if (notation.qCastles || notation.kCastles) {
        // TODO: handle checks
        // Check whether the king and the rook has been moved
        if (kingMoved[player] || (notation.qCastles && leftRookMoved[player]) || 
                (notation.kCastles && rightRookMoved[player])) {
            return false;
        }

        int mov = (notation.kCastles ? 1 : -1);
        int y = (player == P_WHITE ? 0 : 7);
        int kCoord = y * BOARD_XSIZE + 4;
        int rCoord = y * BOARD_XSIZE + (mov == 1 ? 7 : 0);
        int sign = (player == P_WHITE ? 1 : -1);

        // Ensure two squares are empty and unprotected (three for queen-side castles)
        if (board[kCoord + mov].piece || !isSquareProtected(&board[kCoord + mov], player)) return false; 
        if (board[kCoord + 2 * mov].piece || !isSquareProtected(&board[kCoord + 2 * mov], player)) return false; 
        // Make sure can castle due to pin
        if (isPinnedToKing(&board[rCoord], 1, 0)) return false;

        // TODO: update pins
        // Execute castling
        board[kCoord].piece = P_NONE;
        board[rCoord].piece = P_NONE;
        board[kCoord + mov].piece = sign * P_ROOK;
        board[kCoord + 2 * mov].piece = sign * P_KING;

        // TODO: Castles & check notation

        return true;
    }

    // Piece move
    int coord = notation.my * BOARD_XSIZE + notation.mx;
    vector<SquareRef*> moves;

    for (SquareRef* move : board[coord].moves) {
        int target = move - board;
        int ty = target / BOARD_XSIZE;
        int tx = target % BOARD_XSIZE;

        if (pieceToPieceType(move->piece) != notation.pieceType) continue;
        if (pieceToPlayer(move->piece) != player) continue;
        if (notation.px != -1 && notation.px != tx) continue;
        if (notation.py != -1 && notation.py != ty) continue;

        moves.push_back(move);
    }

    int sign = (player == P_WHITE ? +1 : -1);
    
    // Ambiguous moves
    if (moves.size() != 1) return false; 

    // Lacks taking notation
    if (board[coord].piece && !notation.takes) return false;

    SquareRef boardcopy[BOARD_SIZE];
    copy(begin(board), end(board), begin(boardcopy));

    // Move
    //swap(board[coord].piece, moves[0]->piece);
    board[coord].piece = moves[0]->piece;
    moves[0]->piece = P_NONE;

    updateBoard();

    // TODO: En passante


    // If no checks, then no check overall
    bool check = !checks[!player].empty();

    // Problem
    // If player has no move, then it's a checkmate
    bool hasMoves = hasMove[!player];
    
    // If is checkmate, must be checkmate notation
    // Otherwise if it is a check, must be check notation

    // notation.check = false, notation.checkmate = false
    // check = true, hasMoves = false

    // Check if there is a check or checkmate, enforce check notation
    if (!hasMoves != notation.checkmate || (!notation.checkmate && notation.check != check)) {
        copy(begin(boardcopy), end(boardcopy), begin(board));
        return false;
    }


    if (!hasMoves) {
        if (check) gameState = sign * G_WIN_CHECKMATE;
        else gameState = sign * G_DRAW_STALEMATE;
        return true; 
    }

    if (notation.pieceType == P_PAWN) {
        int prior = moves[0] - board;
        int mov = (player == P_WHITE ? +1 : -1);

        if (prior + 2 * mov * BOARD_XSIZE == coord) 
            pawnPass[player] = &board[coord];
    } else if (notation.pieceType == P_ROOK) {       
        int ty = (player == P_WHITE ? 0 : 7);
        
        if (ty * BOARD_XSIZE == coord) leftRookMoved[player] = true;
        else if (ty * BOARD_XSIZE + 7 == coord) rightRookMoved[player] = true;
    } else if (notation.pieceType == P_KING) {
        kingMoved[player] = true;
    }

    pawnPass[!player] = nullptr;

    return true;
}

void updateBoard() {
    // TODO: Fix nothing working

    vector<Pin> pins;
    vector<SquareRef*> protects;
    vector<SquareRef*> moves; // Projections

    // 1. Clear past move projections, square protections and pins
    for (int i = 0; i < BOARD_SIZE; ++i) {
        board[i].pins.clear();
        board[i].moves.clear();
        board[i].protects.clear();
    }

    // 2. Clear past checks
    checks[P_WHITE].clear();
    checks[P_BLACK].clear();

    // 3. Update pins
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (!board[i].piece) continue;

        pins.clear();

        getPins(&board[i], pins);
        
        for (Pin pin : pins) {
            pin.pinned->pins.push_back(pin);
        }
    }

    // 4. Update protected squares
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (!board[i].piece) continue;

        Player player = pieceToPlayer(board[i].piece);

        protects.clear();

        getProtects(&board[i], protects);

        for (SquareRef* protect : protects) {
            // 4. find checks
            if (pieceToPieceType(protect->piece) == P_KING && pieceToPlayer(protect->piece) != player) {
                checks[!player].push_back(&board[i]);
            }

            protect->protects.push_back(&board[i]);
        }
    }

    // Invalidate
    hasMove[P_WHITE] = false;
    hasMove[P_BLACK] = false;

    // 5. Update move projections
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (!board[i].piece) continue;

        Player player = pieceToPlayer(board[i].piece);

        moves.clear();

        getMoves(&board[i], moves);

        if (!moves.empty()) hasMove[player] = true;

        for (SquareRef* move : moves) {
            move->moves.push_back(&board[i]);
        }
    }
}

////////////////////////////////////////////////////////
/* Helper functions */
void displayBoard() {
    cout << "    a b c d e f g h" << endl << endl;
    for (int y = BOARD_YSIZE - 1; y >= 0; --y) {
        cout << y+1 << "   ";
        for (int x = 0; x < BOARD_XSIZE; ++x) {
            int coord = y * BOARD_XSIZE + x;

            Player player = pieceToPlayer(board[coord].piece);
            char p = pieceToChar(board[coord].piece);

            string msg = (player == P_WHITE ? COL_WHITE<char>(p) : COL_BLACK<char>(p));

            cout << msg << " ";
        }
        cout << "  " << y+1 << endl;
    }
    cout << endl << "    a b c d e f g h" << endl;
}

void displayProjections() {
    for (int y = 0; y < BOARD_YSIZE; ++y) {
        for (int x = 0; x < BOARD_XSIZE; ++x) {
            cout << (char)('a' + x) << y+1 << ":\t"; 
            for (SquareRef* sqr : board[y * BOARD_XSIZE + x].moves) {
                cout << squareToString(sqr) << ", ";
            }
            cout << endl << endl;
        }
    }
}

void displayProjections(const Player player) {
    for (int y = 0; y < BOARD_YSIZE; ++y) {
        for (int x = 0; x < BOARD_XSIZE; ++x) {
            cout << (char)('a' + x) << y+1 << ":\t"; 
            for (SquareRef* sqr : board[y * BOARD_XSIZE + x].moves) {
                if (pieceToPlayer(sqr->piece) == player) {
                    cout << squareToString(sqr) << ", ";
                }
            }
            cout << endl << endl;
        }
    }
}

void displayPins() {
    for (int y = 0; y < BOARD_YSIZE; ++y) {
        for (int x = 0; x < BOARD_XSIZE; ++x) {
            cout << (char)('a' + x) << y+1 << ":" << endl;
            for (Pin pin : board[y * BOARD_XSIZE + x].pins) {
                cout << pinToString(pin) << endl;
            }
            cout << endl;
        }
    }
}

void displayPins(const Player player) {
    for (int y = 0; y < BOARD_YSIZE; ++y) {
        for (int x = 0; x < BOARD_XSIZE; ++x) {
            cout << (char)('a' + x) << y+1 << ":" << endl;
            for (Pin pin : board[y * BOARD_XSIZE + x].pins) {
                if (pieceToPlayer(pin.from->piece) == player || pieceToPlayer(pin.pinned->piece) == player || 
                        pieceToPlayer(pin.to->piece) == player) {
                    cout << pinToString(pin) << endl;
                }
            }
            cout << endl;
        }
    }
}

void displayProtects() {
    for (int y = 0; y < BOARD_YSIZE; ++y) {
        for (int x = 0; x < BOARD_XSIZE; ++x) {
            cout << (char)('a' + x) << y+1 << ":\t"; 
            for (SquareRef* sqr : board[y * BOARD_XSIZE + x].protects) {
                cout << squareToString(sqr) << ", ";
            }
            cout << endl << endl;
        }
    }
}

void displayNotationInfo(const NotationInfo& notationInfo) {
    cout << "Bad notation: " << notationInfo.badNotation << endl;
    cout << "Piece type: " << notationInfo.pieceType << endl;
    cout << "px: " << notationInfo.px << endl;
    cout << "py: " << notationInfo.py << endl;
    cout << "mx: " << notationInfo.mx << endl;
    cout << "my: " << notationInfo.my << endl;
    cout << "takes: " << notationInfo.takes << endl;
    cout << "check: " << notationInfo.check << endl;
    cout << "checkmate: " << notationInfo.checkmate << endl;
    cout << "queen-side castle: " << notationInfo.qCastles << endl;
    cout << "king-side castle: " << notationInfo.kCastles << endl;
    cout << "draw offer: " << notationInfo.drawOffer << endl;
}


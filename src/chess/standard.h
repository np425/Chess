#ifndef CHESS_H
#define CHESS_H

/*
Chess definitions
*/

#include <stdexcept>
#include <cmath>

class StandardChess {
public:
    // Type definitions
    typedef int Piece;
    typedef int GameState;

    // Constants
    constexpr static int MAX_ATTACKERS = 16; // Max pieces that can attack a square
    constexpr static int BOARD_SIZE_X = 8;
    constexpr static int BOARD_SIZE_Y = 8;
    constexpr static const char *const INITIAL_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


    // Structure definitions
    struct Square {
        int y;
        int x;
    };
    enum Player {
        PL_WHITE = 0, PL_BLACK = 1, PL_NONE = 2
    };
    enum PieceType {
        PT_NONE = 0, PT_ROOK, PT_KNIGHT, PT_BISHOP, PT_QUEEN, PT_KING, PT_PAWN
    };
    enum GameStateType {
        GS_PLAYING = 0, GS_CHECKMATE, GS_STALEMATE
    };

    struct MoveInfo {
        PieceType pieceType = PT_NONE;
        PieceType promote = PT_NONE;

        // Prior coordinates: which piece is moving
        Square from = {-1,-1};

        // Move coordinates: where the piece is moving
        Square to = {-1,-1};

        bool capture = false;

        // bool drawOffer = false;

        bool check = false;
        bool checkmate = false;
        bool castles[2] = { false, false }; // king-side and queen-side castling
    };

protected:
    // Variables
    Piece board[BOARD_SIZE_Y * BOARD_SIZE_X]{};

    Player toMove = PL_NONE; // Player to move

    Square kingPos[2]{}; // Holds king positions for quick access
    bool canCastle[2][2]{}; // For white & black, left & right
    Square passant{}; // A pawn that has just moved twice

    unsigned halfMoves{}; // TODO: To implement
    unsigned fullMoves{}; // TODO: To implement

    Square checks[MAX_ATTACKERS]{};
    Square *checksEnd{};

    GameState gameState = GS_PLAYING;

    // Move functions
    Square *getMoves(const Square &target, Square *it, Player pl);

    bool canMove(const Square &from, const Square &to, Player pl);

    bool isPinned(const Square &from, const Square &to, Player pl);


    // Notation functions
    static int readPiece(const char *&it, Piece &piece);

    static bool readPlayer(const char *&it, Player &player);

    static bool readPassant(const char *&it, StandardChess::Square &sqr);

    static bool readCastlingRights(const char *&it, bool castlingRights[2][2]);


public:
    // Validation functions
    void validateChecks();

    void validateBoard();

protected:
    // Protecting (defendinging) functions
    Square *getProtects(const Square &target, Square *it, Player pl);

    Square *getPawnProtects(const Square &target, Square *it, Player pl);

    Square *getKnightProtects(const Square &target, Square *it, Player pl);

    Square *getBRQKProtects(const Square &target, Square *it, Player pl);


    // Gamestate functions
    void updateGameState();

    bool isCheckmate(Player pl);

    bool isStalemate(Player pl);


public:
    // Helper functions
    static inline Player pieceToPlayer(const Piece piece) {
        if (piece > 0)
            return PL_WHITE;
        else if (piece < 0)
            return PL_BLACK;
        else
            return PL_NONE;
    }

    static inline PieceType pieceToType(const Piece piece) {
        return (PieceType) abs(piece);
    }


    static inline bool inBounds(const int y, const int x) {
        return (y >= 0 && y < BOARD_SIZE_Y && x >= 0 && x < BOARD_SIZE_X);
    }

    // Assume no PL_NONE
    static inline int getPlayerSign(const Player p) {
        return (p == PL_WHITE ? 1 : -1);
    }

    [[nodiscard]] inline GameStateType gameStateToType() const {
        return (GameStateType) abs(gameState);
    }

    [[nodiscard]] inline Player gameStateToPlayer() const {
        if (gameState > 0)
            return PL_WHITE;
        else if (gameState < 0)
            return PL_BLACK;
        else
            return PL_NONE;
    }

    static inline const char* gameStateTypeToString(GameStateType type) {
        switch (type) {
            case GS_CHECKMATE:
                return "Checkmate";
            case GS_STALEMATE:
                return "Stalemate";
            case GS_PLAYING:
                return "Playing";
            default:
                return "Unknown";
        }
    }

    static inline const char* playerToString(const Player pl) {
        switch (pl) {
            case PL_WHITE:
                return "White";
            case PL_BLACK:
                return "Black";
            default:
                return "Unknown";
        }
    }

public:
//    explicit StandardChess(const char *fen = INITIAL_FEN) {
//        if (!StandardChess::loadFEN(fen))
//            throw std::runtime_error("Invalid initial FEN");
//        StandardChess::validateBoard();
//    }

    explicit StandardChess() = default;

    // Notation functions
    bool loadFEN(const char *fen) ;

    bool loadPGN(const char *pgn) ; // TODO: To implement

    inline const Piece *getBoard() { return board; };

    [[nodiscard]] inline bool gameOver() const { return gameState != GS_PLAYING; }

    inline Player playerToMove() { return toMove; }

    // Available moves
    Square* getAvailableMoves(const Square& target, Square* it) {
        return getMoves(target, it, toMove);
    }

    // Chess notation
    static bool translateNotation(const char* notation, MoveInfo& info);

    bool loadNotation(const char* notation);

    // Finisher moves
    void completeMove(const Square &from, const Square &to, PieceType promote = PT_NONE);

    bool attemptCastles(int side);
};

#endif
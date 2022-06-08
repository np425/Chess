// TODO: When using standard position, make validation skip-able

#include "chessl/chess.h"
#include "chessl/display.h"
#include "pgn_file.h"
#include <fstream>

using namespace chess;

int main() {
    ChessGame game;

    std::string fileName;
    std::cout << "Enter file name: ";
    std::cin >> fileName;

    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Failed to open the file!" << std::endl;
        return 1;
    }

    if (!loadPGNFromFile(file, game)) {
        std::cerr << "Failed to load PGN from the file!" << std::endl;
        return 1;
    }

    printBoard(game.getBoard());

    if (!game.validate()) {
        std::cerr << "Failed to validate the game!" << std::endl;
        return 1;
    }

    printBoard(game.getBoard());

    if (!game.buildPos()) {
        std::cerr << "Failed to build position" << std::endl;
        return 1;
    }

    printBoard(game.getBoard());
    std::cout << "Successfully built the position!" << std::endl;

    State state = game.getState();
    Player pl = stateToPlayer(state);
    StateType stateType = stateToType(state);

    std::cout << "Game state: " << stateTypeToString(stateType);
    if (stateType != PLAYING) {
        std::cout << " by " << playerToString(pl);
    }
    std::cout << std::endl;

    return 0;
}
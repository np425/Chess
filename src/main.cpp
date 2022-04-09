#include <iostream>
#include "chess/chess.h"
#include "chess/display.h"
#include "chess/notation/move_notation.h"
#include "pgn_file.h"
#include "menu.h"
#include "cli_args.h"
#include "switch_move_notation.h"

using namespace chess;

GameState playChess(ChessGame &game) {
    NotatedMove move;
    unsigned moveNo;

    MoveParser moveParser(&move.move);
    MoveSwitchParser moveSwitchParser(&moveNo);

    displayInterface(game);

    while (!game.isGameOver()) {
        std::cout << playerToString(game.getPos().getPlayer()) << " to move: ";
        std::cin >> move.notation;

        if (!moveParser.parseStr(move.notation.c_str())) {
            if (!moveSwitchParser.parseStr(move.notation.c_str())) {
                std::cerr << "Invalid move notation" << std::endl;
                continue;
            }

            game.switchMove(moveNo);
            displayInterface(game);
        } else if (!game.makeMove(move)) {
            std::cerr << "Invalid move in position" << std::endl;
        } else {
            displayInterface(game);
        }
    }

    GameState state = game.getPos().getState();
    std::cout << "Game ended with " << stateToString(state)
              << " by " << playerToString(stateToPlayer(state)) << std::endl;

    return state;
}

int main(int argc, char **argv) {
    ChessGame game;
    CLIArgParser args(&game, argc, (const char **) argv);
    MenuWrapper menu(&game);

    bool again = false;

    do {
        if (!again && args.hasArgs()) {
            if (!args.parse()) {
                return 1;
            }
        } else {
            if (!again) {
                std::cout << "Welcome to Chess!" << std::endl;
            }
            menu.openStartMenu();
        }

        if (!game.validate()) {
            std::cerr << "Failed to validate position" << std::endl;
            return 1;
        }

        if (!game.buildPos()) {
            std::cerr << "Failed to build position" << std::endl;
            return 1;
        }

        playChess(game);

        if (!again && args.hasArgs()) {
            std::string fileName = args.getSaveFileName();
            if (!fileName.empty() && !savePGNToFile(fileName, game)) {
                std::cerr << "Failed to save the file" << std::endl;
                return 1;
            }
        } else {
            EndMenuChoice choice = menu.openEndMenu();
            if (choice == QUIT_GAME) {
                return 0;
            }
            again = true;
        }
    } while (again);

    return 0;
}

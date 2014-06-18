#include "CheckersGame.h"


int main() {
    CheckerGame* game = newGame();

    while (checkEndGame(game) == CHECKER_NONE) {
        // Draw game
        drawGame(game);
    }

    drawGame(game);
    // TODO a victory screen
    getch();

    return 0;
}


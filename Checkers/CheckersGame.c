#include "CheckersGame.h"


CheckerGame* currentGame;


static void    getCheckerPosition(CheckerGame* game, Point position, float* i, float* j);
static void    mouseEventListener(int x, int y, MouseEvent event);
static boolean hasSelection(CheckerGame* game);
static void    clearStates(CheckerGame* game);
static void    selectSpace(CheckerGame* game, int i, int j);
static boolean isFreeSpace(CheckerGame* game, int i, int j);
static boolean hasAdversary(CheckerGame* game, int i, int j, Checker type);
static void    checkSimpleMove(CheckerGame* game, int i, int j);
static void    checkCaptureMove(CheckerGame* game, int i2, int j2, int i1, int j1, Checker type);
static void    getAdjacentFreeMoves(CheckerGame* game, Point position, Checker type);
static void    getCaptureMoves(CheckerGame* game, Point position, Checker type);
static void    calculatePossibleMoves(CheckerGame* game);
static void    moveChecker(CheckerGame* game);


CheckerGame* newGame() {
    int i, j;
    CheckerGame* game = (CheckerGame*) malloc(sizeof(CheckerGame));

    game->turn        = CHECKER_WHITE;
    game->dirty       = TRUE;
    game->selection.x = INVALID_POSITION;
    game->selection.y = INVALID_POSITION;

    // Prepare the animation
    game->moveAnimation.running  = FALSE;
    game->moveAnimation.duration = ANIMATION_DURATION * CLOCKS_PER_SEC;

    // Init the board
    int size = BOARD_SIZE * BOARD_SIZE;
    for (i = 0; i < size; i++) {
        game->board.state._board[i] = SPACE_STATE_CLEAR;
    }

    // Position the checkers
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            game->board.spaces.board[i][j] = ((i+j) % 2 == 0) ? SPACE_BLACK : SPACE_WHITE;
            // White side
            if (i < CHECKER_ROWS && game->board.spaces.board[i][j] == SPACE_BLACK) {
                game->board.checkers.checker[i][j] = CHECKER_WHITE;
                game->checkersCountWhite++;
            }
            // Black side
            else if (i >= BOARD_SIZE-CHECKER_ROWS && game->board.spaces.board[i][j] == SPACE_BLACK) {
                game->board.checkers.checker[i][j] = CHECKER_BLACK;
                game->checkersCountBlack++;
            }
            // Empty space
            else {
                game->board.checkers.checker[i][j] = CHECKER_NONE;
            }
        }
    }

    createWindow(WINDOW_NAME, BOARD_SIZE, mouseEventListener);

    currentGame = game;

    return game;
}

void drawGame(CheckerGame* game) {
    // Draw only if necessary
    if (!(game->dirty || game->moveAnimation.running)) return;

    clearScreen();
    game->dirty = FALSE;

    int i, j;
    const Drawable tileBlack = { BLACK, DRAWABLE_TILE };
    const Drawable tileWhite = { WHITE, DRAWABLE_TILE };
    const Drawable stateSelected = { LIGHTGRAY, DRAWABLE_STATE };
    const Drawable statePossible = { LIGHTBLUE, DRAWABLE_STATE };
    const Drawable checkerBlack = { BLACK, DRAWABLE_CHECKER };
    const Drawable checkerWhite = { WHITE, DRAWABLE_CHECKER };
    const Drawable checkerCrown = { YELLOW, DRAWABLE_CHECKER_CROWN };

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            // Draw the tiles
            switch (game->board.spaces.board[i][j]) {
                case SPACE_BLACK:
                    drawRelativePosition(tileBlack, i, j);
                    break;

                case SPACE_WHITE:
                    drawRelativePosition(tileWhite, i, j);
                    break;

                default:
                    break;
            }
        }
    }

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            // Draw the states
            switch (game->board.state.board[i][j]) {
                case SPACE_STATE_CLEAR:
                    break;

                case SPACE_STATE_SELECTED:
                    drawRelativePosition(stateSelected, i, j);
                    break;

                case SPACE_STATE_POSSIBLE:
                    drawRelativePosition(statePossible, i, j);
                    break;

                default:
                    break;
            }
        }
    }


    // Draw the checkers
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            Point position;
            position.x = j;
            position.y = i;
            float iChecker;
            float jChecker;
            if (game->board.spaces.board[i][j] == SPACE_BLACK) {
                getCheckerPosition(game, position, &iChecker, &jChecker);
                switch (game->board.checkers.checker[i][j]) {
                    case CHECKER_NONE:
                        break;

                    case CHECKER_BLACK:
                        drawRelativePosition(checkerBlack, iChecker, jChecker);
                        break;

                    case CHECKER_KING_BLACK:
                        drawRelativePosition(checkerBlack, iChecker, jChecker);
                        drawRelativePosition(checkerCrown, iChecker, jChecker);
                        break;

                    case CHECKER_WHITE:
                        drawRelativePosition(checkerWhite, iChecker, jChecker);
                        break;

                    case CHECKER_KING_WHITE:
                        drawRelativePosition(checkerWhite, iChecker, jChecker);
                        drawRelativePosition(checkerCrown, iChecker, jChecker);
                        break;
                }
            }
        }
    }

    // Swap buffers!
    swapBuffers();
}

Checker checkEndGame(CheckerGame* game) {
    if (game->checkersCountWhite == 0) return CHECKER_BLACK;
    if (game->checkersCountBlack == 0) return CHECKER_WHITE;

    return CHECKER_NONE;
}

#define BITMASK_CHECKER_COLOR 0x02
Checker getCheckerColor(Checker checker) {
    return (checker & BITMASK_CHECKER_COLOR) == 0 ? CHECKER_WHITE : CHECKER_BLACK;
}
#define BITMASK_CHECKER_KING  0x04
boolean isCheckerKing(Checker checker) {
    return (checker & BITMASK_CHECKER_KING) == 0 ? FALSE : TRUE;
}

static void getCheckerPosition(CheckerGame* game, Point position, float* i, float* j) {
    if (game->moveAnimation.running &&
        game->moveAnimation.start.x == position.x &&
        game->moveAnimation.start.y == position.y) {
        // This is the checker being animated

        float animProgress = (clock() - game->moveAnimation.startTime) / game->moveAnimation.duration;

        if (animProgress >= 1) {
            moveChecker(game);

            *i = game->moveAnimation.end.y;
            *j = game->moveAnimation.end.x;
        }
        else {
            *i = game->moveAnimation.start.y + animProgress * (game->moveAnimation.end.y - game->moveAnimation.start.y);
            *j = game->moveAnimation.start.x + animProgress * (game->moveAnimation.end.x - game->moveAnimation.start.x);
        }
    }
    else {
        // No biggies
        *i = position.y;
        *j = position.x;
    }
}

static void mouseEventListener(int x, int y, MouseEvent event) {
    if (currentGame->moveAnimation.running) return;
    switch (event) {
        case MOUSE_CLICK: {
            Point p = getBoardTile(x, y);
            boolean validPosition = p.x != INVALID_POSITION && p.y != INVALID_POSITION;

            if (validPosition) {
                if (hasSelection(currentGame)) {
                    if (currentGame->board.state.board[p.y][p.x] == SPACE_STATE_POSSIBLE) {
                        // Move the checker!
                        // Clear the states and redraw
                        clearStates(currentGame);
                        currentGame->dirty;

                        // Starts the animation
                        currentGame->moveAnimation.start.x   = currentGame->selection.x;
                        currentGame->moveAnimation.start.y   = currentGame->selection.y;
                        currentGame->moveAnimation.end.x     = p.x;
                        currentGame->moveAnimation.end.y     = p.y;
                        currentGame->moveAnimation.startTime = clock();
                        currentGame->moveAnimation.running   = TRUE;
                    }
                    else {
                        // Clear the selection
                        selectSpace(currentGame, INVALID_POSITION, INVALID_POSITION);
                    }
                }
                if (currentGame->board.checkers.checker[p.y][p.x] == currentGame->turn ||
                    currentGame->board.checkers.checker[p.y][p.x] == (currentGame->turn == CHECKER_BLACK ? CHECKER_KING_BLACK : CHECKER_KING_WHITE)) {
                    // Select the checker
                    selectSpace(currentGame, p.y, p.x);
                }
            }
            else {
                // Clear selection
                selectSpace(currentGame, INVALID_POSITION, INVALID_POSITION);
            }
            break;
        }
        case MOUSE_MOVE: {
            // TODO Use hover
            break;
        }
        default:
            break;
    }
}

static boolean hasSelection(CheckerGame* game) {
    return game->selection.x != INVALID_POSITION && game->selection.y != INVALID_POSITION;
}

static void selectSpace(CheckerGame* game, int i, int j) {
    if (game->selection.y != i || game->selection.x != j) {
        // Selection was changed
        clearStates(game);
        game->selection.y = i;
        game->selection.x = j;

        if (hasSelection(game)) {
            game->board.state.board[i][j] = SPACE_STATE_SELECTED;
            calculatePossibleMoves(game);
        }

        game->dirty = TRUE;
    }
}

static void clearStates(CheckerGame* game) {
    int i;
    int size = BOARD_SIZE * BOARD_SIZE;
    for (i = 0; i < size; i++) {
        game->board.state._board[i] = SPACE_STATE_CLEAR;
    }
}

static boolean isFreeSpace(CheckerGame* game, int i, int j) {
    return i >= 0 && i < BOARD_SIZE &&
           j >= 0 && j < BOARD_SIZE &&
           game->board.checkers.checker[i][j] == CHECKER_NONE;
}

static boolean hasAdversary(CheckerGame* game, int i, int j, Checker type) {
    return i >= 0 && i < BOARD_SIZE &&
           j >= 0 && j < BOARD_SIZE &&
           (
                // White captures Black
                ((type == CHECKER_WHITE || type == CHECKER_KING_WHITE) &&
                (game->board.checkers.checker[i][j] == CHECKER_BLACK ||
                 game->board.checkers.checker[i][j] == CHECKER_KING_BLACK))
                ||
                // Black captures White
                ((type == CHECKER_BLACK || type == CHECKER_KING_BLACK) &&
                (game->board.checkers.checker[i][j] == CHECKER_WHITE ||
                 game->board.checkers.checker[i][j] == CHECKER_KING_WHITE))
            );
}

static void checkSimpleMove(CheckerGame* game, int i, int j) {
    if (isFreeSpace(game, i, j)) {
        game->moves.move[game->moves.count].x = j;
        game->moves.move[game->moves.count].y = i;
        game->moves.type[game->moves.count]   = MOVE_SIMPLE;
        game->moves.count++;
    }
}

static void checkCaptureMove(CheckerGame* game, int i2, int j2, int i1, int j1, Checker type) {
    if (isFreeSpace(game, i2, j2) && hasAdversary(game, i1, j1, type)) {
        game->moves.move[game->moves.count].x = j2;
        game->moves.move[game->moves.count].y = i2;
        game->moves.type[game->moves.count]   = MOVE_CAPTURE;
        game->moves.count++;
    }
}

static void getAdjacentFreeMoves(CheckerGame* game, Point position, Checker type) {
    if (type == CHECKER_WHITE || type == CHECKER_KING_WHITE || type == CHECKER_KING_BLACK) {
        // Look down

        // South East
        checkSimpleMove(game, position.y+1, position.x+1);
        // South West
        checkSimpleMove(game, position.y+1, position.x-1);
    }
    if (type == CHECKER_BLACK || type == CHECKER_KING_WHITE || type == CHECKER_KING_BLACK) {
        // Look up

        // North East
        checkSimpleMove(game, position.y-1, position.x+1);
        // North West
        checkSimpleMove(game, position.y-1, position.x-1);
    }
}

static void getCaptureMoves(CheckerGame* game, Point position, Checker type) {
    if (type == CHECKER_BLACK || type == CHECKER_KING_WHITE || type == CHECKER_KING_BLACK) {
        // Look up
        // North East
        checkCaptureMove(game, position.y-2, position.x+2, position.y-1, position.x+1, type);
        // North West
        checkCaptureMove(game, position.y-2, position.x-2, position.y-1, position.x-1, type);
    }
    if (type == CHECKER_WHITE || type == CHECKER_KING_WHITE || type == CHECKER_KING_BLACK) {
        // Look down
        // South East
        checkCaptureMove(game, position.y+2, position.x+2, position.y+1, position.x+1, type);
        // South West
        checkCaptureMove(game, position.y+2, position.x-2, position.y+1, position.x-1, type);
    }
}

static void calculatePossibleMoves(CheckerGame* game) {
    Checker type = game->board.checkers.checker[game->selection.y][game->selection.x];
    game->moves.count = 0;
    getAdjacentFreeMoves(game, game->selection, type);
    getCaptureMoves(game, game->selection, type);

    int i;
    for (i = 0; i < game->moves.count; i++) {
        game->board.state.board[game->moves.move[i].y][game->moves.move[i].x] = SPACE_STATE_POSSIBLE;
    }
}

static void moveChecker(CheckerGame* game) {

    // Place the checker in the ending position
    game->board.checkers.checker[currentGame->moveAnimation.end.y][currentGame->moveAnimation.end.x] = game->board.checkers.checker[currentGame->moveAnimation.start.y][currentGame->moveAnimation.start.x];
    // Remove the checker from its old position
    game->board.checkers.checker[currentGame->moveAnimation.start.y][currentGame->moveAnimation.start.x] = CHECKER_NONE;

    // Check the movement type
    if (fabs(currentGame->moveAnimation.start.x - currentGame->moveAnimation.end.x) == 2) {
        // Capture move
        int y = (currentGame->moveAnimation.start.y+currentGame->moveAnimation.end.y) / 2;
        int x = (currentGame->moveAnimation.start.x+currentGame->moveAnimation.end.x) / 2;

        switch (game->board.checkers.checker[y][x]) {
            case CHECKER_WHITE:
            case CHECKER_KING_WHITE:
                game->checkersCountWhite--;
                break;

            case CHECKER_BLACK:
            case CHECKER_KING_BLACK:
                game->checkersCountBlack--;
                break;

            default:
                break;
        }

        game->board.checkers.checker[y][x] = CHECKER_NONE;
    }

    // Check if this checker becomes King
    if (game->turn == CHECKER_BLACK && currentGame->moveAnimation.end.y == 0) {
        game->board.checkers.checker[currentGame->moveAnimation.end.y][currentGame->moveAnimation.end.x] = CHECKER_KING_BLACK;
    }
    else if (game->turn == CHECKER_WHITE && currentGame->moveAnimation.end.y == BOARD_SIZE-1) {
        game->board.checkers.checker[currentGame->moveAnimation.end.y][currentGame->moveAnimation.end.x] = CHECKER_KING_WHITE;
    }

    // Swap turns
    game->turn = (game->turn == CHECKER_BLACK) ? CHECKER_WHITE : CHECKER_BLACK;

    // Redraw
    game->dirty = TRUE;
    // Ends the animation
    game->moveAnimation.running = FALSE;
}

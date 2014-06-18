#ifndef CHECKERS_GAME_H_INCLUDED
#define CHECKERS_GAME_H_INCLUDED

#include <stdlib.h>
#include <time.h>
#include "CheckersDrawing.h"

#define TRUE  1
#define FALSE 0

#define WINDOW_NAME "Checkers"

#define BOARD_SIZE 8

#define CHECKER_ROWS 3

#define ANIMATION_DURATION .5f // in seconds


typedef char Space;
#define SPACE_WHITE             0
#define SPACE_BLACK             1
#define SPACE_STATE_CLEAR       2
#define SPACE_STATE_SELECTED    3
#define SPACE_STATE_POSSIBLE    4

typedef char Checker;
/* ****************** */
/* bitmask            */
/*  0 - Has checker   */
/*  1 - Checker color */
/*  2 - Checker king  */
/* ****************** */
#define CHECKER_NONE        0x00
#define CHECKER_WHITE       0x01
#define CHECKER_BLACK       0x03
#define CHECKER_KING        0x04
#define CHECKER_KING_WHITE  0x05
#define CHECKER_KING_BLACK  0x07

typedef union {
    Checker _checker[BOARD_SIZE*BOARD_SIZE];
    Checker checker[BOARD_SIZE][BOARD_SIZE];
} CheckerSpaces;

typedef union {
    Space _board[BOARD_SIZE*BOARD_SIZE];
    Space board[BOARD_SIZE][BOARD_SIZE];
} BoardSpaces;

typedef struct {
    BoardSpaces   spaces;
    BoardSpaces   state;
    CheckerSpaces checkers;
} Board;

typedef char MovementType;
#define MOVE_SIMPLE     0
#define MOVE_CAPTURE    1


typedef struct {
    int          count;
    Point        move[BOARD_SIZE*BOARD_SIZE/4];
    MovementType type[BOARD_SIZE*BOARD_SIZE/4];
} Movements;

typedef struct {
    boolean running;
    clock_t startTime;
    float   duration;
    Point   start;
    Point   end;
} Animation;

typedef struct {
    Board     board;
    Point     selection;
    Checker   turn;
    Movements moves;
    int       checkersCountBlack;
    int       checkersCountWhite;
    boolean   dirty;
    Animation moveAnimation;
} CheckerGame;


CheckerGame* newGame();

void drawGame(CheckerGame* game);

Checker checkEndGame(CheckerGame* game);

Checker getCheckerColor(Checker checker);
boolean isCheckerKing(Checker checker);


#endif // CHECKERS_GAME_H_INCLUDED

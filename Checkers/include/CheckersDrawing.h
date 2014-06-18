#ifndef CHECKERS_DRAWING_H_INCLUDED
#define CHECKERS_DRAWING_H_INCLUDED

#include <math.h>
#include "graphics.h"

#define INVALID_POSITION -1

#define TILE_SIZE     80
#define BOARD_PADDING 20


typedef enum {
    MOUSE_CLICK_DOWN,
    MOUSE_CLICK_UP,
    MOUSE_CLICK,
    MOUSE_MOVE
} MouseEvent;

typedef void (*MouseListener)(int, int, MouseEvent);

typedef struct {
    int x;
    int y;
} Point;

typedef enum {
    DRAWABLE_TILE,
    DRAWABLE_STATE,
    DRAWABLE_CHECKER,
    DRAWABLE_CHECKER_CROWN
} DrawableType;

typedef struct {
    int          color;
    DrawableType type;
} Drawable;


void createWindow(char* windowName, int boardSize, MouseListener l);

void drawRelativePosition(Drawable drawable, float i, float j);

void swapBuffers();
void clearScreen();

Point getBoardTile(int x, int y);


#endif // CHECKERS_DRAWING_H_INCLUDED

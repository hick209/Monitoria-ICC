#include "CheckersDrawing.h"

#define CLICK_SLOP 10


struct {
    char          clickDown;
    int           downX;
    int           downY;
    MouseListener l;
} mouseClickListener;

static void mouseDown(int x, int y) {
    mouseClickListener.downX = x;
    mouseClickListener.downY = y;
    mouseClickListener.clickDown = TRUE;
    mouseClickListener.l(x, y, MOUSE_CLICK_DOWN);
}
static void mouseUp(int x, int y) {
    mouseClickListener.l(x, y, MOUSE_CLICK_UP);
    if (mouseClickListener.clickDown && fabs(mouseClickListener.downX - x) < CLICK_SLOP && fabs(mouseClickListener.downY - y) < CLICK_SLOP) {
        mouseClickListener.l(x, y, MOUSE_CLICK);
    }
    mouseClickListener.clickDown = FALSE;
}
static void mouseMove(int x, int y) {
    mouseClickListener.l(x, y, MOUSE_MOVE);
}


static void drawAbsolutePosition(Drawable drawable, Point position);


void createWindow(char* windowName, int boardSize, MouseListener l) {
    int windowSize = 2*BOARD_PADDING + boardSize * TILE_SIZE;
    initwindow(windowSize, windowSize, windowName);
    setbkcolor(LIGHTGRAY);
    clearScreen();
    swapBuffers();

    mouseClickListener.l = l;

    // register mouse listeners
    registermousehandler(WM_LBUTTONDOWN, mouseDown);
    registermousehandler(WM_LBUTTONUP, mouseUp);
    registermousehandler(WM_MOUSEMOVE, mouseMove);
}

void drawRelativePosition(Drawable drawable, float i, float j) {
    Point position;
    position.y = i*TILE_SIZE + BOARD_PADDING;
    position.x = j*TILE_SIZE + BOARD_PADDING;

    drawAbsolutePosition(drawable, position);
}

static void drawAbsolutePosition(Drawable drawable, Point position) {
    setfillstyle(SOLID_FILL, drawable.color);

    switch (drawable.type) {
        case DRAWABLE_TILE:
            setcolor(drawable.color);
            setlinestyle(SOLID_LINE, 1, NORM_WIDTH);
            bar(position.x, position.y, position.x+TILE_SIZE, position.y+TILE_SIZE);
            break;

        case DRAWABLE_STATE:
            setcolor(drawable.color);
            setlinestyle(DASHED_LINE, 1, THICK_WIDTH);
            rectangle(position.x, position.y, position.x+TILE_SIZE, position.y+TILE_SIZE);
            break;

        case DRAWABLE_CHECKER:
            setcolor(DARKGRAY);
            setlinestyle(SOLID_LINE, 1, THICK_WIDTH);
            fillellipse(position.x + TILE_SIZE/2, position.y + TILE_SIZE/2, TILE_SIZE/3, TILE_SIZE/3);
            break;

        case DRAWABLE_CHECKER_CROWN:
            setcolor(drawable.color);
            setfillstyle(CLOSE_DOT_FILL, drawable.color);
            setlinestyle(SOLID_LINE, 1, NORM_WIDTH);
            fillellipse(position.x + TILE_SIZE/2, position.y + TILE_SIZE/2, TILE_SIZE/6, TILE_SIZE/6);
            break;
    }
}

void swapBuffers() {
    swapbuffers();
}
void clearScreen() {
    cleardevice();
}

Point getBoardTile(int x, int y) {
    Point p = { INVALID_POSITION, INVALID_POSITION };

    int boardSize = getwindowwidth()-2*BOARD_PADDING;

    x -= BOARD_PADDING;
    y -= BOARD_PADDING;

    if (x > 0 && x < boardSize) {
        p.x = x / TILE_SIZE;
    }
    if (y > 0 && y < boardSize) {
        p.y = y / TILE_SIZE;
    }

    return p;
}


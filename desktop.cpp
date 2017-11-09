#include "desktop.h"

#define GRID_VERTICAL   8 //10
#define GRID_HORIZONTAL 4 //6

#define TOP_FIELD       0.15
#define BOTTOM_FIELD    0.05
#define LEFT_FIELD      0.05
#define RIGHT_FIELD     0.05

Desktop * desktop;

Desktop::Desktop( int scr_width, int scr_height )
{
    scrWidth = scr_width;
    scrHeight = scr_height;

    dwRect.left = scrWidth * LEFT_FIELD; 
    dwRect.bottom = scrHeight * BOTTOM_FIELD;
    dwRect.right = scrWidth * ( 1 - RIGHT_FIELD );
    dwRect.top = scrHeight * ( 1 - TOP_FIELD );
    dwRect.width = dwRect.right - dwRect.left;
    dwRect.height = dwRect.top - dwRect.bottom;

    rowCount = GRID_VERTICAL;
    colCount = GRID_HORIZONTAL;
    colWidth = dwRect.width / colCount;
    rowHeight = dwRect.height / rowCount;

    printf("%s\tDesktop size %d x %d, widgets area size %d x %d\n", strNow(), scrWidth, scrHeight, dwRect.width, dwRect.height);
};

Desktop::~Desktop() { }


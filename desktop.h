#pragma once

#include <stdio.h>
#include "Timer.h"

struct Desktop
{
    int scrWidth, scrHeight;                                            // screen size
    struct { int left, top, right, bottom, width, height; } dwRect;     // widgets area
    int colCount, rowCount, colWidth, rowHeight;                        // grid

    Desktop( int scr_width, int scr_height );
    ~Desktop();
};

extern Desktop * desktop;
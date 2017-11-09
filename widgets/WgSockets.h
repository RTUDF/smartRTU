/***********************************************************************************************************

Socket  RPi      GPIO    WiringPi
        header   number  pin
        pin    
1       3        2       8
2       5        3       9
3       7        4       7
4       11       17      0

***********************************************************************************************************/

#pragma once

#include <wiringPi.h>
#include <unistd.h>

#include "IWidget.h"
#include "../timetable.h"
#include "../Timer.h"

#define SOCKETS_COUNT 4

const int socketsPins[ SOCKETS_COUNT ] = { 8, 9, 7, 0 };

class WgSockets : public IWidget
{
private:
    int id;
    bool curStates[ SOCKETS_COUNT ];
public:
    ~WgSockets();
    WgSockets();
    bool update();
    void render() {}
    bool isVisible() { return false; };
    void getRect(int & x, int & y, int & w, int & h) { x = 0; y = 0; w = 0; h = 0; }
    void setId(int id) { this->id = id; }
    int getUpdateTime() { return 1000; } //1000ms = 1 time / sec
};        
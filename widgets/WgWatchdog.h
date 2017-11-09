#pragma once

#include "../Engine.h"
#include "IWidget.h"
#include "../Timer.h"

#define WATCHDOG_FILES_NAME "./infoboard.watchdog"

class WgWatchdog : public IWidget
{
private:
    int id;
public:
    ~WgWatchdog();
    WgWatchdog();
    bool update();
    void render() {}
    bool isVisible() { return false; };
    void getRect(int & x, int & y, int & w, int & h) { x = 0; y = 0; w = 0; h = 0; }
    void setId(int id) { this->id = id; }
    int getUpdateTime() { return 10 * 60 * 1000; } //10min
};        
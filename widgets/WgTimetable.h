#pragma once

#include <sys/stat.h>
#include <sys/types.h>

#include "../Engine.h"
#include "IWidget.h"
#include "../timetable.h"
#include "../Timer.h"

class WgTimetable : public IWidget
{
private:
    int id;
	time_t fileTime;
    time_t getFileTime();
public:
    ~WgTimetable();
    WgTimetable();
    bool update();
    void render() {}
    bool isVisible() { return false; };
    void getRect(int & x, int & y, int & w, int & h) { x = 0; y = 0; w = 0; h = 0; }
    void setId(int id) { this->id = id; }
    int getUpdateTime() { return 1000; } //1000ms = 1 time / sec
};        
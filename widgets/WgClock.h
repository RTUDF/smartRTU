/*WgClock Author: Igors Kolcins
*-- This is visual widget.
*		His only update time and render it.
*		Background render parent class WgBackground
* 
*--  -- Modes: it has 3 modes
*			md1x1: show only time
*			md1x2: show time and current lecture
*			md1x3: show time, current lecture and show timer to the next lecture
*/
#pragma once

#include <iostream>
#include <fstream>
#include <cstring>

#include "WgBackground.h"
#include "../timetable.h"
#include "../desktop.h"
#include "../Timer.h"

using namespace std;
using json = nlohmann::json;

class WgClock : public WgBackground
{
private:
	int hour, min, sec; // last update time
	char strTime[10], strLect[20], strInfo[100], strTimer[15];
	
	void renderMode1();
	void renderMode2();
	void renderMode3();
	
public:
	WgClock(int AposX, int AposY, wgMode Amode);
	~WgClock();
	
	bool update();
	void render();
};


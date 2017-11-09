/*WgCalendar Author: Igors Kolcins
 *--This visual widget
 * 
 */
#pragma once
#include <ctime>
#include <iostream>
#include <string.h>
#include "WgBackground.h"
#include "../CFontStorage.h"
#include "../timetable.h"
#include "../desktop.h"
#include "../Timer.h"

class WgCalendar : public WgBackground
{
private:
	char bufDate[32], bufWeekDay[32], bufWeekInfo[50], bufWeek[32]; 
	
	const char* convertWeekDayFromInt(int wday);
	const char* convertMounthFromInt(int mon);

	void renderMode1();
	void renderMode2();
	void renderMode3();
	
public:
	WgCalendar(int AposX, int AposY, wgMode Amode);
	~WgCalendar();
	
	bool update();
	void render();
};

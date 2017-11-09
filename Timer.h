#pragma once

#include <ctime>
#include <cstdio>
#include <cstdlib>

//***********************************************************************************************
//*** Timer: getTime() returns time in ms from Timer object creation moment (period ~580 years)
//***********************************************************************************************

typedef unsigned long long int LongTimeMs;

class Timer {
private:
	timespec tmStart;
	LongTimeMs TimeToMs(const timespec aTime);
public:
	LongTimeMs getTime();
	Timer();
};

extern Timer timer;
extern const char * strNow();

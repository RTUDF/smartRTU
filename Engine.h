#pragma once 

//  basic
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include <fcntl.h>

//  our classes
#include "board.h"
#include "lib/Picture.h"
#include "Timer.h"

//  includes to kbhit function
// #include <termios.h>

#define FRAMES_PER_SECOND 25

class Engine
{
private:
	int frameCap; 
	bool isRunning;
	bool terminated;
	
	Board *board;
	IWidget *w;
	
	void run();
	void update();
	void render(bool All);
	void showFrames();
	//int kbhit(void);
	
public:
	Engine();
	~Engine();
	void start();
	void terminate() { terminated = true; }
	void forcedUpdate();
};

extern Engine * engine;
extern const char * fmt (const char * f, const char * s1, const char * s2 = NULL, const char * s3 = NULL );
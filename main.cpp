#include <cstdio>
#include <csignal>
#include "Engine.h"

//--Storages
#include "CPicturesStorage.h"
#include "CFontStorage.h"
#include "timetable.h"
#include "desktop.h"

void termSignalHandler(int signal)
{
  if (engine) engine->terminate();
}

int main()
{
	std::signal(SIGTERM, termSignalHandler);

	try {
		printf( "%s\t***** INFOBORAD engine is started\n", strNow() );

		try {
			timetable = new Timetable;
		}
		catch (...) { 
			timetable = NULL; 
		}

		int width, height;
		init(&width, &height);

		desktop = new Desktop( width, height );

		PicStorage = new CPicturesStorage();
		FontStorage = new CFontStorage();
		FontStorage->setFont((char*)"arialBold", (char*)"lib/TFont/ArialBold.ttf");

		engine = new Engine;
		engine->start();
		delete engine;

		delete FontStorage;
		delete PicStorage;

		finish();

		printf( "%s\t***** INFOBORAD engine is finished\n", strNow() );
	}
	catch (...) {
		printf( "%s\t~~~~~ INFOBORAD engine is crashed\n", strNow() );
		return 1; // need restart
	}
	return 0;
}

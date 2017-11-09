/*WgAds
 * Visual widget.
 * 	Show some ads and notifications.
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "WgBackground.h"
#include "../Engine.h"
#include "../desktop.h"
#include "../Timer.h"
#include "../CPicturesStorage.h"
#include "../CFontStorage.h"

using namespace std;

#define ADS_FILES_PATH "./widgets/text/" 
#define ADS_FILE_NAME "text.txt"
#define FLAG_FILE_NAME "flag"

class WgAds : public WgBackground
{
private:
	char *ads;
	char **adsPeace;
	int lineCount;
	time_t fileTime;
	
	int Separate( char * Src, char ** Dst = NULL );
	time_t getFileTime();
	bool renewFlag();
	bool needRenew();
	bool readFile( char * & text );

public:
	WgAds(int AposX, int AposY, wgMode Amode);
	~WgAds();
	
	bool update();
	void render();
};

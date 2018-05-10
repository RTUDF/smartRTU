#pragma once

#include "Ads.h"

#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <fstream> /// for json
#include <cstring> /// for json


//#include "WgBackground.h"
#include "../Engine.h"
#include "../desktop.h"
#include "../Timer.h"
#include "../CPicturesStorage.h"
#include "../CFontStorage.h"


#include "../lib/json.h"  /// for json

#define ADS_FILES_PATH "./resources/ads/" 
#define ADS_FILE_NAME "text.txt"
#define FLAG_FILE_NAME "flag"

using namespace std;
using json = nlohmann::json;  /// for json


class AdsText : public Ads {
private:

protected:
    bool Load(const char * jsonFile);

    /** WgAds  */
    char fileName[50] = "";
	std::string captionString;
	std::string sourceString;

	char *ads;
	char **adsPeace;
	int lineCount;
	time_t fileTime;
	json sch;  /// for json
	
	int Separate( char * Src, char ** Dst = NULL );
	time_t getFileTime();
	bool renewFlag();
	bool needRenew();
    bool readFile( char * & text );
    /** end - WgAds */

public:
    AdsText(string jsonFile, int left, int right, int top, int bottom, int width, int height);
    ~AdsText();
	std::string getCaption();
	int getRotation();
    bool update();
    void render();
}; 

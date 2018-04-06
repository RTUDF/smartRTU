/*WgAds
 * Visual widget.
 * 	Show some ads and notifications.
 */
 
 #pragma once
 
 #include <fstream>
 #include <cstring>
 #include <stdio.h>
 #include <iostream>
 #include <string>
 #include <dirent.h>
 #include <math.h>
 #include <sys/stat.h>
 #include <sys/types.h>
 
 #include "WgBackground.h"
 #include "../Engine.h"
 #include "../desktop.h"
 #include "../Timer.h"
 #include "../CPicturesStorage.h"
 #include "../CFontStorage.h"
 #include "AdsText.h"
 #include "Ads.h"
 
 
 using namespace std;
 using json = nlohmann::json;

 #define ADS_FILES_PATH "./resources/ads/" 
 #define MAX_ADS 10
 
struct AdsInfo { 
    string instance; // parameters file name (database parameter instance) 
    time_t modify; // parameters modification timestamp 
    Ads * controller; 
    AdsInfo() { instance[0] = 0; modify = 0; controller = NULL; } 
    ~AdsInfo() { if (controller) delete controller; } 
};

 class WgMulAds : public WgBackground
 { 
 public:
     WgMulAds(int AposX, int AposY, wgMode Amode);
     ~WgMulAds();
     bool update();
     void render();

private:
    int getFilesNames(const char* path, string files[100]);    // return files count.
    bool checkJsonFile(string str);
    void pageIndicator(int width, int height, int active, int circleCount, int diameter);

    int activeAds;
    int circleSize = 30;
    AdsInfo ads[MAX_ADS];
    int adsCount;
    int curAds;     // curAds = &ads[++idAd]; curAds->controller->render()      // int -> Ads* //
    //int curAds; // ads[curAds].controller->render()  curAds = curAds->next
    // Ads * defAds;
    LongTimeMs curAdsStart;
    //bool AddAds(const char * Parameters);

    //json sch;
 };
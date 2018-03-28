/*WgAds
 * Visual widget.
 * 	Show some ads and notifications.
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
 #include "AdsText.h"
 #include "Ads.h"
 
 using namespace std;
 using json = nlohmann::json;
 
 #define MAX_ADS 10
 
 struct AdsInfo { 
    char instance[100]; // parameters file name (database parameter instance) 
    time_t modify; // parameters modification timestamp 
    Ads * controller; 
    AdsInfo() { instance[0] = 0; modify = 0; controller = NULL; } 
    ~AdsInfo() { if (controller) delete controller; } 
};
 
 class WgMulAds : public WgBackground
 {
 private:
	AdsInfo ads[MAX_ADS];
    int curAds;     // curAds = &ads[++idAd]; curAds->controller->render()
	LongTimeMs curAdsStart;
 public:
    WgMulAds(int AposX, int AposY, wgMode Amode);
    ~WgMulAds();
    bool update();
    void render();
 };
 
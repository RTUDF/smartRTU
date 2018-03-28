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
 #include "Ads.h"
 
 using namespace std;
 using json = nlohmann::json;
 
 #define ADS_FILES_PATH "./resources/ads/" 
 #define MAX_ADS 100
 
 class WgMulAds : public WgBackground
 {
 private:
     Ads * ads[MAX_ADS];
     int adsCount;
     Ads * curAds;
     LongTime curAdsStart;
     bool AddAds(const char * FileName);

 public:
     WgMulAds(int AposX, int AposY, wgMode Amode);
     ~WgMulAds();
     bool update();
     void render();
 };
 
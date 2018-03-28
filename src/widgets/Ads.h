#pragma once

#include <string>
#include <fstream> /// for json
#include <cstring> /// for json
#include "../Timer.h"  // for strNow() 
#include "../lib/json.h"  /// for json
#include "../CFontStorage.h"

 #include "WgBackground.h"

using namespace std;
using json = nlohmann::json;  /// for json

#define ADS_FILES_PATH "./resources/ads/" 

/*
struct Color { // class? 
    int R, G, B, A; Color() { R = 0; G = 0; B = 0; A = 1; } 
    Color(int r, int g, int b, int a) { R = r; G = g; B = b; A = a; } 
    Color(int r, int g, int b) { R = r; G = g; B = b; A = 1; } 
    Color(const json Json); 
};

struct AdsBackground { // class? 
    enum { bgNone, bgSolid, bgImage, bgTitle } type; 
    TPicture * image; 
    Color * color; 
    AdsBackground() { type = bgNone; image = NULL; color = NULL; } 
    AdsBackground(const json Json); 
    ~AdsBackground() { if (image) delete image; if (color) delete color; } 
};
*/

struct rectClient {int left, right, top, bottom, width, height; };
const struct { unsigned int r, g, b; } Colors[] = {
	{ 255, 255, 255 },  // clWhite
	{   0, 121, 194 },  // clBlue
	{  37, 196, 166 },  // clCyan
	{ 139, 209,  93 },  // clGreen
	{   0, 185, 241 },  // clHighBlue
	{ 139,  91, 164 },  // clHighPurple
	{ 201, 208,  34 },  // clLazyYellow
	{ 240, 115,  30 },  // clOrange
	{ 183,  46, 145 },  // clPurple
	{ 255, 200,  50 },  // clYellow
	{   0,  85,  81 }   // clHaki
};

class Ads
{
protected:
    //int left, bottom, width, height; 
    char * caption; 
    int rotation; 
    time_t startTime; 
    //time_t finish; 
    //AdsBackground * background;
    //virtual bool Load(const char * Parameters);
    // bool checkFileData(); // true - file changed;
      rectClient rect;
      void setTextColor(wgColor c);
public: 
    //Ads(const json Parameters, int Left, int Bottom, int Width, int Height);
    Ads(int left, int right, int top, int bottom, int width, int height);
    ~Ads();
    //virtual bool update() = 0;
    virtual void render();
    bool isActual() { return true;  /*mock*/ }
    virtual std::string getCaption() { return "Par mums"; };
    int getRotation() { return 10;  /*mock*/ } 
    // events
    //virtual void onShow() {};
    //virtual void onHide() {};
    //virtual int getId();    //~
};

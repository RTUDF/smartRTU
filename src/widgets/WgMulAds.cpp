#include "WgMulAds.h"

WgMulAds::WgMulAds(int Ax, int Ay, wgMode Amode):	WgBackground(Ax, Ay, Amode) 
{
	for (int i = 0; i < MAX_ADS; i++) ads[i] = NULL;
	adsCount = 0;
	curAds = NULL:

	// For each file in folder ADS_FILES_PATH create ads

	curAds = NULL: // if adsCount != 0 ...

	// // заполнение массива объявлений
	// for (int i = 0; i <= /*кол-во объявлений*/; i++)
	// {
	// 	ads[i] = new AdsText(i, fileName); // или ads? или как?
	// }
	// adsCount = i;
}

WgMulAds::~WgMulAds()
{
	for (int i = 0; i < adsCount; i++)
	{
		delete ads[i];
	}
}

bool WgMulAds::AddAds(const char * FileName)
{
	// create JSON and load it from the file

	// read field type

	// check type and create one of Ads

	// add ads object to array if it's actual

}


void WgMulAds::render()
{
	//~~~ render background

	WgBackground::render();
	
	//~~~ render header

	if (curAds) {
		renderHeader( curAds->getCaption() );		// renderHeader( ads[currentAds].getHeader() ) ?

		curAds->render();
	}
	else {
		//....?
	}
}

bool WgMulAds::update()
{
	//1. chech publication time and delete nonactual ads
	//2. check show time and change curAds
	return curAds->update(); //or ads is changed
}


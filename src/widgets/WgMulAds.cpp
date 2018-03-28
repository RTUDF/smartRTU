#include "WgMulAds.h"

WgMulAds::WgMulAds(int Ax, int Ay, wgMode Amode):	WgBackground(Ax, Ay, Amode) 
{
	this->updateTime = 2000;
	
	ads[0].controller = new AdsText(rectClient.left, rectClient.right, rectClient.top,
									rectClient.bottom, rectClient.width, rectClient.height);
	ads[1].controller = new AdsText(rectClient.left, rectClient.right, rectClient.top,
									rectClient.bottom, rectClient.width, rectClient.height);
	curAds = 0;
	printf( "%s\tWgMulAds widget objects is created\n", strNow() );
}

WgMulAds::~WgMulAds()
{
	
}


void WgMulAds::render()
{
	//~~~ render background

	WgBackground::render();
	
	//~~~ render header

	if (curAds == 0)
		renderHeader("Ads Nr. 1");
	if (curAds == 1)
		renderHeader("Ads Nr. 2");

	ads[curAds].controller->render();
}

bool WgMulAds::update()
{
	//1. chech publication time and delete nonactual ads
	//2. check show time and change curAds
	if (curAds == 0)
		curAds = 1;
	else
		curAds = 0;
	
	
	this->render();

	return false;//curAds->update(); //or ads is changed
	/*moch update is false*/
}


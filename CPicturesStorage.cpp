#include "iostream"
#include "CPicturesStorage.h"

using namespace std;

CPicturesStorage * PicStorage;



CPicturesStorage::CPicturesStorage(){
	this->addPic();
	//arialBold = new TFont("lib/TFont/ArialBold.ttf");
}

CPicturesStorage::~CPicturesStorage(){
	this->cleanStorage();
}

void CPicturesStorage::addPic(){
	Logo = new Picture("resources/title.png");
	ScreenBackgroud = new Picture("resources/Background.jpg");
	Arrow = new Picture("resources/arrow_sm.png");
	WgShadows = new wgShadowPictures;
	WgShadows->t = new Picture("resources/shadows/sTop.png");
	WgShadows->lt = new Picture("resources/shadows/sLeftTop.png");
	WgShadows->rt = new Picture("resources/shadows/sRightTop.png");
	WgShadows->b = new Picture("resources/shadows/sBot.png");
	WgShadows->lb = new Picture("resources/shadows/sLeftBot.png");
	WgShadows->rb = new Picture("resources/shadows/sRightBot.png");
	WgShadows->l = new Picture("resources/shadows/sLeft.png");
	WgShadows->r = new Picture("resources/shadows/sRight.png");
	
}

void CPicturesStorage::cleanStorage(){
	delete ScreenBackgroud;
	delete WgShadows->t;
	delete WgShadows->b;
	delete WgShadows->l;
	delete WgShadows->r;
	delete WgShadows->lt;
	delete WgShadows->rb;
	delete WgShadows->lb;
	delete WgShadows->rt;
	delete WgShadows;
	delete Logo;
	delete Arrow;
}



#include "Ads.h"

Ads::Ads(int id, const char *jsonFile)
{
    this->rect.left = left;
	this->rect.right = right;
	this->rect.top = top;
	this->rect.bottom = bottom;
	this->rect.width = width;
	this->rect.height = height;

	printf( "%s\tAds interface is created\n", strNow() );
}

Ads::~Ads()
{}

void Ads::setTextColor(wgColor c){
	FontStorage->getFont((char*)"arialBold")->SetColour(
		Colors[c].r,
		Colors[c].g,
		Colors[c].b
	);
}

void Ads::render()
{
   cout << "Ads Render\n";
}

//int Ads::getId() { return id; }

//bool checkFileData() { return false; } // true - file changed; 
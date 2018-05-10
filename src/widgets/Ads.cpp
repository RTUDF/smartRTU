#include "Ads.h"

// bool Ads::Load(const char * jsonFile)
// {

// }

// Ads::Ads(const json Parameters, int Left, int Bottom, int Width, int Height)
// {
//     // this.File = jsonFile;
//     // Load(File);
//     // this.id = id;

// } 

Ads::Ads(string jsonFile, int left, int right, int top, int bottom, int width, int height)
{
	this->rect.left = left;
	this->rect.right = right;
	this->rect.top = top;
	this->rect.bottom = bottom;
	this->rect.width = width;
	this->rect.height = height;
	
	startTime = time(NULL); // start time in timestamp
	rotation = 1000;

	printf( "%s\tAds interface is created\n", strNow() );
}

Ads::~Ads()
{
	
}


std::string Ads::getCaption(){
    // try{
	// 	//string str = string(ADS_FILES_PATH) + "textJsonOne.json";

	// 	char text[50]; strcpy(text, ADS_FILES_PATH);
	// 	//** Test JSON. caption 
	// 	strcat(text,"textJsonOne.json");
	// 	ifstream i(text);
	// 	i >> sch;
	// 	std::string jString = sch["caption"];
	// 	//*
	// 	return jString;
	// }
	// catch(...){
		return "Par mums";
	//}
} 

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

/*int Ads::getId()
{
    return id;
}*/

/*bool checkFileData()
{
    return false; // true - file changed;
}*/

int Ads::getRotation() {
	return rotation;  /*mock*/ 
}
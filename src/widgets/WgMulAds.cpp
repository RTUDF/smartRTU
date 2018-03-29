#include "WgMulAds.h"

WgMulAds::WgMulAds(int Ax, int Ay, wgMode Amode): WgBackground(Ax, Ay, Amode) 
{
	//for (int i = 0; i < MAX_ADS; i++) ads[i] = NULL;
	//adsCount = 0;
	//curAds = NULL;

	// For each file in folder ADS_FILES_PATH create ads

	//curAds = NULL: // if adsCount != 0 ...

	// // заполнение массива объявлений
	// for (int i = 0; i <= /*кол-во объявлений*/; i++)
	// {
	// 	ads[i] = new AdsText(i, fileName); // или ads? или как?
	// }
	// adsCount = i;


	//  ads[0].controller = new AdsText(rectClient.left, rectClient.right, rectClient.top,
	//  								rectClient.bottom, rectClient.width, rectClient.height);
	// ads[1].controller = new AdsText(rectClient.left, rectClient.right, rectClient.top,
	// 								rectClient.bottom, rectClient.width, rectClient.height);

	updateTime = 3000;

	string files[100];
	int filesCount = 0;
	const char* path = "./resources/ads";

	adsCount = getFilesNames(path, files);

	for (int i = 0; i < adsCount; i++) cout << files[i] << endl;
	
	for (int i = 0; i < adsCount; )
	{
		char file[100] = "";
		strcat(file, path);
		strcat(file, "/");
		strcat(file, files[i].c_str());
		json sch;
		ifstream j(file);
		j >> sch;
		string type = sch["type"];
		
		if (!strcmp("text", type.c_str()))
		{
			ads[i].instance = files[i];
			ads[i].controller = new AdsText(files[i], rectClient.left, rectClient.right, rectClient.top,
	  								rectClient.bottom, rectClient.width, rectClient.height);
			i++;
		}
		// else if ... AdsImage ...
	}

	curAds = 0;
	printf( "%s\tWgMulAds widget objects is created\n", strNow() );
}

WgMulAds::~WgMulAds()
{
	//delete curAds;
	
	// for (int i = 0; i < adsCount; i++)
	// {
	// 	delete ads[i];
	// }
	//if ( curAds ) delete [] curAds;
	//printf( "%s\tWgMulAds widget objects is deleted\n", strNow() );
}

int WgMulAds::getFilesNames(const char* path, string files[100])
{
	struct dirent* ent;
	DIR* dir;
	int i = 0;
	if ((dir = opendir(path)) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (checkJsonFile(ent->d_name))
			{
				files[i] = ent->d_name;
				i++;
			}
			
		}
		closedir(dir);
	}
	return i;
}

bool WgMulAds::checkJsonFile(string str)
{
	char temp[6] = ".json";
	int i = str.length();
	int j = 5;

	for ( ; i >= 0 && j >= 0; i--, j--)
	{
		if (str[i] != temp[j])
		{
			return false;
		}
	}
	return true;
}

// bool WgMulAds::Load(const char * jsonFile)
// {
	
// 	ifstream i(jsonFile);
// 	i >> sch;
// 	if(sch) return true; // ~
// 	return false;  // ~
// }

// bool WgMulAds::AddAds(const char Parameters)  // ? *
// {
// 	// create JSON and load it from the file
// 	//Load(strcat(ADS_FILES_PATH,"test.json"));
// 	std::string jType = Parameters["type"];
	
// 	// read field type

// 	// check type and create one of Ads

// 	if (strcmp(jType,"text")){
// 		curAds = new Ads(sch, 10, 10, 10, 10); /* mock */
// 		//curAds = new AdsText();
// 	}
// }


void WgMulAds::render()
{
	//~~~ render background
	WgBackground::render();
	//~~~ render header

	// if (curAds) {
		
	 			// renderHeader( ads[currentAds].getHeader() ) ?
	// 	//curAds->render();
	// }
	// else {
	// 	renderHeader("Default");
	// }
	//renderHeader("GGGG");
	
	// try{
	// 	renderHeader(ads[0].controller->getCaption().c_str()/*curAds->getCaption()*/);
	// }
	// catch(...){
	// 	renderHeader("Default");
	// };

	cout << "current Ads: " << ads[curAds].controller->getCaption().c_str() << endl;
	if (ads[curAds].controller)
	{
		renderHeader(ads[curAds].controller->getCaption().c_str());
		ads[curAds].controller->render();
	}
}

bool WgMulAds::update()
{
	//1. chech publication time and delete nonactual ads
	//2. check show time and change curAds

	curAds++;
	if (curAds >= adsCount) curAds = 0;
	
	this->render();

	return false;//curAds->update(); //or ads is changed
	/*moch update is false*/
}


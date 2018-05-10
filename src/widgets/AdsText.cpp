#include "AdsText.h"




bool AdsText::Load(const char * jsonFile)
{ return 0;}

AdsText::AdsText(string jsonFile, int left, int right, int top, int bottom, int width, int height) : Ads(jsonFile, left, right, top, bottom, width, height)
{	
	ads = NULL;
	adsPeace = NULL;
	lineCount = 0;
	fileTime = 0;

	char texttext[100] = "";
	strcat(texttext, ADS_FILES_PATH);
	strcat(texttext, jsonFile.c_str());
	printf( "\t texttext = %s\n", texttext );
	ifstream i(texttext);
	i >> sch;
	captionString = sch["caption"];
	rotation = sch["rotation"]; 
	rotation *= 1000; // updateTime in milliseconds

	try{
		//** JSON. caption 
		printf(" TEST - fileName: %s\n", fileName);
		json &content = sch["content"];
		sourceString = content["source"];
		strcat(fileName, sourceString.c_str()); 
		printf(" TEST - fileName after: %s\n", fileName);
	}
	catch(...){
		strcat(fileName, ADS_FILE_NAME);
	}

	update();

	printf( "%s\tAdsText widget object is created\n", strNow() );
}

AdsText::~AdsText()
{
    if ( ads ) delete [] ads;
	if ( adsPeace ) delete [] adsPeace;
	printf( "%s\tAdsText widget object is deleted\n", strNow() );
}

std::string AdsText::getCaption(){
    try{
		return captionString;
	}
	catch(...){
		return "Par mums";
	}
}

bool AdsText::update()
{
    if ( readFile(ads) ) 
	{
		lineCount = Separate( ads ) + 1;
		if ( adsPeace ) delete [] adsPeace;
		adsPeace = new char* [ lineCount ];
		for (int i = 0; i < lineCount; i++) adsPeace[i] = NULL;
		Separate( ads, adsPeace );
		printf( "%s\tNew %d-lines advertisement text is loaded\n", strNow(), lineCount );
		return true;
	}
	return false;
}

void AdsText::render()
{
/*
    try{
		renderHeader(captionString.c_str());
	}
	catch(...){
		renderHeader("Par mums");
	}*/
	
	//~~~ calculate line count without last empy strings

	int lc = lineCount;
	for (int i = lc - 1; i >= 0; i--) {
		if (  !adsPeace[i] || adsPeace[i][0] == '\0' ) lc--;
		else break;
	}

	//~~~ render body if exists

	if ( lc == 0 ) return;

	TFont * font = FontStorage->getFont((char*)"arialBold");
	
	const float lineSpacing = 0.4;
	int width = (float) this->rect.width * 0.9;	
	int height = (float) this->rect.height * 0.9;

	float lh = (float) height / lc;
	float fh = lh * (1. - lineSpacing);
	font->SetSize( (int)fh );

	int tw = 0;
	for (int i = 0; i < lc; i++) 
	{
		int w = font->TextWidth( adsPeace[i] );
		if ( w > tw ) tw = w;
	}
	if ( tw > width ) {
		lh *= (float)  width / tw;
		fh = lh * (1. - lineSpacing);
	}

	int ofsY = ( this->rect.height - lh * lc - lh * lineSpacing ) / 2 - lh * 0.1;
	setTextColor(clHaki);								
	font->SetSize( fh )	;
	for (int i = 0; i < lc; i++) 
	{
		font->TextMid(
			adsPeace[i], 
			this->rect.left + this->rect.width / 2, 
			this->rect.top - ofsY - lh * (i + 1)
		);
	}
}

int AdsText::getRotation() {
	return rotation; 
}

bool AdsText::isActual() {
	// startTime ??
	time_t timeNow = time(NULL);
	int dFinish, mFinish, yFinish;
	tm *ltm = localtime(&timeNow);
	//printf("d: %i m: %i y: %i\n", ltm->tm_mday, ltm->tm_mon+1, ltm->tm_year+1900);

	try{
		//** JSON finish time 
		printf("isActual - work\n");  // message
		json &publication = sch["publication"];
		json &finish = publication["finish"];

		dFinish = finish["d"];
		mFinish = finish["m"];
		yFinish = finish["y"];
	
		if (ltm->tm_year+1900 >= yFinish){
			if (ltm->tm_mon+1 >= mFinish) {
				if (ltm->tm_mday >= dFinish) {
					return false;
				}
			}
		}
		return true;
	}
	catch(...){
		printf(">>> err isActual\n");
	}
	return false;
}

/* --------------------------------------------------------- */
/* --------------------------------------------------------- */
/* --------------------------------------------------------- */

time_t AdsText::getFileTime()
{
	struct stat buff;
	if ( stat( fmt("%s%s", ADS_FILES_PATH, fileName), &buff) == 0 ) return buff.st_mtime;
	return 0;
}

bool AdsText::renewFlag()
{
	struct dirent **namelist;
	int n = scandir( ADS_FILES_PATH, &namelist, NULL, alphasort );
	if (n < 0) perror("scandir");
	else while (n--) {
		if ( strcmp( namelist[n]->d_name, FLAG_FILE_NAME ) == 0 ) return true;
	}
	return false;
}

bool AdsText::needRenew()
{
	time_t ft = getFileTime();
	if ( ft != 0 && ft != fileTime || renewFlag() ) {
		system( fmt( "rm -rf %s%s", ADS_FILES_PATH, FLAG_FILE_NAME ) );
		fileTime = ft;
		return true;
	}		
	return false;
}

bool AdsText::readFile(char * & text)
{
	if ( needRenew() ) {
		FILE *file = fopen( fmt("%s%s", ADS_FILES_PATH, fileName), "rb" );
		fseek(file, 0, SEEK_END);
		int size = ftell(file);

		if (text) delete [] text;
		text = new char[ size + 1 ];
		
		fseek(file, 0, SEEK_SET);
		fread(text, 1, size, file);
		fclose(file);
		text[ size ] = '\0';
		
		return true;
	}
	return false;
}

int AdsText::Separate( char * Src, char ** Dst )
{
	int p = 0;
	if (Dst) Dst[p] = Src;
	for ( ; *Src; Src++) {
		if ( *Src == '\n' ) { 
			if (Dst) *Src = '\0';
			char * str = Src + 1; 
			while (*str == '\r') *str++; 
			if (Dst) Dst[++p] = str; else p++;
		}
	}
	return p;
}
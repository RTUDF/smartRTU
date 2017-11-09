#include "WgAds.h"

WgAds::WgAds(int Ax, int Ay, wgMode Amode):	WgBackground(Ax, Ay, Amode) 
{
	system( fmt( "touch %s%s", ADS_FILES_PATH, FLAG_FILE_NAME ) );
	updateTime = 1000; // 1s
	ads = NULL;
	adsPeace = NULL;
	lineCount = 0;
	fileTime = 0;

	printf( "%s\tWgAds widget object is created\n", strNow() );
}

WgAds::~WgAds()
{
	if ( ads ) delete [] ads;
	if ( adsPeace ) delete [] adsPeace;
	printf( "%s\tWgAds widget object is deleted\n", strNow() );
}

time_t WgAds::getFileTime()
{
	struct stat buff;
	if ( stat( fmt("%s%s", ADS_FILES_PATH, ADS_FILE_NAME), &buff) == 0 ) return buff.st_mtime;
	return 0;
}

bool WgAds::renewFlag()
{
	struct dirent **namelist;
	int n = scandir( ADS_FILES_PATH, &namelist, NULL, alphasort );
	if (n < 0) perror("scandir");
	else while (n--) {
		if ( strcmp( namelist[n]->d_name, FLAG_FILE_NAME ) == 0 ) return true;
	}
	return false;
}

bool WgAds::needRenew()
{
	time_t ft = getFileTime();
	if ( ft != 0 && ft != fileTime || renewFlag() ) {
		system( fmt( "rm -rf %s%s", ADS_FILES_PATH, FLAG_FILE_NAME ) );
		fileTime = ft;
		return true;
	}		
	return false;
}

bool WgAds::readFile(char * & text)
{
	if ( needRenew() ) {
		FILE *file = fopen( fmt("%s%s", ADS_FILES_PATH, ADS_FILE_NAME), "rb" );
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

int WgAds::Separate( char * Src, char ** Dst )
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

bool WgAds::update()
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

void WgAds::render()
{
	//~~~ render background

	WgBackground::render();
	
	//~~~ render header

	//renderHeader( "Sludinājums" );
	renderHeader( "Par mums" );
	
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
	int width = (float) rectClient.width * 0.9;	
	int height = (float) rectClient.height * 0.9;

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

	int ofsY = ( rectClient.height - lh * lc - lh * lineSpacing ) / 2 - lh * 0.1;
	setTextColor(clHaki);
	font->SetSize( fh )	;
	for (int i = 0; i < lc; i++) 
	{
		font->TextMid(
			adsPeace[i], 
			rectClient.left + rectClient.width / 2, 
			rectClient.top - ofsY - lh * (i + 1)
		);
	}
}
#include "WgBackground.h"
#include "../CPicturesStorage.h"
#include "../CFontStorage.h"

WgBackground::WgBackground(int AposX, int AposY, wgMode Amode)
{
	updateTime = 0; // never
	posX = AposX;   // horizontal position in the grid
	posY = AposY;   // vertical position in the grid
	mode = Amode;
	color = wgColor( 1 + rand() % 10 );
	
	isShadows = true; // shadows on
	
	// -- select widget size from mode

	switch (mode){
		case md1x1: sizeX = 1; sizeY = 1; break;
		case md1x2: sizeX = 1; sizeY = 2; break;
		case md1x3: sizeX = 1; sizeY = 3; break;
		case md3x8: sizeX = 3; sizeY = 8; break;
		default: sizeX = getWidth(); sizeY = getHeight(); 
	}

	shadowSize.left = PicStorage->WgShadows->l->getWidth();
	shadowSize.right = PicStorage->WgShadows->r->getWidth();
	shadowSize.top = PicStorage->WgShadows->t->getHeight();
	shadowSize.bottom = PicStorage->WgShadows->b->getHeight();

	rectWidget.left = desktop->dwRect.left + desktop->colWidth * posX;
	rectWidget.right = rectWidget.left + desktop->colWidth * sizeX;
	rectWidget.top  = desktop->dwRect.bottom + desktop->rowHeight * posY;
	rectWidget.bottom = rectWidget.top - desktop->rowHeight * sizeY;
	rectWidget.width = desktop->colWidth * sizeX;
	rectWidget.height = desktop->rowHeight * sizeY;

	rectHeader = rectWidget;
	rectHeader.bottom = rectWidget.top - desktop->rowHeight;
	rectHeader.height = desktop->rowHeight;

	rectClient = rectWidget;
	rectClient.top = rectWidget.top - desktop->rowHeight;
	rectClient.height = rectWidget.height - desktop->rowHeight;
}

WgBackground::~WgBackground()
{
}

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

void WgBackground::setTextColor(wgColor c){
	FontStorage->getFont((char*)"arialBold")->SetColour(
		Colors[c].r,
		Colors[c].g,
		Colors[c].b
	);
/*	
	if (c==clBlue) FontStorage->getFont((char*)"arialBold")->SetColour(0,121,194);
	else if (c==clCyan) FontStorage->getFont((char*)"arialBold")->SetColour(37,196,166);
	else if (c==clGreen) FontStorage->getFont((char*)"arialBold")->SetColour(139,209,93);
	else if (c==clHighBlue) FontStorage->getFont((char*)"arialBold")->SetColour(0,185,241);
	else if (c==clHighPurple) FontStorage->getFont((char*)"arialBold")->SetColour(139,91,164);
	else if (c==clLazyYellow) FontStorage->getFont((char*)"arialBold")->SetColour(201,208,34);
	else if (c==clOrange) FontStorage->getFont((char*)"arialBold")->SetColour(240,115,30);
	else if (c==clPurple) FontStorage->getFont((char*)"arialBold")->SetColour(183,46,145);
	else if (c==clYellow) FontStorage->getFont((char*)"arialBold")->SetColour(255,200,50);
	else if (c==clHaki) FontStorage->getFont((char*)"arialBold")->SetColour(0, 85, 81);
*/
}

void WgBackground::setFillColor(wgColor c)
{
	Fill(
		Colors[c].r,
		Colors[c].g,
		Colors[c].b,
		1
	);
/*
	if (c==clWhite) Fill(255,255,255,1);
	else if (c==clBlue) Fill(0,121,194,1);
	else if (c==clCyan) Fill(37,196,166,1);
	else if (c==clGreen) Fill(139,209,93,1);
	else if (c==clHighBlue) Fill(0,185,241,1);
	else if (c==clHighPurple) Fill(139,91,164,1);
	else if (c==clLazyYellow) Fill(201,208,34,1);
	else if (c==clOrange) Fill(240,115,30,1);
	else if (c==clPurple) Fill(183,46,145,1);
	else if (c==clYellow) Fill(255,200,50,1);	
*/
}


void WgBackground::render()
{
	// -- render widget background blocks

	setFillColor( clWhite );
	Rect( rectClient.left, rectClient.bottom, rectClient.width, rectClient.height );
	setFillColor( color );
	Rect( rectHeader.left, rectHeader.bottom, rectHeader.width, rectHeader.height );

	if (isShadows) // -- render widget shadows
	{
		// -- -- render top and bot
		for (int i=0; i<rectWidget.width; i++){
			PicStorage->WgShadows->t->render( rectWidget.left + i, rectWidget.top - shadowSize.top );
			PicStorage->WgShadows->b->render( rectWidget.left + i, rectWidget.bottom );
		}
		
		// -- -- render left and right
		for (int i=0; i<rectWidget.height; i++){
			PicStorage->WgShadows->l->render( rectWidget.left, rectWidget.top - i );
			PicStorage->WgShadows->r->render( rectWidget.right - shadowSize.right, rectWidget.top - i );
		}
	}
}

void WgBackground::getRect(int & left, int & bottom, int & width, int & height)
{
	left = rectWidget.left;
	bottom = rectWidget.bottom;
	width = rectWidget.width;
	height = rectWidget.height;
}

void WgBackground::renderHeader( const char * headerText )
{
	int maxw = rectHeader.width * 0.8;
	
	TFont * font = FontStorage->getFont((char*)"arialBold");
	font->SetColour( 255, 255, 255 );
	int fh = rectHeader.height * 0.6;
	font->SetSize( fh );
	if ( font->TextWidth( headerText ) > maxw ) {
		fh = ( fh *  maxw ) / font->TextWidth( headerText );
		font->SetSize( fh );
	}

	font->TextMid(
		headerText, 
		rectHeader.left + rectHeader.width / 2, 
		rectHeader.bottom + ( rectHeader.height - fh ) / 2
	);
}

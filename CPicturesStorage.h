/*CPicturesStorage
 * --This is a Picture Storage
 * 		You can add here pictures and
 * 		and use it anywhere in project,
 * 		just add #include "CPicturesStorage.h"
 * --Here uses library Picture.h frow Denisovs Maksims 
 */
#pragma once
#include "./lib/Picture.h"
//#include "./lib/TFont/TFont.h"

struct wgShadowPictures{
	Picture *t;
	Picture *b;
	Picture *l;
	Picture *r;
	Picture *lt;
	Picture *lb;
	Picture *rb;
	Picture *rt;
};

class CPicturesStorage
{
private:
	void addPic();
	void cleanStorage();

public:
	CPicturesStorage();
    ~CPicturesStorage();
    
    //TFont *arialBold;
    Picture *ScreenBackgroud;
    Picture *Logo;
    Picture *Arrow;
	wgShadowPictures *WgShadows;
};

extern CPicturesStorage * PicStorage;

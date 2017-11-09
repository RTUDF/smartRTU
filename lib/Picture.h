#pragma once
#include "png.h"
#include "VG/openvg.h"
#include "VG/vgu.h"
#include <jpeglib.h>
#include <stdlib.h>
#include "fontinfo.h"
#include "shapes.h"
#include <string.h> //+

//#define ONDEBUG
/*
    Authors: Maksims Denisovs, Igors Sčukins.
    RTU.
    DAUGAVPILS, LATVIA.

        Install openvg:
             sudo apt-get update && sudo apt-get upgrade
             sudo apt-get install libjpeg8-dev indent libfreetype6-dev ttf-dejavu-core
             git clone git://github.com/ajstarks/openvg
             cd openvg
             make
             sudo make install
             cd client
             make test

        Install libpng:
            sudo apt-get install libpng-dev

        Example:
            //	your pictures creation
            #include "lib/Pictures.h"

            Picture *image1 = new Picture("png.png");
            printf("width: %i, height: %i\n", image1->getWidth(), image1->getHeight());
            Picture *image2 = new Picture("rad.jpg");
            printf("width: %i, height: %i\n", image2->getWidth(), image2->getHeight());

            //	if you want to use all pixels of image
            unsigned int *pixels = new unsigned int[image2->getWidth() * image2->getHeight()];
            image2->getPixels(0, 0, image2->getWidth(), image2->getHeight(), pixels);

            //	if just some part of image 150 X 150
            // unsigned int *pixels = new unsigned int[150 * 150];
            // image2->getPixels(0, 0, 150, 150, pixels);

            //	change some pixel color
            for (int i = 0; i < image2->getWidth() * image2->getHeight(); i++)
            {
                if (pixels[i] == Picture::getColor(252, 155, 16, 255)) pixels[i] = Picture::getColor(41, 128, 21, 255);
                if (pixels[i] == Picture::getColor(255, 255, 255, 255)) pixels[i] = Picture::getColor(5, 32, 58, 255);
            }
            image2->setPixels(0, 0, image2->getWidth(), image2->getHeight(), pixels);
            delete[] pixels;


            //	drawing on the screen
            Start(width, height);
            Background(100, 100, 120);
            image2->drawImage(100, 100);
            image1->drawImage(105, 105);
            End();

            //	do not forget to destroy
            delete image1;
            delete image2;
*/

enum PictureType { picJPG, picPNG, picUnknown };

class Picture
{
private:

    int width, height;
    float scaleX, scaleY;
    float shearX, shearY;
    float rotate;
    VGImage finImg;
    
    PictureType GetPictureType(const char * Path);
    
    VGImage createImageFromPNG(const char *path);
	void createImageFromJPG(const char *path);
    VGImage createImageFromJpeg(const char *filename);

public:

    Picture(const char *path);
    ~Picture();

    void render(int x, int y);
    void render(int x, int y, float scaleX, float scaleY, float shearX, float shearY, float rotate);
//	

    int getWidth();
    int getHeight();

    void getPixels(int x, int y, int w, int h, unsigned long *pixels);
    void setPixels(int x, int y, int w, int h, unsigned long *pixels);

    void setScale(float scaleX, float scaleY);

    int getColor(float r, float g, float b, float alpha);
    int getColor(int r, int g, int b, int alpha);
    float getRed(int color);
	float getGreen(int color);
	float getBlue(int color);
	float getAlpha(int color);

};


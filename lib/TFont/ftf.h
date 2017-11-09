#pragma once
#include <vector>
#include <float.h>
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#define NGLYPHS 2000
using namespace std;

class Vector2
{
public:
	Vector2();
	Vector2(float px, float py);
	float x;
	float y;
};

Vector2	operator+(const Vector2& a, const Vector2& b);
Vector2	operator*(const Vector2& a, float b);
float convFTFixed( const FT_Pos &x );
Vector2 convFTVector( const FT_Vector &v );
bool isOn( char b );

bool LoadFTFont(const char *FileName,
		int *&Pt,
		int *&PtInd,
		unsigned char *&Ins,
		int *&InsInd,
		int *&InsCnt,
		int *&Adv,
		short Map[],
		int &Count,
		int &FontHeight,
		int &DescenderHeight);


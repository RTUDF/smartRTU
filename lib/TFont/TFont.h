/*
Writed by Georgiy Gamarnik in RTU DF.
*/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include <string.h>
#include "ftf.h"
#include "shapes.h"
#define NGLYPHS 2000

class TFont
{
		short *CharMap;
		VGPath *Glyphs;
		int GlyphsCount;
		int *GlyphAdvances;
		int FontHeight;
		int DescenderHeight;
		int Size;
		unsigned char R;
		unsigned char G;
		unsigned char B;
		VGfloat A;
		bool CreateGlyphs(int *Pnt, int *PntInd, unsigned char *Ins, int *InsInd, int *InsCnt);
		unsigned char *next_utf8_char(unsigned char *utf8, int *codepoint);
	public:
		TFont(const char *FTFileName);
		TFont(TFont &BaseFont);
		~TFont();
		void TextOut(const char *str, int x, int y);
		void TextMid(const char *str, int x, int y);
		void TextEnd(const char *str, int x, int y);
		VGfloat TextWidth(const char *);
		VGfloat TextHeight();
		VGfloat TextDepth();
		void SetSize(int localsize);
		void SetColour(unsigned char local_R, unsigned char local_G, unsigned char local_B, VGfloat local_A = 1.0);
};

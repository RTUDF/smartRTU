#include "TFont.h"
#include <iostream>
using namespace std;


bool TFont::CreateGlyphs(int *Pnt, int *PntInd, unsigned char *Ins, int *InsInd, int *InsCnt)
{
	memset(Glyphs, 0, GlyphsCount * sizeof(VGPath));
	for (int i = 0; i < GlyphsCount; i++) {
		const int *p = &Pnt[PntInd[i] * 2];
		const unsigned char *instructions = &Ins[InsInd[i]];
		int ic = InsCnt[i];
		Glyphs[i] = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_S_32,
					   1.0f / 65536.0f, 0.0f, 0, 0,
					   VG_PATH_CAPABILITY_ALL);
		if (ic) 
		{
			vgAppendPathData(Glyphs[i], ic, instructions, p);
		}
	}
	return true;
}

TFont::TFont(const char *FTFileName)
{
	int *Pnt, *PntInd, *InsInd, *InsCnt;
	unsigned char *Ins;
	CharMap = new short[NGLYPHS];
	LoadFTFont(FTFileName, Pnt, PntInd, Ins, InsInd, InsCnt, GlyphAdvances, CharMap, GlyphsCount, FontHeight, DescenderHeight);
	Glyphs = new VGPath[GlyphsCount];
	CreateGlyphs(Pnt, PntInd, Ins, InsInd, InsCnt);
	delete [] Pnt;
	delete [] PntInd;
	delete [] Ins;
	delete [] InsInd;
	delete [] InsCnt;
	Size = R = G = B = 0;
}

TFont::TFont(TFont &BaseFont)
{
	/*
	GlyphsCount = BaseFont.GlyphsCount;
	CharMap = new short int[NGLYPHS];
	Glyphs = new VGPath[GlyphsCount];
	memset(Glyphs, 0, GlyphsCount * sizeof(VGPath));
	GlyphAdvances = new int[GlyphsCount];
	for (int i = 0; i <= NGLYPHS; i++)
	{
		CharMap[i] = BaseFont.CharMap[i];
	}
	for (int i = 0; i <= BaseFont.GlyphsCount; i++)
	{
		Glyphs[i] = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_S_32,
						1.0f / 65536.0f, 0.0f, 0, 0,
						VG_PATH_CAPABILITY_ALL);
		vgAppendPath(Glyphs[i], BaseFont.Glyphs[i]);
		GlyphAdvances[i] = BaseFont.GlyphAdvances[i];
	}
	FontHeight = BaseFont.FontHeight;
	DescenderHeight = BaseFont.DescenderHeight;
	Size = BaseFont.Size;
	R = BaseFont.R;
	G = BaseFont.G;
	B = BaseFont.B;
	A = BaseFont.A;
	*/
	throw 0;
}

TFont::~TFont()
{
	delete [] CharMap;
	for (int i = 0; i <= GlyphsCount; i++) 
	{
		vgDestroyPath(Glyphs[i]);
	}
	delete [] Glyphs;
	delete [] GlyphAdvances;
}

unsigned char* TFont:: next_utf8_char(unsigned char *utf8, int *codepoint) 
{
	int seqlen;
	int datalen = (int)strlen((const char *)utf8);
	unsigned char *p = utf8;

	if (datalen < 1 || *utf8 == 0) {
		return NULL;
	}
	if (!(utf8[0] & 0x80)) {
		*codepoint = (wchar_t) utf8[0];
		seqlen = 1;
	} else if ((utf8[0] & 0xE0) == 0xC0) { 
		*codepoint = (int)(((utf8[0] & 0x1F) << 6) | (utf8[1] & 0x3F));
		seqlen = 2;
	} else if ((utf8[0] & 0xF0) == 0xE0) {
		*codepoint = (int)(((utf8[0] & 0x0F) << 12) | ((utf8[1] & 0x3F) << 6) | (utf8[2] & 0x3F));
		seqlen = 3;
	} else {
		return NULL;
	}
	p += seqlen;
	return p;
}

void TFont::TextOut(const char *str, int x, int y)
{

	vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
	Fill((int)R, (int)G, (int)B, A);
	VGfloat localsize = (VGfloat) Size, xx = x, mm[9];
	vgGetMatrix(mm);
	int character;
	unsigned char *ss = (unsigned char *)str;
	while ((ss = next_utf8_char(ss, &character)) != NULL) 
	{
		int glyph = CharMap[character];
		if (glyph == -1) {
			continue;
		}
		VGfloat mat[9] = {
			localsize, 0.0f, 0.0f,
			0.0f, localsize, 0.0f,
			xx, (VGfloat)y, 1.0f
		};
		vgLoadMatrix(mm);
		vgMultMatrix(mat);
		vgDrawPath(Glyphs[glyph], VG_FILL_PATH);
		xx += localsize * GlyphAdvances[glyph] / 65536.0f;
	}
	vgLoadMatrix(mm);
}

VGfloat TFont::TextWidth(const char *str) 
{
	VGfloat tw = 0.0;
	VGfloat localsize = (VGfloat) Size;
	int character;
	unsigned char *ss = (unsigned char *)str;
	while ((ss = next_utf8_char(ss, &character)) != NULL) {
		int glyph = CharMap[character];
		if (glyph == -1) {
			continue;
		}
		tw += localsize * GlyphAdvances[glyph] / 65536.0f;
	}
	return tw;
}

VGfloat TFont::TextHeight()
{
	return (FontHeight * Size) / 65536;
}

VGfloat TFont::TextDepth()
{
	return (-DescenderHeight * Size) / 65536;
}

void  TFont::SetSize(int localsize)
{
	Size = localsize;
}

void TFont::SetColour(unsigned char local_R, unsigned char local_G, unsigned char local_B, VGfloat local_A)
{
	R = local_R;
	G = local_G;
	B = local_B;
	A = local_A;
}

void TFont::TextMid(const char *str, int x, int y) 
{
	VGfloat tw = TextWidth(str);
	TextOut(str, x - (tw / 2), y);
}

void TFont::TextEnd(const char *str, int x, int y)
{
	VGfloat tw = TextWidth(str);
	TextOut(str, x - tw, y);
}
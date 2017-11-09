/* Based on font2openvg. See http://developer.hybrid.fi for more information. */
#include "ftf.h"
using namespace std;

	Vector2::Vector2() {};
	Vector2::Vector2(float px, float py) { x = px; y = py; }
	Vector2	operator+(const Vector2& a, const Vector2& b)	{ return Vector2(a.x+b.x, a.y+b.y); }
	Vector2	operator*(const Vector2& a, float b)	{ return Vector2(a.x*b, a.y*b); }

	float convFTFixed( const FT_Pos &x )
	{
		return (float)x / 4096.0f;
	}

	Vector2 convFTVector( const FT_Vector &v )
	{
		return Vector2(convFTFixed(v.x),convFTFixed(v.y));
	}

	bool isOn( char b )
	{
		return b & 1 ? true : false;
	}

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
		int &DescenderHeight)
	{
		FT_Library library;
		FT_Face face;
		Pt = PtInd = InsInd = InsCnt = Adv = NULL;
		Ins = NULL;
		Count = FontHeight = DescenderHeight = 0;

		if( FT_Init_FreeType( &library ) )
		{
			return false;
		}
		int faceIndex = 0;
		if( FT_New_Face( library, FileName, faceIndex, &face ) )
		{
			return false;
		}

		FT_Set_Char_Size(
            		face,    /* handle to face object           */
            		0,       /* char_width in 1/64th of points  */
            		64*64,   /* char_height in 1/64th of points */
            		96,     /* horizontal device resolution    */
            		96 );   /* vertical device resolution      */

		std::vector<int>		gpvecindices;
		std::vector<int>		givecindices;
		std::vector<int>		gpvecsizes;
		std::vector<int>		givecsizes;
		std::vector<Vector2>	gpvec;
		std::vector<char>		givec;
		std::vector<float>		gbbox;
		std::vector<float>		advances;

		float global_miny = 1000000.0f;
		float global_maxy = -10000000.0f;

		int glyphs = 0;
		for(int cc=0;cc<NGLYPHS;cc++)
		{
			Map[cc] = -1;
			if( cc < 32 )
				continue;
			int glyphIndex = FT_Get_Char_Index( face, cc );

			if( !FT_Load_Glyph( face, glyphIndex, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING | FT_LOAD_IGNORE_TRANSFORM ) )
			{
				float advance = convFTFixed( face->glyph->advance.x );
				if( cc == ' ' )
				{	//space doesn't contain any data
					gpvecindices.push_back( gpvec.size() );
					givecindices.push_back( givec.size() );

					gpvecsizes.push_back( 0 );
					givecsizes.push_back( 0 );

					gbbox.push_back(0);
					gbbox.push_back(0);
					gbbox.push_back(0);
					gbbox.push_back(0);

					advances.push_back(advance);

					//write glyph index to character map
					Map[cc] = glyphs++;
					continue;
				}

				FT_Outline &outline = face->glyph->outline;
				std::vector<Vector2>		pvec;
				std::vector<unsigned char>	ivec;
				float minx = 10000000.0f,miny = 100000000.0f,maxx = -10000000.0f,maxy = -10000000.0f;
				int s = 0,e;
				bool on;
				Vector2 last,v,nv;
				
				for(int con=0;con<outline.n_contours;++con)
				{
					int pnts = 1;
					e = outline.contours[con]+1;
					last = convFTVector(outline.points[s]);

					//read the contour start point
					ivec.push_back(2);
					pvec.push_back(last);

					int i=s+1;
					while(i<=e)
					{
						int c = (i == e) ? s : i;
						int n = (i == e-1) ? s : (i+1);
						v = convFTVector(outline.points[c]);
						on = isOn( outline.tags[c] );
						if( on )
						{	//line
							++i;
							ivec.push_back(4);
							pvec.push_back(v);
							pnts += 1;
						}
						else
						{	//spline
							if( isOn( outline.tags[n] ) )
							{	//next on
								nv = convFTVector( outline.points[n] );
								i += 2;
							}
							else
							{	//next off, use middle point
								nv = (v + convFTVector( outline.points[n] )) * 0.5f;
								++i;
							}
							ivec.push_back(10);
							pvec.push_back(v);
							pvec.push_back(nv);
							pnts += 2;
						}
						last = nv;
					}
					ivec.push_back(0);
					s = e;
				}

				for(int i=0;i<pvec.size();++i)
				{
					if( pvec[i].x < minx ) minx = pvec[i].x;
					if( pvec[i].x > maxx ) maxx = pvec[i].x;
					if( pvec[i].y < miny ) miny = pvec[i].y;
					if( pvec[i].y > maxy ) maxy = pvec[i].y;
				}
				if(!pvec.size())
				{
					minx = 0.0f;
					miny = 0.0f;
					maxx = 0.0f;
					maxy = 0.0f;
				}

				gpvecindices.push_back( gpvec.size() );
				givecindices.push_back( givec.size() );

				gpvecsizes.push_back( pvec.size() );
				givecsizes.push_back( ivec.size() );

				gbbox.push_back( minx );
				gbbox.push_back( miny );
				gbbox.push_back( maxx );
				gbbox.push_back( maxy );
				advances.push_back(advance);

				if (miny < global_miny) 
				{
					global_miny = miny;
                    		}
                    		if (maxy > global_maxy) 
				{
                        		global_maxy = maxy;
                    		}
                        
				int size;
				size = gpvec.size();
				gpvec.resize( size + pvec.size() );
				memcpy( &(gpvec[size]), &(pvec[0]), pvec.size() * sizeof(Vector2) );

				size = givec.size();
				givec.resize( size + ivec.size() );
				memcpy( &(givec[size]), &(ivec[0]), ivec.size() * sizeof(char) );

				//write glyph index to character map
				Map[cc] = glyphs++;
			}
		}
		if(!glyphs)
			return false;
		
		//instructions
		int givec_size = givec.size();
		Ins = new unsigned char[givec_size];
		for(int i = 0;i < givec_size;i++)
		{
			Ins[i] = givec[i];
		}

		int givecindices_size = givecindices.size();
		InsInd = new int[givecindices_size];
		for(int i = 0; i < givecindices_size; i++)
		{
			InsInd[i] = givecindices[i];
		}

		int givecsizes_size = givecsizes.size();
		InsCnt = new int[givecsizes_size];
		for(int i = 0; i < givecsizes_size; i++)
		{
			InsCnt[i] = givecsizes[i];
		}

		int gpvecindices_size = gpvecindices.size();
		PtInd = new int[gpvecindices_size];
		for(int i = 0 ;i < gpvecindices_size; i++)
		{
			PtInd[i] = gpvecindices[i];
		}

		int gpvec_size = gpvec.size();
		Pt = new int[gpvec_size * 2];
		for(int i = 0; i < gpvec_size; i++)
		{
			Pt[i*2] = 65536.0f*gpvec[i].x;
			Pt[i*2+1] = 65536.0f*gpvec[i].y;
		}

		int advances_size = advances.size();
		Adv = new int[advances_size];
		for(int i = 0; i < advances_size; i++)
		{
			Adv[i] = advances[i] * 65536.0f;
		}

		DescenderHeight = 65536.0f * global_miny;
		FontHeight = 65536.0f * global_maxy;

		Count = glyphs;

		
		FT_Done_Face( face );
		FT_Done_FreeType( library );
		return true;
	}

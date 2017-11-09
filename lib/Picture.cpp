#include "Picture.h"
#define MAX_SIGNATURE_LENGTH 10
//#include <string.h> //FIX

//#define BREV(b) ((b) >> 24 | (b) << 24 | ((b) & 0x00FF0000) >> 8 | ((b) & 0x0000FF00 ) << 8)

VGImage Picture::createImageFromPNG(const char *path)
{
    unsigned char header[8];

    FILE *file = fopen(path, "rb");
    if (!file)
    {
        #ifdef ONDEBUG
            printf("ONDEBUG: no file\n");
        #endif
        return VG_INVALID_HANDLE;
    }
    fread(header, 1, 8, file);
    bool is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png)
    {
        #ifdef ONDEBUG
            printf("ONDEBUG: it's not png file\n");
        #endif
        return VG_INVALID_HANDLE;
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        #ifdef ONDEBUG
             printf("ONDEBUG: ptr_error\n");
        #endif
        return VG_INVALID_HANDLE;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        #ifdef ONDEBUG
              printf("ONDEBUG: info_ptr error\n");
        #endif
        fclose(file);
        return VG_INVALID_HANDLE;
    }
    
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return VG_INVALID_HANDLE;
	}
    
    
    
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        #ifdef ONDEBUG
              printf("ONDEBUG: Error during init_io\n");
        #endif
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(file);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(file);
        return VG_INVALID_HANDLE;
    }

    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 8);	
	png_read_info(png_ptr, info_ptr);

    png_uint_32 width1 = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height1 = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
	int filter_method = png_get_filter_type(png_ptr, info_ptr);
	int compression_type = png_get_compression_type(png_ptr, info_ptr);
	int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width1, &height1, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	width = (int)width1;
	height = (int)height1;
	
	
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY &&
		bit_depth < 8) 
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr,
		PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	if (bit_depth == 16)
		png_set_strip_16(png_ptr);
	if (bit_depth < 8)
		png_set_packing(png_ptr);
	png_color_8p sig_bit;
	if (png_get_sBIT(png_ptr, info_ptr, &sig_bit))
		png_set_shift(png_ptr, sig_bit);
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);
	if (bit_depth == 16)
		png_set_swap(png_ptr);
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		png_set_swap_alpha(png_ptr);	
	
    png_bytep *row_pointers = new png_bytep[height];
    
    
    unsigned long *pixels = new unsigned long[width * height];

    for (int y = 0; y < height; y++)
        row_pointers[height - y - 1] = (png_byte*)&pixels[y * width];
        

    png_read_image(png_ptr, row_pointers);

    delete[] row_pointers;
    
    png_read_end(png_ptr, end_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			pixels[x + y * width] = getColor(getRed(pixels[x + y * width]),
				getGreen(pixels[x + y * width]), getBlue(pixels[x + y * width]), getAlpha(pixels[x + y * width]));
		}
	}
	
    VGImage img;
    img = vgCreateImage(VG_sRGBA_8888, width, height, VG_IMAGE_QUALITY_BETTER);
    vgImageSubData(img, pixels, width * 4, VG_sRGBA_8888, 0, 0, width, height);
    delete[] pixels;
    fclose(file);
    return img;
}

float Picture::getBlue(int color) { return (0xff & (color >> 24)) / 255.0; } // vozvrat ot 0.0 do 1.0
float Picture::getGreen(int color) { return (0xff & (color >> 16)) / 255.0; }
float Picture::getRed(int color) { return (0xff & (color >> 8)) / 255.0; }
float Picture::getAlpha(int color) { return (0xff & color) / 255.0; }

int Picture::getColor(float r, float g, float b, float alpha) // vozvrat v 15-4noj sisteme
{
	return ((int)((float)r * 255.0 + 0.5) << 24 |
			(int)((float)g * 255.0 + 0.5) << 16 |
			(int)((float)b * 255.0 + 0.5) << 8 |
			(int)((float)alpha * 255.0 + 0.5));
}


void Picture::createImageFromJPG(const char *path)
{
    finImg = createImageFromJpeg(path);
    width = vgGetParameteri(finImg, VG_IMAGE_WIDTH);
    height = vgGetParameteri(finImg, VG_IMAGE_HEIGHT);
}

// createImageFromJpeg decompresses a JPEG image to the standard image format
// source: https://github.com/ileben/ShivaVG/blob/master/examples/test_image.c
VGImage Picture::createImageFromJpeg(const char *filename) {
    FILE *infile;
    struct jpeg_decompress_struct jdc;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY buffer;
    unsigned int bstride;
    unsigned int bbpp;

    VGImage img;
    VGubyte *data;
    unsigned int width;
    unsigned int height;
    unsigned int dstride;
    unsigned int dbpp;

    VGubyte *brow;
    VGubyte *drow;
    unsigned int x;
    unsigned int lilEndianTest = 1;
    VGImageFormat rgbaFormat;

    // Check for endianness
    if (((unsigned char *)&lilEndianTest)[0] == 1)
        rgbaFormat = VG_sABGR_8888;
    else rgbaFormat = VG_sRGBA_8888;

    // Try to open image file
    infile = fopen(filename, "rb");
    if (infile == NULL) {
        printf("Failed opening '%s' for reading!\n", filename);
        return VG_INVALID_HANDLE;
    }
    // Setup default error handling
    jdc.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&jdc);

    // Set input file
    jpeg_stdio_src(&jdc, infile);

    // Read header and start
    jpeg_read_header(&jdc, TRUE);
    jpeg_start_decompress(&jdc);
    width = jdc.output_width;
    height = jdc.output_height;

    // Allocate buffer using jpeg allocator
    bbpp = jdc.output_components;
    bstride = width * bbpp;
    buffer = (*jdc.mem->alloc_sarray)
        ((j_common_ptr) & jdc, JPOOL_IMAGE, bstride, 1);

    // Allocate image data buffer
    dbpp = 4;
    dstride = width * dbpp;
    data = (VGubyte *) malloc(dstride * height);

    // Iterate until all scanlines processed
    while (jdc.output_scanline < height) {

        // Read scanline into buffer
        jpeg_read_scanlines(&jdc, buffer, 1);
        drow = data + (height - jdc.output_scanline) * dstride;
        brow = buffer[0];
        // Expand to RGBA
        for (x = 0; x < width; ++x, drow += dbpp, brow += bbpp) {
            switch (bbpp) {
            case 4:
                drow[0] = brow[0];
                drow[1] = brow[1];
                drow[2] = brow[2];
                drow[3] = brow[3];
                break;
            case 3:
                drow[0] = brow[0];
                drow[1] = brow[1];
                drow[2] = brow[2];
                drow[3] = 255;
                break;
            }
        }
    }

    // Create VG image
    img = vgCreateImage(rgbaFormat, width, height, VG_IMAGE_QUALITY_BETTER);
    vgImageSubData(img, data, dstride, rgbaFormat, 0, 0, width, height);

    // Cleanup
    jpeg_destroy_decompress(&jdc);
    fclose(infile);
    free(data);

    return img;
}

void Picture::render(int x, int y)
{
    if (finImg != VG_INVALID_HANDLE)
    {
        vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);

		vgTranslate(x, y);
		vgTranslate(this->width/2, this->height/2);
		vgRotate(this->rotate);
		vgTranslate(-this->width/2, -this->height/2);
		vgShear(this->shearX, this->shearY);
        vgScale(this->scaleX, this->scaleY);
        vgDrawImage(finImg);
        vgScale(1 / this->scaleX, 1 / this->scaleY);
		vgShear(-shearX, -shearY);
		vgTranslate(this->width/2, this->height/2);
		vgRotate(-this->rotate);
		vgTranslate(-this->width/2, -this->height/2);
		vgTranslate(-x, -y);
    }
    else
    {
        #ifdef ONDEBUG
            printf("ONDEBUG: can't draw\n");
        #endif
    }
}

void Picture::render(int x, int y, float scaleX, float scaleY, float shearX, float shearY, float rotate)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    this->shearX = shearX;
    this->shearY = shearY;
    this->rotate = rotate;

    if (finImg != VG_INVALID_HANDLE)
    {
		vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);

        float w = this->width * this->scaleX;
        float h = this->height * this->scaleY;
        
        vgTranslate(x, y);
		vgTranslate(w/2, h/2);
		vgRotate(this->rotate);
		vgTranslate(-w/2, -h/2);
		vgShear(this->shearX, this->shearY);
        vgScale(this->scaleX, this->scaleY);
        vgDrawImage(finImg);
        vgScale(1 / this->scaleX, 1 / this->scaleY);
		vgShear(-shearX, -shearY);
		vgTranslate(w/2, h/2);
		vgRotate(-this->rotate);
		vgTranslate(-w/2, -h/2);
		vgTranslate(-x, -y);



    }
    else
    {
        #ifdef ONDEBUG
            printf("ONDEBUG: can't draw\n");
        #endif
    }
}




PictureType Picture::GetPictureType(const char * Path)
{
    const char * FileSignatures[] = {
        "\xFF\xD8\xFF", 				// JPG signature
        "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A",		// PNG signature
        "" 						// end of list
    };
    FILE * f = fopen(Path, "rb");
    if (!f) return picUnknown;
    char sign[MAX_SIGNATURE_LENGTH] = { 0 };
    fread(sign, 1, MAX_SIGNATURE_LENGTH, f);
    fclose(f);
    for (int i = 0; FileSignatures[i][0]; i++)
    {
        if (memcmp(sign, FileSignatures[i], strlen(FileSignatures[i])) == 0) 
			return PictureType(i);
    }
    return picUnknown;
}


Picture::Picture(const char *path)
{
    width = 0;
    height = 0;
    scaleX = 1;
    scaleY = 1;
    shearX = 0;
    shearY = 0;
    rotate = 0;
    finImg = VG_INVALID_HANDLE;

    switch(GetPictureType(path))
    {
    case picJPG:
    {
        createImageFromJPG(path);

        #ifdef ONDEBUG
            printf("File contains picture in JPG format\n");
        #endif
        break;
    }
    case picPNG:
    {
        finImg = createImageFromPNG(path);

        #ifdef ONDEBUG
            printf("File contains picture in PNG format\n");
        #endif
        break;
    }
    default: {
        #ifdef ONDEBUG
             printf("File does not contain picture or file read error\n");
        #endif
    }
    }
}


Picture::~Picture()
{
        if (finImg != VG_INVALID_HANDLE) vgDestroyImage(finImg);
}

//Getters
int Picture::getWidth()
{
    return width;
}
int Picture::getHeight()
{
    return height;
}


void Picture::getPixels(int x, int y, int w, int h, unsigned long *pixels)
{

    if (finImg != VG_INVALID_HANDLE)
    {
        if (x + w > width || x < 0)
        {
            #ifdef ONDEBUG
                printf("ONDEBUG: out of borders of image (width, get) : w: %i, h: %i \n", x + w, y + h);
            #endif
            return;
        }
        if (y + h > height || y < 0)
        {
            #ifdef ONDEBUG
                printf("ONDEBUG: out of borders of image (height, get) : w: %i, h: %i \n", x + w, y + h);
            #endif
            return;
        }

        unsigned long *pix = new unsigned long[width * height];
        vgGetImageSubData(finImg, pix, width * 4, VG_lRGBA_8888, 0, 0, width, height);

        for (int i = x, a = 0; i < x + w, a < w; i++, a++)
        {
            for (int j = y, b = 0; j < y + h, b < h; j++, b++)
            {
                pixels[a + b * w] = pix[i + j * width];
            }
        }
        delete[] pix;
    }
}


void Picture::setPixels(int x, int y, int w, int h, unsigned long *pixels)
{
    if (finImg != VG_INVALID_HANDLE)
    {
        if (x + w > width || x < 0)
        {
            #ifdef ONDEBUG
                printf("ONDEBUG: out of borders of image (width, set)\n");
            #endif
            return;
        }
        if (y + h > height || y < 0)
        {
            #ifdef ONDEBUG
                printf("ONDEBUG: out of borders of image (height, set)\n");
            #endif
            return;
        }


        unsigned long *pix = new unsigned long[width * height];
        vgGetImageSubData(finImg, pix, width * 4, VG_lRGBA_8888, 0, 0, width, height);

        for (int i = x, a = 0; i < x + w, a < w; i++, a++)
        {
            for (int j = y, b = 0; j < y + h, b < h; j++, b++)
            {
                pix[i + j * width] = pixels[a + b * w];
            }
        }


        vgImageSubData(finImg, pix, width * 4, VG_lRGBA_8888, 0, 0, width, height);
        delete[] pix;
    }
}

int Picture::getColor(int r, int g, int b, int alpha)
{
    return (r << 24 | g << 16 | b << 8 | alpha);
}



void Picture::setScale(float scaleX, float scaleY)
{
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

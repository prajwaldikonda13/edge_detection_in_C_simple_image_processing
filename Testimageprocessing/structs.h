typedef struct tagBitmapHeader
{
	unsigned char signature[2];
	unsigned char size[4];
	unsigned char reserved[2];
	unsigned char reserved1[2];
	unsigned char startOffset[4];
	unsigned char sizeOfBitmapHeaderStructure[4];
	unsigned char width[4];
	unsigned char height[4];
	unsigned char noOfPlanes[2];
	unsigned char bitsPerPixel[2];
	unsigned char compressionType[4];
	unsigned char sizeOfImageData[4];
	unsigned char horizontalResolution[4];
	unsigned char verticalResolution[4];
	unsigned char numberOfColorsInImage[4];
	unsigned char numberOfImportantColorsInImage[4];
}BITMAPHEADER;


typedef struct tagRgb
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}RGB;
typedef struct tagPoint
{
	unsigned x;
	unsigned y;
	unsigned label;
	RGB rgb;
}POINT1;
typedef struct tagImageData
{
	unsigned int height;
	unsigned int width;
	unsigned int noOfPixels;
	unsigned int sizeInBytes;
	unsigned short bitsPerPixel;
	POINT1 *pixels;
}IMAGE;

typedef struct tagMatrix
{
	unsigned width;
	unsigned height;
	unsigned char *p;
}MATRIX;

typedef struct tagHistogram
{
	unsigned int frequency[256]={ 0 };
}HISTOGRAM;

typedef struct tagPMF
{
	double value[256] = { 0.0 };
}PMF;
typedef struct tagCMF
{
	double value[256] = { 0.0 };
}CMF;
typedef struct tagMASK
{
	unsigned char height;
	unsigned char width;
	float *values;
}MASK;
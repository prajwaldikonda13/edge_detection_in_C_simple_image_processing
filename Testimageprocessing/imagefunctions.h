#define PREWITT_OPERATOR 0X0
#define SOBEL_OPERATOR 0X1
#define LAPLACIAN_OF_GAUSSIAN_OPERATOR 0X2

#define NORTH_DIRECTION 0X101
#define NORTH_WEST_DIRECTION 0X102
#define WEST_DIRECTION 0X103
#define SOUTH_WEST_DIRECTION 0X104
#define SOUTH_DIRECTION 0X105
#define SOUTH_EAST_DIRECTION 0X106
#define EAST_DIRECTION 0X107
#define NORTH_EAST_DIRECTION 0X108

#define POSITIVE_LAPLACIAN 0X201
#define NEGATIVE_LAPLACIAN 0X202
MASK getLaplacianMask(unsigned);
MASK getKirschCompassMask(unsigned);
MASK getRobinsonCompassMask(unsigned);
MASK getHorizontalSobelMask();
MASK getVerticalSobelMask();
int detectEdges(IMAGEPTR, unsigned);
int displayScaledImage(IMAGEPTR, char, HDC);
int getVerticalPrewittMask(MASK*);
int getHorizontalPrewittMask(MASK);
int blurImageByMeanFilter(IMAGEPTR,unsigned);
int applyMaskOnImage(IMAGEPTR, MASK);
int flipMaskHorizontal(MASK *m);
int flipMaskVertical(MASK *m);
int compareRGBStructs(RGB*, RGB*);
int getGrayScalePMF(IMAGEPTR grayscaleImageData, PMF *arr);
int getGrayScaleHistogram(IMAGEPTR, HISTOGRAM*);
int histogramSlideLeft(IMAGEPTR, unsigned);
int histogramSlideRight(IMAGEPTR, unsigned);
int displayImage(IMAGEPTR);
int addLabels(IMAGEPTR buffer);
IMAGE zoomByPixelReplication(IMAGEPTR);
IMAGE zoomByKTimes(IMAGEPTR,int);
IMAGE zoomByZeroOrderHold(IMAGEPTR);
int getImage(IMAGEPTR, const char*);
int getGrayScaleImage(IMAGEPTR, const char*);
int getImage(IMAGEPTR buffer, const char *FilePath)
{
	int x, y;
	FILEPTR fp;
	BITMAPHEADER BitmapHeader;
	fp = fopen(FilePath, "rb");
	fread(&BitmapHeader, sizeof(BITMAPHEADER), 1, fp);
	buffer->height = *(int*)BitmapHeader.height;
	buffer->width = *(int*)BitmapHeader.width;
	buffer->noOfPixels = buffer->height*buffer->width;
	buffer->bitsPerPixel = *(short*)BitmapHeader.bitsPerPixel;
	buffer->sizeInBytes = buffer->noOfPixels*(buffer->bitsPerPixel / 8);
	RGB *rgbHead = (RGB*)malloc(buffer->noOfPixels * sizeof(RGB));
	buffer->pixels = (POINT1*)malloc(buffer->noOfPixels * sizeof(POINT1));
	fread(rgbHead, buffer->sizeInBytes, 1, fp);
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width; x++)
		{
			buffer->pixels[x + y * buffer->width].x = x;
			buffer->pixels[x + y * buffer->width].y = y;
			buffer->pixels[x + y * buffer->width].rgb = rgbHead[x + (buffer->height - 1)*buffer->width - y * buffer->width];
			buffer->pixels[x + y * buffer->width].label = 0;
			//SetPixel(hdc, x,y, RGB(rgbHead[x + (image.height - 1)*image.width - y * image.width].r, rgbHead[x +(image.height-1)*image.width -y * image.width].g, rgbHead[x + (image.height - 1)*image.width - y * image.width].b));
		}
	}
	return 0;
}





int getGrayScaleImage(IMAGEPTR buffer, const char *FilePath)
{
	unsigned x, y;
	unsigned grayVal;
	FILEPTR fp;
	RGB tempRgb;
	BITMAPHEADER BitmapHeader;
	fp = fopen(FilePath, "rb");
	fread(&BitmapHeader, sizeof(BITMAPHEADER), 1, fp);
	buffer->height = *(int*)BitmapHeader.height;
	buffer->width = *(int*)BitmapHeader.width;
	buffer->noOfPixels = buffer->height*buffer->width;
	buffer->bitsPerPixel = *(short*)BitmapHeader.bitsPerPixel;
	buffer->sizeInBytes = buffer->noOfPixels*(buffer->bitsPerPixel / 8);
	RGB *rgbHead = (RGB*)malloc(buffer->noOfPixels * sizeof(RGB));
	buffer->pixels = (POINT1*)malloc(buffer->noOfPixels * sizeof(POINT1));
	fread(rgbHead, buffer->sizeInBytes, 1, fp);
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width; x++)
		{
			buffer->pixels[x + y * buffer->width].x = x;
			buffer->pixels[x + y * buffer->width].y = y;
			tempRgb = rgbHead[x + (buffer->height - 1)*buffer->width - y * buffer->width];
			grayVal = tempRgb.r*0.3 + tempRgb.g*0.59 + tempRgb.b*0.11;
			tempRgb.r = grayVal;
			tempRgb.g = grayVal;
			tempRgb.b = grayVal;
			buffer->pixels[x + y * buffer->width].rgb = tempRgb;
			buffer->pixels[x + y * buffer->width].label = 0;
		}
	}
	return 0;
}
int createBitmap()
{
	FILEPTR fp;
	BITMAPHEADER header;
	fp = fopen("c:\\users\\winner\\desktop\\testbmp.bmp", "wb");
	*(short*)header.signature = 0x4d42;
	*(unsigned*)header.size = 55;
	*(short*)header.reserved = 0;
	*(short*)header.reserved1 = 0;
	*(unsigned*)header.startOffset = 54;
	*(unsigned*)header.sizeOfBitmapHeaderStructure = 40;
	*(unsigned*)header.width = 1;
	*(unsigned*)header.height = 1;
	*(short*)header.noOfPlanes = 1;
	*(short*)header.bitsPerPixel = 8;
	*(unsigned*)header.compressionType = 0;
	*(unsigned*)header.sizeOfImageData = 0;
	*(unsigned*)header.horizontalResolution = 1;
	*(unsigned*)header.verticalResolution = 1;
	*(unsigned*)header.numberOfColorsInImage = 0;
	*(unsigned*)header.numberOfImportantColorsInImage = 0;
	fwrite(&header, sizeof(header), 1, fp);


	unsigned char arr = 0xff;
	fwrite(&arr, sizeof(arr), 1, fp);
	fclose(fp);
	return 0;
}





IMAGE zoomByPixelReplication(IMAGEPTR buffer)
{
	/*
	Method 1: Pixel replication:
	Introduction:
	It is also known as Nearest neighbor interpolation. As its name suggest, in this method, we just replicate the neighboring pixels. As we have already discussed in the tutorial of Sampling, that zooming is nothing but increase amount of sample or pixels. This algorithm works on the same principle.

	Working:
	In this method we create new pixels form the already given pixels. Each pixel is replicated in this method n times row wise and column wise and you got a zoomed image. Its as simple as that.
	*/
	unsigned x, y;
	IMAGE zoomedImage;
	POINT1 currPixel;
	zoomedImage.height = buffer->height <<1;
	zoomedImage.width = buffer->width <<1;
	zoomedImage.pixels = (POINT1*)malloc(zoomedImage.width*zoomedImage.height*sizeof(POINT1));
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width; x++)
		{
			currPixel = buffer->pixels[x + y * buffer->width];
			zoomedImage.pixels[2*x + 2*y*zoomedImage.width] = currPixel;
			zoomedImage.pixels[2*x+1 + 2*y*zoomedImage.width] = currPixel;
		}
	}
	for (y = 0; y < zoomedImage.height; y+=2)
	{
		for (x = 0; x < zoomedImage.width; x++)
		{
			currPixel = zoomedImage.pixels[x + y * zoomedImage.width];
			zoomedImage.pixels[x + (y + 1) * zoomedImage.width] = currPixel; 
		}
	}
	return zoomedImage;
}
IMAGE zoomByZeroOrderHold(IMAGEPTR buffer)
{
	/*
	Method 2: Zero order hold
	Introduction
	Zero order hold method is another method of zooming. It is also known as zoom twice. Because it can only zoom twice. We will see in the below example that why it does that.

	Working
	In zero order hold method, we pick two adjacent elements from the rows respectively and then we add them and divide the result by two, and place their result in between those two elements. We first do this row wise and then we do this column wise.
	*/
	unsigned x, y;
	IMAGE zoomedImage;
	RGB newRgb;
	unsigned char r, g, b;
	POINT1 leftPixel,rightPixel,newPixel,upperPixel,lowerPixel;
	zoomedImage.height = (buffer->height << 1)-1;
	zoomedImage.width = (buffer->width << 1)-1;
	zoomedImage.pixels = (POINT1*)malloc(zoomedImage.width*zoomedImage.height * sizeof(POINT1));
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width - 1; x++)
		{
			leftPixel = buffer->pixels[x + y * buffer->width];
			rightPixel = buffer->pixels[(x + 1) + y * buffer->width];
			newRgb.r = (leftPixel.rgb.r + rightPixel.rgb.r) / 2;
			newRgb.g = (leftPixel.rgb.g + rightPixel.rgb.g) / 2;
			newRgb.b = (leftPixel.rgb.b + rightPixel.rgb.b) / 2;
			newPixel.rgb = newRgb;
			zoomedImage.pixels[2 * x + 2 * y*zoomedImage.width] = leftPixel;
			zoomedImage.pixels[(2 * x + 1) + 2 * y*zoomedImage.width] =rightPixel;
			zoomedImage.pixels[(2 * x + 2) + 2 * y*zoomedImage.width] = newPixel;
		}
	}
	for (y = 1; y < zoomedImage.height - 1; y += 2)
	{
		for (x = 0; x < zoomedImage.width; x++)
		{
			upperPixel = zoomedImage.pixels[x + (y-1) * zoomedImage.width];
			lowerPixel = zoomedImage.pixels[x + (y + 1) * zoomedImage.width];
			newRgb.r = (upperPixel.rgb.r + lowerPixel.rgb.r) / 2;
			newRgb.g = (upperPixel.rgb.g + lowerPixel.rgb.g) / 2;
			newRgb.b = (upperPixel.rgb.b + lowerPixel.rgb.b) / 2;
			newPixel.rgb = newRgb;
			zoomedImage.pixels[x + y * zoomedImage.width] = newPixel;
		}
	}
	return zoomedImage;
}
IMAGE zoomByKTimes(IMAGEPTR buffer,int k)
{
	/*
	Method 3: K-Times zooming
Introduction:
K times is the third zooming method we are going to discuss. It is one of the most perfect zooming algorithm discussed so far. It caters the challenges of both twice zooming and pixel replication. K in this zooming algorithm stands for zooming factor.

Working:
It works like this way.

First of all, you have to take two adjacent pixels as you did in the zooming twice. Then you have to subtract the smaller from the greater one. We call this output (OP).

Divide the output(OP) with the zooming factor(K). Now you have to add the result to the smaller value and put the result in between those two values.

Add the value OP again to the value you just put and place it again next to the previous putted value. You have to do it till you place k-1 values in it.

Repeat the same step for all the rows and the columns , and you get a zoomed images.
	*/
	unsigned x, y;
	IMAGE zoomedImage;
	RGB newRgb;
	unsigned char r, g, b;
	POINT1 leftPixel, rightPixel, newPixel, upperPixel, lowerPixel;
	zoomedImage.height = (buffer->height << 1) - 1;
	zoomedImage.width = (buffer->width << 1) - 1;
	zoomedImage.pixels = (POINT1*)malloc(zoomedImage.width*zoomedImage.height * sizeof(POINT1));
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width - 1; x++)
		{
			leftPixel = buffer->pixels[x + y * buffer->width];
			rightPixel = buffer->pixels[(x + 1) + y * buffer->width];


			leftPixel.rgb.r;


			newRgb.r = (leftPixel.rgb.r + rightPixel.rgb.r) / 2;
			newRgb.g = (leftPixel.rgb.g + rightPixel.rgb.g) / 2;
			newRgb.b = (leftPixel.rgb.b + rightPixel.rgb.b) / 2;
			newPixel.rgb = newRgb;
			zoomedImage.pixels[2 * x + 2 * y*zoomedImage.width] = leftPixel;
			zoomedImage.pixels[(2 * x + 1) + 2 * y*zoomedImage.width] = rightPixel;
			zoomedImage.pixels[(2 * x + 2) + 2 * y*zoomedImage.width] = newPixel;
		}
	}
	for (y = 1; y < zoomedImage.height - 1; y += 2)
	{
		for (x = 0; x < zoomedImage.width; x++)
		{
			upperPixel = zoomedImage.pixels[x + (y - 1) * zoomedImage.width];
			lowerPixel = zoomedImage.pixels[x + (y + 1) * zoomedImage.width];
			newRgb.r = (upperPixel.rgb.r + lowerPixel.rgb.r) / 2;
			newRgb.g = (upperPixel.rgb.g + lowerPixel.rgb.g) / 2;
			newRgb.b = (upperPixel.rgb.b + lowerPixel.rgb.b) / 2;
			newPixel.rgb = newRgb;
			zoomedImage.pixels[x + y * zoomedImage.width] = newPixel;
		}
	}
	return zoomedImage;
}


int histogramSlideLeft(IMAGEPTR buffer, unsigned value)
{
	int x, y, r, g, b;
	RGB currentRgb;
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width; x++)
		{
			currentRgb = buffer->pixels[y*buffer->width + x].rgb;
			r = currentRgb.r - value;
			g = currentRgb.g - value;
			b = currentRgb.b - value;
			/*if (r < 0)
				r = 256+r;
			if (g < 0)
				g = 256 + g;
			if (b < 0)
				b = 256 + b;*/
			if (r < 0)
				r = 0;
			if (g < 0)
				g = 0;
			if (b < 0)
				b = 0;
			currentRgb.r = r;
			currentRgb.g = g;
			currentRgb.b = b;
			buffer->pixels[y*buffer->width + x].rgb = currentRgb;
		}
	}
	return 0;
}
int histogramSlideRight(IMAGEPTR buffer, unsigned value)
{
	int x, y,r,g,b;
	RGB currentRgb;
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width; x++)
		{
			currentRgb=buffer->pixels[y*buffer->width + x].rgb;
			r = currentRgb.r+value;
			g = currentRgb.g+value;
			b = currentRgb.b+value;
			/*if (r > 255)
				r = r-256;
			if (g> 255)
				g= g - 256;
			if (b > 255)
				b = b - 256;*/

			if (r > 255)
				r = 255;
			if (g> 255)
				g = 255;
			if (b > 255)
				b = 255;

			currentRgb.r = r;
			currentRgb.g = g;
			currentRgb.b = b;
			buffer->pixels[y*buffer->width + x].rgb = currentRgb;
		}
	}
	return 0;
}
int displayImage(IMAGEPTR buffer,HDC hdc)
{
	int x, y;
	RGB tempRgb;
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width; x++)
		{
			tempRgb = buffer->pixels[y*buffer->width + x].rgb;
			SetPixel(hdc,x,y,RGB(tempRgb.r,tempRgb.g,tempRgb.b));
		}
	}
	return 0;
}
int displayScaledImage(IMAGEPTR buffer,char scaleFactor, HDC hdc)
{
	int x, y;
	scaleFactor *= -1;
	RGB tempRgb;
	for (y = 0; y < buffer->height; y += scaleFactor)
	{
		for (x = 0; x < buffer->width; x += scaleFactor)
		{
			tempRgb = buffer->pixels[y*buffer->width + x].rgb;
			SetPixel(hdc, x /(float) scaleFactor, y / (float)scaleFactor, RGB(tempRgb.r, tempRgb.g, tempRgb.b));
		}
	}
	return 0;
}
int addLabels(IMAGEPTR buffer)
{
	int x, y;
	const char UNLABELLED = 0;
	POINT1 *curPixel, *rightPixel, *lowerRightPixel, *lowerPixel, *lowerLeftPixel;
	RGB tempRgb;
	unsigned labelValue = 1;
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width; x++)
		{
			curPixel = &buffer->pixels[x + y * buffer->width];
			if (x > 0 && x < buffer->width - 1 && y < buffer->height - 1)
			{
				if (curPixel->label == UNLABELLED)
					curPixel->label = labelValue++;

				rightPixel = &buffer->pixels[(x + 1) + y * buffer->width];
				lowerRightPixel = &buffer->pixels[(x + 1) + (y + 1) * buffer->width];
				lowerPixel = &buffer->pixels[x + (y + 1) * buffer->width];
				lowerLeftPixel = &buffer->pixels[(x - 1) + (y + 1) * buffer->width];


				if (compareRGBStructs(&curPixel->rgb, &rightPixel->rgb))
					rightPixel->label = curPixel->label;

				if (compareRGBStructs(&curPixel->rgb, &lowerRightPixel->rgb))
					lowerRightPixel->label = curPixel->label;

				if (compareRGBStructs(&curPixel->rgb, &lowerPixel->rgb))
					lowerPixel->label = curPixel->label;


				if (compareRGBStructs(&curPixel->rgb, &lowerLeftPixel->rgb))
					lowerLeftPixel->label = curPixel->label;

			}
			tempRgb = buffer->pixels[x + y * buffer->width].rgb;
		}
	}
	return 0;
}

int getGrayScaleHistogram(IMAGEPTR grayscaleImageData, HISTOGRAM *arr)
{
	unsigned int i;
	for (i = 0; i < grayscaleImageData->noOfPixels; i++)
	{
		arr->frequency[grayscaleImageData->pixels[i].rgb.r]++;
		/*arr->frequency[grayscaleImageData->pixels[i].rgb.g]++;
		arr->frequency[grayscaleImageData->pixels[i].rgb.b]++;*/
	}
	return 0;
}

int getGrayScalePMF(IMAGEPTR grayscaleImageData, PMF *arr)
{
	unsigned int i;
	HISTOGRAM histogram;
	getGrayScaleHistogram(grayscaleImageData, &histogram);
	for (i = 0; i < 256; i++)
	{
		arr->value[i] =(float) histogram.frequency[i] / grayscaleImageData->noOfPixels;
	}
	return 0;
}

int getGrayScaleCMF(IMAGEPTR grayscaleImageData, CMF *arr)
{
	unsigned int i;
	PMF pmf;
	getGrayScalePMF(grayscaleImageData, &pmf);
	arr->value[0] = (double)pmf.value[0];
	for (i = 1; i < 256; i++)
	{
		arr->value[i] = ((double)arr->value[i-1] + (double)pmf.value[i]);
	}
	return 0;
}
int equalizeUsingHistogram(IMAGEPTR grayscaleImageData)
{
	CMF cmf;
	unsigned grayLevels = 256;
	getGrayScaleCMF(grayscaleImageData, &cmf);
	for (int i = 0; i < 256; i++)
	{
		cmf.value[i] *= grayLevels-1;
	}
	RGB tempRgb, curRgb;
	for (int y = 0; y < grayscaleImageData->height; y++)
	{
		for (int x = 0; x < grayscaleImageData->width; x++)
		{
			curRgb = grayscaleImageData->pixels[y*grayscaleImageData->width + x].rgb;
			tempRgb.r = cmf.value[curRgb.r];
			tempRgb.g = cmf.value[curRgb.g];
			tempRgb.b = cmf.value[curRgb.b];
			grayscaleImageData->pixels[y*grayscaleImageData->width + x].rgb = tempRgb;
		}
	}
	return 0;
}
int compareRGBStructs(RGB *rgb1, RGB *rgb2)
{
	if (rgb1->r == rgb2->r && rgb1->g == rgb2->g && rgb1->b == rgb2->b)
		return 1;
	return 0;
}


int increaseContrastByHistogramStetching(IMAGEPTR buffer)
{
	int x, y,newVal;
	RGB currentRgb,tempRgb;
	float min = 1, max = 254;
	for (y = 0; y < buffer->height; y++)
	{
		for (x = 0; x < buffer->width; x++)
		{
			currentRgb =buffer->pixels[y*buffer->width + x].rgb;
			newVal = (currentRgb.r - min) / (max - min) * 255;
			tempRgb.r = newVal;
			tempRgb.g = newVal;
			tempRgb.b = newVal;
			buffer->pixels[y*buffer->width + x].rgb = tempRgb;
		}
	}
	return 0;
}
int flipMaskVertical(MASK *m)
{
	unsigned char topRowPointer, bottomRowPointer, x;
	char temp;
	topRowPointer = 0;
	bottomRowPointer = m->height - 1;
	while (topRowPointer<bottomRowPointer)
	{
		for (x = 0; x < m->width; x++)
		{
			temp = m->values[topRowPointer*m->width + x];
			m->values[topRowPointer*m->width + x] = m->values[bottomRowPointer*m->width + x];
			m->values[bottomRowPointer*m->width + x] = temp;
		}
		topRowPointer++;
		bottomRowPointer--;
	}
	return 0;
}
int flipMaskHorizontal(MASK *m)
{
	unsigned char leftColumnPointer, rightColumnPointer, y;
	char temp;
	leftColumnPointer = 0;
	rightColumnPointer = m->width - 1;
	while (leftColumnPointer<rightColumnPointer)
	{
		for (y= 0; y < m->height; y++)
		{
			temp = m->values[leftColumnPointer + y * m->width];
			m->values[leftColumnPointer+y * m->width] = m->values[rightColumnPointer + y * m->width];
			m->values[rightColumnPointer + y * m->width] = temp;
		}
		leftColumnPointer++;
		rightColumnPointer--;
	}
	return 0;
}


int applyMaskOnImage(IMAGEPTR buffer, MASK m)
{
	unsigned imageX, imageY, maskX, maskY;
	int tempImageX, tempImageY;
	float valR,valG,valB;
	char xfactor = -1 * (m.width >> 1), yFactor = -1 * (m.height >> 1);
	for (imageY = 0; imageY < buffer->height; imageY++)
	{
		for (imageX = 0; imageX< buffer->width; imageX++)
		{
			valR = 0;
			valG = 0;
			valB = 0;
			for (maskY = 0; maskY < m.height; maskY++)
			{
				//tempImageY = imageY + maskY + yFactor;
				tempImageY = imageY + maskY;
				for (maskX = 0; maskX < m.width; maskX++)
				{
					//tempImageX = imageX + maskX + xfactor;
					tempImageX = imageX + maskX;
					if (tempImageX >= 0 && tempImageX < buffer->width && tempImageY >= 0 && tempImageY < buffer->height)
					{
						int v = buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.r;
						valR += (float)buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.r * m.values[maskX + maskY * m.width];
						valG += (float)buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.g * m.values[maskX + maskY * m.width];
						valB += (float)buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.b * m.values[maskX + maskY * m.width];
					}
				}
			}
			buffer->pixels[imageX + imageY * buffer->width].rgb.r = sqrt(valR*valR);
			buffer->pixels[imageX + imageY * buffer->width].rgb.g = sqrt(valG*valG);
			buffer->pixels[imageX + imageY * buffer->width].rgb.b = sqrt(valB*valB);
		}
	}
	return 0;
}







int blurImageByMeanFilter(IMAGEPTR buffer,unsigned order)
{
	/*
	Mean filter
Mean filter is also known as Box filter and average filter. A mean filter has the following properties.

It must be odd ordered
The sum of all the elements should be 1
All the elements should be same
If we follow this rule, then for a mask of 3x3. We get the following result.

1/9	1/9	1/9
1/9	1/9	1/9
1/9	1/9	1/9
Since it is a 3x3 mask, that means it has 9 cells. The condition that all the element sum should be equal to 1 can be achieved by dividing each value by 9. As

1/9 + 1/9 + 1/9 + 1/9 + 1/9 + 1/9 + 1/9 + 1/9 + 1/9 = 9/9 = 1
	*/
	if (order < 1 || order % 2 == 0)
		return 0;
	MASK m;
	unsigned i;
	m.height = order;
	m.width = order;
	m.values = (float*)malloc(m.height*(m.width) * sizeof(float));
	for (i = 0; i < m.height*(m.width); i++)
	{
		m.values[i] = 1.0 / (m.height*m.width);
	}
	applyMaskOnImage(buffer, m);
	return 0;
}



int blurImageByWeightedAverageFilter(IMAGEPTR buffer, unsigned order) {
	/*
	In weighted average filter, we gave more weight to the center value. Due to which the contribution of center becomes more then the rest of the values. Due to weighted average filtering, we can actually control the blurring.

Properties of the weighted average filter are.

It must be odd ordered
The sum of all the elements should be 1
The weight of center element should be more then all of the other elements
Filter 1
1	1	1
1	2	1
1	1	1
The two properties are satisfied which are (1 and 3). But the property 2 is not satisfied. So in order to satisfy that we will simple divide the whole filter by 10, or multiply it with 1/10.

Filter 2
1	1	1
1	10	1
1	1	1
Dividing factor = 18.
	*/
	if (order < 1 || order % 2 == 0)
		return 0;
	MASK m;
	unsigned i;
	m.height = order;
	m.width = order;
	m.values = (float*)malloc(m.height*(m.width) * sizeof(float));
	for (i = 0; i < m.height*m.width; i++)
	{
		if(i== ((m.height*m.width)>>1))
			m.values[i] = 10.0 / 18;
		else
			m.values[i] = 1.0 / 18;
	}
	applyMaskOnImage(buffer, m);
	return 0;
}


int getVerticalPrewittMask(MASK* m)
{
	m->height = 3;
	m->width = 3;
	m->values = (float*)malloc(m->height*m->width * sizeof(float));
	m->values[0] = -1;
	m->values[1] = 0;
	m->values[2] = 1;
	m->values[3] = -1;
	m->values[4] = 0;
	m->values[5] = 1;
	m->values[6] = -1;
	m->values[7] = 0;
	m->values[8] = 1;
	return 0;
}
int getHorizontalPrewittMask(MASK* m)
{
	m->height = 3;
	m->width = 3;
	m->values = (float*)malloc(m->height*m->width * sizeof(float));
	m->values[0] = -1;
	m->values[1] = -1;
	m->values[2] = -1;
	m->values[3] = 0;
	m->values[4] = 0;
	m->values[5] = 0;
	m->values[6] = 1;
	m->values[7] = 1;
	m->values[8] = 1;
	return 0;
}


MASK getVerticalSobelMask()
{
	MASK m;
	m.height = 3;
	m.width = 3;
	m.values = (float*)malloc(m.height*m.width * sizeof(float));
	m.values[0] = -1;
	m.values[1] = 0;
	m.values[2] = 1;
	m.values[3] = -2;
	m.values[4] = 0;
	m.values[5] = 2;
	m.values[6] = -1;
	m.values[7] = 0;
	m.values[8] = 1;
	return m;
}

MASK getHorizontalSobelMask()
{
	MASK m;
	m.height = 3;
	m.width = 3;
	m.values = (float*)malloc(m.height*m.width * sizeof(float));
	m.values[0] = -1;
	m.values[1] =-2;
	m.values[2] = -1;
	m.values[3] = 0;
	m.values[4] = 0;
	m.values[5] = 0;
	m.values[6] = 1;
	m.values[7] = 2;
	m.values[8] = 1;
	return m;
}


int detectEdges(IMAGEPTR buffer,unsigned operatorMask)
{
	unsigned imageX, imageY, maskX, maskY;
	MASK verticalMask, horizontalMask;
	int tempImageX, tempImageY;
	int gXR,gYR,gXG,gYG,gXB,gYB;
	char xfactor,yFactor;
	
	switch (operatorMask)
	{
	case PREWITT_OPERATOR:
		getVerticalPrewittMask(&verticalMask);
		getHorizontalPrewittMask(&horizontalMask);
		break;
	case SOBEL_OPERATOR:
		horizontalMask = getHorizontalSobelMask();
		verticalMask = getVerticalSobelMask();
		break;
	case LAPLACIAN_OF_GAUSSIAN_OPERATOR:
		horizontalMask = getLaplacianMask(POSITIVE_LAPLACIAN);
		verticalMask = getLaplacianMask(NEGATIVE_LAPLACIAN);
		break;
	default:
		break;
	}
	
	
	xfactor = -1 * (horizontalMask.width >> 1);
	 yFactor = -1 * (verticalMask.height >> 1);
	for (imageY = 0; imageY < buffer->height; imageY++)
	{
		for (imageX = 0; imageX< buffer->width; imageX++)
		{
			gXR= gYR= gXG= gYG= gXB= gYB = 0;
			for (maskY = 0; maskY < verticalMask.height; maskY++)
			{
				//tempImageY = imageY + maskY + yFactor;
				tempImageY = imageY + maskY;
				for (maskX = 0; maskX < horizontalMask.width; maskX++)
				{
					//tempImageX = imageX + maskX + xfactor;
					tempImageX = imageX + maskX ;
					if (tempImageX >= 0 && tempImageX < buffer->width && tempImageY >= 0 && tempImageY < buffer->height)
					{
						gXR +=buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.r *horizontalMask.values[maskX + maskY * horizontalMask.width];
						gYR += buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.r * verticalMask.values[maskX + maskY * verticalMask.width];

						gXG += buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.g *horizontalMask.values[maskX + maskY * horizontalMask.width];
						gYG += buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.g * verticalMask.values[maskX + maskY * verticalMask.width];

						gXB += buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.b *horizontalMask.values[maskX + maskY * horizontalMask.width];
						gYB += buffer->pixels[tempImageX + tempImageY * buffer->width].rgb.b * verticalMask.values[maskX + maskY * verticalMask.width];



					}
				}
			}
			buffer->pixels[imageX + imageY * buffer->width].rgb.r = sqrt(gXR*gXR+gYR*gYR);
			buffer->pixels[imageX + imageY * buffer->width].rgb.g = sqrt(gXG*gXG+gYG*gYG);
			buffer->pixels[imageX + imageY * buffer->width].rgb.b = sqrt(gXB*gXB + gYB * gYB);
		}
	}
	return 0;
}



MASK getRobinsonCompassMask(unsigned direction)
{
	MASK m;
	m.height = 3;
	m.width = 3;
	m.values = (float*)malloc(m.height*m.width * sizeof(float));

	switch (direction)
	{
	case NORTH_DIRECTION:
		/*
		-1	0	1
-2	0	2
-1	0	1
		*/
		m.values[0] = -1;
		m.values[1] = 0;
		m.values[2] = 1;
		m.values[3] = -2;
		m.values[4] = 0;
		m.values[5] = 2;
		m.values[6] = -1;
		m.values[7] = 0;
		m.values[8] = 1;
		break;
	case NORTH_WEST_DIRECTION:
		/*
		0	1	2
-1	0	1
-2	-1	0
		*/
		m.values[0] =0;
		m.values[1] = 1;
		m.values[2] = 2;
		m.values[3] = -1;
		m.values[4] = 0;
		m.values[5] = 1;
		m.values[6] = -2;
		m.values[7] = -1;
		m.values[8] = 0;
		break;
	case WEST_DIRECTION:
		/*
		1	2	1
0	0	0
-1	-2	-1
		*/
		m.values[0] = 1;
		m.values[1] = 2;
		m.values[2] = 1;
		m.values[3] = 0;
		m.values[4] = 0;
		m.values[5] = 0;
		m.values[6] = -1;
		m.values[7] = -2;
		m.values[8] = -1;
		break;
	case SOUTH_WEST_DIRECTION:
		/*
		2	1	0
1	0	-1
0	-1	-2
		*/
		m.values[0] = 2;
		m.values[1] = 1;
		m.values[2] = 0;
		m.values[3] = 1;
		m.values[4] = 0;
		m.values[5] = -1;
		m.values[6] = 0;
		m.values[7] = -1;
		m.values[8] = -2;
		break;
	case SOUTH_DIRECTION:
		/*1	0 - 1
			2	0 - 2
			1	0 - 1*/
		m.values[0] = 1;
		m.values[1] = 0;
		m.values[2] = -1;
		m.values[3] = 2;
		m.values[4] = 0;
		m.values[5] = -2;
		m.values[6] = 1;
		m.values[7] = 0;
		m.values[8] = -1;
		break;
	case SOUTH_EAST_DIRECTION:
		/*
		0	-1	-2
1	0	-1
2	1	0
		*/
		m.values[0] = 0;
		m.values[1] = -1;
		m.values[2] = -2;
		m.values[3] = 1;
		m.values[4] = 0;
		m.values[5] = -1;
		m.values[6] = 2;
		m.values[7] = 1;
		m.values[8] = 0;
		break;
	case EAST_DIRECTION:
		/*
		-1	-2	-1
0	0	0
1	2	1
		*/
		m.values[0] = -1;
		m.values[1] = -2;
		m.values[2] = -1;
		m.values[3] = 0;
		m.values[4] = 0;
		m.values[5] = 0;
		m.values[6] = 1;
		m.values[7] =2;
		m.values[8] = 1;
		break;
	case NORTH_EAST_DIRECTION:
		/*-2 - 1	0
			- 1	0	1
			0	1	2*/
		m.values[0] = -2;
		m.values[1] = -1;
		m.values[2] = 0;
		m.values[3] = -1;
		m.values[4] = 0;
		m.values[5] = 1;
		m.values[6] = 0;
		m.values[7] = 1;
		m.values[8] = 2;
		break;
	default:
		break;
	}
	return m;
}




MASK getKirschCompassMask(unsigned direction)
{
	MASK m;
	m.height = 3;
	m.width = 3;
	m.values = (float*)malloc(m.height*m.width * sizeof(float));

	switch (direction)
	{
	case NORTH_DIRECTION:
		/*
		-3	-3	5
-3	0	5
-3	-3	5
		*/
		m.values[0] = -3;
		m.values[1] = -3;
		m.values[2] = 5;
		m.values[3] = -3;
		m.values[4] = 0;
		m.values[5] = 5;
		m.values[6] = -3;
		m.values[7] = -3;
		m.values[8] = 5;
		break;
	case NORTH_WEST_DIRECTION:
		/*
		-3	5	5
-3	0	5
-3	-3	-3
		*/
		m.values[0] = -3;
		m.values[1] = 5;
		m.values[2] = 5;
		m.values[3] = -3;
		m.values[4] = 0;
		m.values[5] = 5;
		m.values[6] = -3;
		m.values[7] = -3;
		m.values[8] = -3;
		break;
	case WEST_DIRECTION:
		/*
		5	5	5
-3	0	-3
-3	-3	-3
		*/
		m.values[0] = 5;
		m.values[1] = 5;
		m.values[2] = 5;
		m.values[3] = -3;
		m.values[4] = 0;
		m.values[5] = -3;
		m.values[6] = -3;
		m.values[7] = -3;
		m.values[8] = -3;
		break;
	case SOUTH_WEST_DIRECTION:
		/*
		5	5	-3
5	0	-3
-3	-3	-3
		*/
		m.values[0] = 5;
		m.values[1] = 5;
		m.values[2] = -3;
		m.values[3] = 5;
		m.values[4] = 0;
		m.values[5] = -3;
		m.values[6] = -3;
		m.values[7] = -3;
		m.values[8] = -3;
		break;
	case SOUTH_DIRECTION:
		/*
		5	-3	-3
5	0	-3
5	-3	-3
		*/
		m.values[0] = 5;
		m.values[1] = -3;
		m.values[2] = -3;
		m.values[3] = 5;
		m.values[4] = 0;
		m.values[5] = -3;
		m.values[6] = 5;
		m.values[7] = -3;
		m.values[8] = -3;
		break;
	case SOUTH_EAST_DIRECTION:
		/*
		-3	-3	-3
5	0	-3
5	5	-3
		*/
		m.values[0] = -3;
		m.values[1] = -3;
		m.values[2] = -3;
		m.values[3] = 5;
		m.values[4] = 0;
		m.values[5] = -3;
		m.values[6] = 5;
		m.values[7] = 5;
		m.values[8] = -3;
		break;
	case EAST_DIRECTION:
		/*
		-3	-3	-3
-3	0	-3
5	5	5
		*/
		m.values[0] = -3;
		m.values[1] = -3;
		m.values[2] = -3;
		m.values[3] = -3;
		m.values[4] = 0;
		m.values[5] = -3;
		m.values[6] = 5;
		m.values[7] = 5;
		m.values[8] = 5;
		break;
	case NORTH_EAST_DIRECTION:
		/*
		-3	-3	-3
-3	0	5
-3	5	5
		*/
		m.values[0] = -3;
		m.values[1] = -3;
		m.values[2] = -3;
		m.values[3] = -3;
		m.values[4] = 0;
		m.values[5] = 5;
		m.values[6] = -3;
		m.values[7] = 5;
		m.values[8] = 5;
		break;
	default:
		break;
	}
	return m;
}
MASK getLaplacianMask(unsigned type)
{
	MASK m;
	m.height = 3;
	m.width = 3;
	m.values = (float*)malloc(m.height*m.width * sizeof(float));

	switch (type)
	{
	case POSITIVE_LAPLACIAN:
		/*
		0	1	0
1	-4	1
0	1	0
		*/
		m.values[0] = 0;
		m.values[1] = 1;
		m.values[2] = 0;
		m.values[3] = 1;
		m.values[4] = -4;
		m.values[5] = 1;
		m.values[6] = 0;
		m.values[7] = 1;
		m.values[8] = 0;
		break;
	case NEGATIVE_LAPLACIAN:
		/*
		0	-1	0
-1	4	-1
0	-1	0
		*/
		m.values[0] = 0;
		m.values[1] = -1;
		m.values[2] = 0;
		m.values[3] = -1;
		m.values[4] = 4;
		m.values[5] = -1;
		m.values[6] = 0;
		m.values[7] = -1;
		m.values[8] = 0;
		break;
	default:
		break;
	}
	
	return m;
}
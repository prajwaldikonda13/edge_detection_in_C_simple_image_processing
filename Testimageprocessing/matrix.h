MATRIX CreateMatrix(int height, int width, unsigned type)
{
	MATRIX m;
	m.height = height;
	m.width = width;
	m.p = (unsigned char*)malloc(height*width);
	return m;
}
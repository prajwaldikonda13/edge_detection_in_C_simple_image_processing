int processImage(HDC hdc)
{
	IMAGE image, image1;
	getImage(&image, "path to your bitmap(.bmp) file.");
	displayImage(&image, hdc);

	getGrayScaleImage(&image, "path to your bitmap(.bmp) file.");
	displayImage(&image, hdc);

	equalizeUsingHistogram(&image);
	displayImage(&image, hdc);

	blurImageByMeanFilter(&image, 3);
	displayImage(&image, hdc);


	detectEdges(&image, LAPLACIAN_OF_GAUSSIAN_OPERATOR);
	displayImage(&image, hdc);
	//displayScaledImage(&image,-2, hdc);



	

	return 0;
}
// 2DImProExercise3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <opencv2\opencv_modules.hpp>

#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

void HistogramOrientedGradient(Mat *image, Mat *output, double cellsize)
{
	Mat gx, gy;
	Sobel(*image, gx, CV_32F, 1, 0, 1);
	Sobel(*image, gy, CV_32F, 0, 1, 1);

	Mat mag, angle;
	cartToPolar(gx, gy, mag, angle, 1);

	Size imsize = image->size();
	Size size(cellsize * floor(imsize.width / cellsize), cellsize * floor(imsize.height / cellsize));

	resize(*image, *output, size);
}

int main(int argc, char **argv)
{
	Mat image = imread(argv[1]);
	image.convertTo(image, CV_32F, 1 / 255.0);
	Mat *im = &image;

	Mat resim = image;
	Mat *res = &resim; // Weil pointer lustig sind

	string str = string(argv[2]);
	istringstream iss(str);
	double d;
	iss >> d;
	if (argc > 3)
	{
		cerr << "Too many input arguments." << endl;
		return -2;
	}
	if (!iss.eof())
	{
		cerr << "Expected a double value as second input." << endl;
		return -1;
	}

	HistogramOrientedGradient(im, res, d); // stod: string to double conversion, stdlib.h

	imwrite("Test.png", 255 * resim); // war zu einfach, das zurück auf [0,255] zu skalieren :D

    return 0;
}


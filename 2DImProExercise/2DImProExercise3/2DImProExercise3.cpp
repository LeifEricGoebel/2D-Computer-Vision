// 2DImProExercise3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <opencv2\opencv_modules.hpp>

#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <string>

#define PI 3.14159265

using namespace cv;
using namespace std;

vector<double> ComputeHistogram(Mat *angle, Mat *mag, int d, int index_x, int index_y)
{
	vector<double> hist(9);
	for (int i = 0; i < d; ++i)
	{
		for (int j = 0; j < d; ++j)
		{
			double x = angle->at<float>(Point(i + index_x*d, j + index_y*d));
			// calculate first: 18 bins of width 20 degrees
			// calculate then : 9 bins of with 20 degrees with "i + 180" degrees is in the same equivalence class of "i":
			int mod = ((int)floor(x) % 20) % 9;

			hist[mod] = (int)(20 * mod * mag->at<float>(i, j));
		}
	}
	return hist;
}

void HistogramOrientedGradient(Mat *image, vector<vector<vector<double> > > *list, Mat *hist, double cellsize)
{
	// use sobel filter to compute the finite differences to approximate the gradient of the image in x and y direction:
	Mat gx, gy;
	Sobel(*image, gx, CV_32F, 1, 0, 1);
	Sobel(*image, gy, CV_32F, 0, 1, 1);

	// compute gradient magniture and phase out of finite differences in x and y direction:
	Mat mag, angle;
	cartToPolar(gx, gy, mag, angle, 1);
	Mat *ma = &mag, *an = &angle;

	size_t size_x = list->size();
	size_t size_y = (*list)[0].size();

	// create and fill a 2D list containing the histogram as list of size 9 (as desired):
	for (int i = 0; i < size_x; ++i)
	{
		for (int j = 0; j < size_y; ++j)
		{
			(*list)[i][j] = ComputeHistogram(an, ma, cellsize, i, j);
		}
	}

	for (int i = 0; i < size_x; ++i)
	{
		for (int j = 0; j < size_y; ++j)
		{
			Mat tmp = Mat::zeros(cellsize, cellsize, CV_8U); // submatrix for the lines
			//Mat tmp2(cellsize, cellsize, CV_32F); // submatrix of angle
			

			// compute line using (sin(x), sin(y)) representation
			auto Max = max_element((*list)[i][j].begin(), (*list)[i][j].end());
			auto Min = min_element((*list)[i][j].begin(), (*list)[i][j].end());

			for (int l = 0; l < 9; ++l)
			{
				// calculate 2 points defining the line using the that i+180 and i lie in the same equivalence class:
				Point p1(floor(cellsize / 2) + floor(cellsize/2)*sin((180.0 + l*20) * (PI / 180)), floor(cellsize / 2) + floor(cellsize / 2)*cos((180.0 + l * 20) * (PI / 180)));
				Point p2(floor(cellsize / 2) + floor(cellsize / 2)*sin((l * 20) * (PI / 180)), floor(cellsize / 2) + floor(cellsize / 2)*cos((l * 20) * (PI / 180)));

				// color of the line depending of the value of the histogram value corresponding to the angle:
				Scalar color = 255*((*list)[i][j][l] - *Min) / (*Max - *Min);

				// calculate the line:
				line(tmp, p1, p2, color);
			}

			// include calculation into histogram matrix (hist):
			for (int k = 0; k < cellsize; ++k)
			{
				for (int m = 0; m < cellsize; ++m)
				{
					hist->at<uchar>(k + cellsize*i, m + cellsize*j) = tmp.at<uchar>(k, m);
				}
			}
		}
	}
	return;
}

int main(int argc, char **argv)
{
	if (argc > 3) // argv[0] contains the path to the executable
	{
		cerr << "Too many input arguments." << endl;
		return -2;
	}
	Mat image = imread(argv[1]);

	string str = string(argv[2]);
	istringstream iss(str);
	double d;
	iss >> d;

	// check if 2nd input argument (actually 3rd) is of type double:
	if (!iss.eof())
	{
		cerr << "Expected a double value as second input." << endl;
		return -1;
	}

	// resize image such that is can hold as many complete cells as possible:
	Size imsize = image.size();
	Size size(d * floor(imsize.width / d), d * floor(imsize.height / d));

	resize(image, image, size);

	Mat hist = Mat::zeros(size, CV_8U);
	vector<vector<vector<double> > > resim(floor(imsize.width / d), vector<vector<double> >(floor(imsize.height / d), vector<double>(d)));

	// using pointer in function calls is more efficient:
	Mat *im = &image;
	vector<vector<vector<double> > > *res = &resim; 
	Mat *his = &hist;

	HistogramOrientedGradient(im, res, his, d); // stod: string to double conversion, stdlib.h

	imwrite("HOG.png", hist);

    return 0;
}


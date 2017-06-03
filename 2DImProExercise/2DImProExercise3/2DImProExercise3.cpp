// 2DImProExercise3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <opencv2\opencv_modules.hpp>

#include <stdlib.h>
#include <iostream>
#include <cmath>

#define PI 3.14159265

using namespace cv;
using namespace std;

vector<double> ComputeHistogram(Mat *angle, Mat *mag, int d, int index_x, int index_y)
{
	vector<double> hist(d);
	for (int i = 0; i < d; ++i)
	{
		for (int j = 0; j < d; ++j)
		{
			double x = angle->at<float>(Point(i + index_x*d, j + index_y*d));
			int mod = (int)floor(x) % 20;
			mod = mod % 9;
			hist[mod] = x * mag->at<float>(i, j);
		}
	}
	return hist;
}

void HistogramOrientedGradient(Mat *image, vector<vector<vector<double> > > *list, Mat *hist, double cellsize)
{
	Mat gx, gy;
	Sobel(*image, gx, CV_32F, 1, 0, 1);
	Sobel(*image, gy, CV_32F, 0, 1, 1);

	Mat mag, angle;
	cartToPolar(gx, gy, mag, angle, 1);
	Mat *ma = &mag, *an = &angle;

	size_t size_x = list->size();
	size_t size_y = (*list)[0].size();

	

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
			Mat tmp(cellsize, cellsize, CV_32F); // submatrix for the lines
			Mat tmp2(cellsize, cellsize, CV_32F); // submatrix of angle
			for (int k = 0; k < cellsize; ++k)
			{
				for (int m = 0; m < cellsize; ++m)
				{
					tmp.at<float>(Point(k, m)) = image->at<float>(Point(k + cellsize*i, m + cellsize*j));
				}
			}

			// compute line using (sin(x), sin(y)) representation
			auto Max = std::max_element((*list)[i][j].begin(), (*list)[i][j].end());
			for (int l = 0; l < 9; ++l)
			{
				Point p1(cellsize*sin((180.0 + l*20) * (PI / 180)), cellsize*cos((180.0 + l * 20) * (PI / 180)));
				Point p2(cellsize*sin((l * 20) * (PI / 180)), cellsize*cos((l * 20) * (PI / 180)));

				double val = (*list)[i][j][l];

				line(tmp, p1, p2,  val / *Max * 255.0);
			}

			// include calculation into histogram matrix (hist):
			for (int k = 0; k < cellsize; ++k)
			{
				for (int m = 0; m < cellsize; ++m)
				{
					hist->at<float>(k + cellsize*i, m + cellsize*j) = tmp.at<float>(k, m);
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	Mat image = imread(argv[1]);
	image.convertTo(image, CV_32F, 1 / 255.0);
	Mat *im = &image;

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

	Size imsize = image.size();
	Size size(d * floor(imsize.width / d), d * floor(imsize.height / d));

	resize(image, image, size);

	vector<vector<vector<double> > > resim(floor(imsize.width / d), vector<vector<double> >(floor(imsize.height / d), vector<double>(d)));
	vector<vector<vector<double> > > *res = &resim; // Weil pointer lustig sind

	Mat hist = Mat::zeros(size, CV_32F);
	Mat *his = &hist;

	HistogramOrientedGradient(im, res, his, d); // stod: string to double conversion, stdlib.h

	imwrite("HOG.png", 255 * hist); // war zu einfach, das zurück auf [0,255] zu skalieren :D

    return 0;
}


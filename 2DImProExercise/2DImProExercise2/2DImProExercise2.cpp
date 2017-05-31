// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include <iostream>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\photo.hpp>
#include <opencv2\ximgproc.hpp>

using namespace cv;
using namespace cv::ximgproc;

using namespace std;

int main(int argc, char **argv)
{
	Mat input;
	
	if (argc == 1)
	{
		argv[1] = "SaltAndPepper.png";
	}
	
	input = imread(argv[1]);
	Mat med, bilat, guide, roll;
	medianBlur(input, med, 5);	
	imwrite(std::string(argv[1]) + "MedianFilter.png", med);

	bilateralFilter(input, bilat, 5, 1.0, 1.0);
	imwrite(std::string(argv[1]) + "BilateralFilter.png", bilat);

	guidedFilter(input, input, guide, 5, 0.001, -1);
	imwrite(std::string(argv[1]) + "GuidedFilter.png", guide);

	rollingGuidanceFilter(input, roll);
	imwrite(std::string(argv[1]) + "RollingGuidanceFilter.png", roll);

    return 0;
}


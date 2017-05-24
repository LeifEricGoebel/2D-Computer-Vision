// 2DImProExercise1.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>


//using namespace cv;

// global variables
const int slider1_max = 200, slider2_max = 200;
int slider1_value, slider2_value;
cv::Mat InputImage;

void updateCanny(int, void*)
{
	// compute edges using Canny edge detector:
	cv::Mat OutputImage;
	cv::Canny(InputImage, OutputImage, slider1_value, slider2_value, 3);
	cv::imshow("Canny Edge Detection", OutputImage);
}

// Exercise 1.3:
void startEx1_3()
{
	// read input image:
	InputImage = cv::imread("clock.png", 0);

	// create parent window:
	cv::namedWindow("Canny Edge Detection", 1);

	// initialize values:
	slider1_value = 100;
	slider2_value = 150;

	// show initial image before applying edge detection:
	imshow("Canny Edge Detection", InputImage);

	// create two trackbars:
	cv::createTrackbar("Threshold 1", "Canny Edge Detection", &slider1_value, slider1_max, updateCanny);
	cv::createTrackbar("Threshold 2", "Canny Edge Detection", &slider2_value, slider2_max, updateCanny);

	cv::waitKey();
}

void startEx1_4()
{
	// import the required images:
	cv::Mat liveview = cv::imread("liveview.jpg");
	cv::Mat mask = cv::imread("mask.jpg", 0);
	cv::Mat overlay = cv::imread("overlay.jpg");

	// copy "liveview" to AR:
	cv::Mat AR(liveview);
	
	// iterate the 2 image dimensions:
	for (int i = 0; i < AR.size().height; ++i)
	{
		for (int j = 0; j < AR.size().width; ++j)
		{
			cv::Scalar mas = mask.at<uchar>(i, j);
			
			// check for mask and replace respective pixels in "liveview" by "overlay":
			if (mas.val[0] == 0)
			{
				AR.at<cv::Vec3b>(i, j) = overlay.at<cv::Vec3b>(i, j);
			}
		}
	}

	// write image to file and show image in new window:
	cv::imwrite("Augmented Vision.jpg", AR);
	cv::namedWindow("Augmented Vision", 1);
	cv::imshow("Augmented Vision", AR);

	cv::waitKey();
}


int main(int argc, char **argv)
{
	int i = 1;

	while (i != 0)
	{
		// Print Exercise selection menu:
		std::cout << "Which exercise do you want to see? Enter the correct number:\n";
		std::cout << "......... Exercise 1.3 ................ Enter: 3\n";
		std::cout << "......... Exercise 1.4 ................ Enter: 4\n";
		std::cout << "................................................\n";
		std::cout << "............ Leave .................... Enter: 0\n";
		std::cout << "\n";
		std::cout << "............ Input: ";

		std::cin >> i;
		// check if exercise 3 or 4, or leave, is selected, otherwise show error and repeat:
		if (i == 3)
		{
			startEx1_3();
		}
		else if (i == 4)
		{
			startEx1_4();
		}
		else
		{
			std::cout << "......... Error: Incorrect input! ......................... \n";
		}
	}

    return 0;
}


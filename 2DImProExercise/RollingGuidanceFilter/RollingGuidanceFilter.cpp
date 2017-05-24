#include "stdafx.h">
#include "RollingGuidanceFilter.h"

int main()
{
	string name = "./imgs/image.png";

	Mat img = imread(name);

	if(img.empty()){
		printf("No such file.\n");
		getchar();
		exit(1);
	}

	/*Mat can;
	Canny(img, can, 250.0, 290.0);*/

	clock_t startTime = clock();
	Mat res = RollingGuidanceFilter::filter(img,3,25.5,10);
	printf("Elapsed Time: %d ms\n",clock()-startTime);

	

	imshow("img",img);
	//imshow("can", can);
	imshow("res",res);
	waitKey();

	return 0;
}
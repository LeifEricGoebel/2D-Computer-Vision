#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	CascadeClassifier face_cascade;

	if( !face_cascade.load( "haarcascade_frontalface_alt.xml" ) ) { printf( "--(!)Error loading\n" ); return -1; };
	
	int in = 2;
	do
	{
		cout << "For the default example, enter ""1""." << endl;
		cout << "For webcam capture, enter ""2""." << endl;
		cout << "To exit the application, press ""0""." << endl;

		cin >> in;

		if( in == 1 )
		{
			cout << "Accessing default example:" << endl;

			vector<Rect> faces;
			Mat frame = imread( "group.jpeg" );

			Mat frame_gray;

			cvtColor( frame , frame_gray , CV_BGR2GRAY );

			face_cascade.detectMultiScale( frame_gray , faces , 1.1 , 2 , 0 | CV_HAAR_SCALE_IMAGE , Size( 30 , 30 ) );

			for( size_t i = 0; i < faces.size(); i++ )
			{
				Point center( faces[ i ].x + faces[ i ].width*0.5 , faces[ i ].y + faces[ i ].height*0.5 );
				ellipse( frame , center , Size( faces[ i ].width*0.5 , faces[ i ].height*0.5 ) , 0 , 0 , 360 , Scalar( 0 , 200 , 255 ) , 4 , 8 , 0 );

				Mat faceROI = frame_gray( faces[ i ] );
				std::vector<Rect> eyes;
			}
			imshow( "Default example" , frame );
			while( true )
			{
				int c = waitKey( 0 );
				if( (char)c == 'c' ) 
				{
					destroyWindow( "Default example" );
					break; 
				}
			}
				
		}

		else if( in == 2 )
		{
			cout << "Accessing webcam:" << endl;

			VideoCapture capture( 0 );
			Mat frame;
			if( capture.grab() )
			{
				while( true )
				{
					capture >> frame;

					//-- 3. Apply the classifier to the frame
					if( !frame.empty() )
					{
						vector<Rect> faces;
						Mat frame_gray;

						cvtColor( frame , frame_gray , CV_BGR2GRAY );

						face_cascade.detectMultiScale( frame_gray , faces , 1.1 , 2 , 0 | CV_HAAR_SCALE_IMAGE , Size( 30 , 30 ) );

						for( size_t i = 0; i < faces.size(); i++ )
						{
							Point center( faces[ i ].x + faces[ i ].width*0.5 , faces[ i ].y + faces[ i ].height*0.5 );
							ellipse( frame , center , Size( faces[ i ].width*0.5 , faces[ i ].height*0.5 ) , 0 , 0 , 360 , Scalar( 0 , 200 , 255 ) , 4 , 8 , 0 );

							Mat faceROI = frame_gray( faces[ i ] );
							std::vector<Rect> eyes;
						}
						imshow( "Webcam" , frame );
					}
					else
					{
						destroyWindow( "Webcam" );
						printf( " --(!) No captured frame -- Break!" ); 
						break;
					}

					int c = waitKey( 10 );
					if( (char)c == 'c' ) 
					{
						destroyWindow( "Webcam" );
						break; 
					}
				}
			}

		}

		else if( in != 0 )
			continue;
	} while( in != 0 );


	return 0;
}

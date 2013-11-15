
#include <iostream>
#include <opencv\cv.h>
#include <opencv2\highgui.hpp>

using namespace std;
using namespace cv;

int bw=1, gw=1, rw=1;

void onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == EVENT_LBUTTONDOWN) {
		Mat frame;
		frame = *((Mat*)param);

		//cout << frame.at<Vec3b>(y,x);
		bw = frame.at<Vec3b>(y,x)[0];
		gw = frame.at<Vec3b>(y,x)[1];
		rw = frame.at<Vec3b>(y,x)[2];

		Mat picker(100, 100, CV_8UC3, frame.at<Vec3b>(y,x));
		imshow("picker", picker);
	}
}

void _onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == EVENT_LBUTTONDOWN) {
		Mat frame;
		frame = *((Mat*)param);

		cout << frame.at<Vec3b>(y,x);
	}
}


Mat canvas(480, 640, CV_8UC3);
Mat image;
VideoCapture cam;

int main()
{
	cam.open(0);
	/*(16:9) = (1920, 1080), (1280, 720)*/
	/*(4:3) = (640, 480), (320, 240)*/
	//cam.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	//cam.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	//cam.set(CV_CAP_PROP_FPS, 30);

	while(true)
	{
		//cam >> image;
		Mat image = imread("C:\\Users\\bui\\Desktop\\colorchecker_expose.jpg");

		imshow("image", image);
		char c = waitKey(10);

		imshow("canvas", canvas);

		for (int i=0; i<image.rows; i++)
			for (int k=0; k<image.cols; k++) {
				canvas.at<Vec3b>(i,k)[0] = (image.at<Vec3b>(i,k)[0] * 216 / bw) > 255 
												? 255 : image.at<Vec3b>(i,k)[0] * 216 / bw;
				canvas.at<Vec3b>(i,k)[1] = (image.at<Vec3b>(i,k)[1] * 211 / gw) > 255  
												? 255 : image.at<Vec3b>(i,k)[1] * 211 / gw;
				canvas.at<Vec3b>(i,k)[2] = (image.at<Vec3b>(i,k)[2] * 210 / rw) > 255  
												? 255 : image.at<Vec3b>(i,k)[2] * 210 / rw;
			}
		
		setMouseCallback("image", onMouse, &image);
		setMouseCallback("canvas", _onMouse, &canvas);

		if( c == 27 ) {
			break;
		}
	}

	cam.release();

	return 0;
}
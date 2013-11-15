
#include <iostream>
#include <opencv\cv.h>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace std;
using namespace cv;

int hw=1, sw=1, vw=1;

void onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == EVENT_LBUTTONDOWN) {
		Mat frame;
		frame = *((Mat*)param);
		
		cvtColor(frame, frame, CV_BGR2HSV);
		hw = frame.at<Vec3b>(y,x)[0];
		sw = frame.at<Vec3b>(y,x)[1];
		vw = frame.at<Vec3b>(y,x)[2];

		Mat picker(100, 100, CV_8UC3, frame.at<Vec3b>(y,x));
		cvtColor(picker, picker, CV_HSV2BGR);
		imshow("picker", picker);

		cout << frame.at<Vec3b>(y,x);
	}
}

void _onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == EVENT_LBUTTONDOWN) {
		Mat frame;
		frame = *((Mat*)param);
		cvtColor(frame, frame, CV_BGR2HSV);
		cout << frame.at<Vec3b>(y,x);
	}
}


Mat canvas(480, 640, CV_8UC3);
Mat image, _image;
VideoCapture cam;

int main()
{
	cam.open(0);

	while(true)
	{
		//cam >> image;
		Mat image = imread("C:\\Users\\bui\\Desktop\\colorchecker_original.jpg");

		imshow("image", image);
		cvtColor(image, _image, CV_BGR2HSV);

		//cout << _image.at<Vec3b>(284,301) << endl;
		//cout << hw << ", " << sw << ", " << vw << endl;
		for (int i=0; i<image.rows; i++)
			for (int k=0; k<image.cols; k++) {
			
			//	canvas.at<Vec3b>(i,k)[0] = _image.at<Vec3b>(i,k)[0];
			//	canvas.at<Vec3b>(i,k)[1] = _image.at<Vec3b>(i,k)[1];
			//	canvas.at<Vec3b>(i,k)[2] = _image.at<Vec3b>(i,k)[2];
			
				if (hw==0) {
					canvas.at<Vec3b>(i,k)[0] =  _image.at<Vec3b>(i,k)[0];
				} else {
					canvas.at<Vec3b>(i,k)[0] = (_image.at<Vec3b>(i,k)[0] * 50 / hw) > 180  
												? 180 : _image.at<Vec3b>(i,k)[0] * 50 / hw;
				}
				if (sw==0) {
					canvas.at<Vec3b>(i,k)[1] = _image.at<Vec3b>(i,k)[1];
				} else {
					canvas.at<Vec3b>(i,k)[1] = (_image.at<Vec3b>(i,k)[1] * 124 / sw) > 255  
												? 255 : _image.at<Vec3b>(i,k)[1] * 124 / sw;
				}
				if (vw==0) {
					canvas.at<Vec3b>(i,k)[2] = _image.at<Vec3b>(i,k)[2];
				}
				else {
					canvas.at<Vec3b>(i,k)[2] = (_image.at<Vec3b>(i,k)[2] * 148 / vw) > 255  
												? 255 : _image.at<Vec3b>(i,k)[2] * 148 / vw;
				}
			
			}
		cvtColor(canvas, canvas, CV_HSV2BGR);
		imshow("canvas", canvas);
		
		setMouseCallback("image", onMouse, &image);
		setMouseCallback("canvas", _onMouse, &canvas);

		char c = waitKey(10);
		if( c == 27 ) {
			break;
		}
	}

	cam.release();

	return 0;
}
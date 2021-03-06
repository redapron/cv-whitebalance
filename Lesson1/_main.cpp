
#include <iostream>
#include <opencv\cv.h>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace std;
using namespace cv;

int hw=1, sw=1, vw=1;

int lagrange(int ask){
	int n=4;
	double x[10] = {1, 2, 3};
	double fx[10] = {1, 8, 27};
	double num, den;
	double ans=0;
	for (int i=0; i<n; i++){
		num=1;
		den=1;
		for (int k=0; k<n; k++){
			if (i!=k){
				num *= ask-x[k];
				den *= x[i]-x[k];
			}
		}
		ans += fx[i]*num/den;
	}
	return ans;
}

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

		cout << frame.at<Vec3b>(y,x) << endl;
	}
}

void _onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == EVENT_LBUTTONDOWN) {
		Mat frame;
		frame = *((Mat*)param);
		cvtColor(frame, frame, CV_BGR2HSV);
		cout << frame.at<Vec3b>(y,x) << endl;
	}
}


Mat canvas(480, 640, CV_8UC3);
Mat image, _image;
VideoCapture cam;

int main()
{
	cam.open(0);
	if (!cam.isOpened()) {
		cout << "not found camera" << endl;
		return 1;
	}

	while(true)
	{
		//make sure there is image to use otherwise will break
		//cam >> image;
		Mat image = imread("C:\\Users\\bui\\Desktop\\_000-sensor\\colorchecker_original.jpg");

		imshow("image", image);
		cvtColor(image, _image, CV_BGR2HSV);

		for (int i=0; i<image.rows; i++)
			for (int k=0; k<image.cols; k++) {
			
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
			cout << lagrange(8) << endl;
			break;
		}
	}

	cam.release();

	return 0;
}
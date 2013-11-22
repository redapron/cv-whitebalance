
#include <iostream>
#include <fstream>
#include <time.h>

#include <opencv\cv.h>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d\calib3d.hpp>

using namespace std;
using namespace cv;

VideoCapture cam;
Mat canvas(480, 640, CV_8UC3);
Mat _image;
Mat image;
Mat hsvImg;

int hw=1, sw=1, vw=1;				// input color
int static hc=54, sc=115, vc=124;	// real color
int thres[12][6] = {	{157, 158, 50, 255, 50, 255}, // 0
						{146, 148, 50, 255, 50, 255},  
						{135, 137, 50, 255, 50, 255},
						{97, 99, 50, 255, 50, 255}, 
						{100, 102, 50, 255, 50, 255}, // 4
						{92, 94, 50, 255, 50, 255},	  // 5
						{84, 86, 50, 255, 50, 255}, 
						{52, 54, 50, 255, 50, 255}, 
						{23, 25, 50, 255, 50, 255},
						{17, 19, 50, 255, 50, 255},	  // 9
						{6, 8, 50, 255, 50, 255}, 
						{0, 3, 50, 255, 50, 255}
				   };
int currColor = 0;
int center[12][2];
int pixelCnt[12];


void onMouse(int event, int x, int y, int flags, void* param)
{
	Mat frame;
	frame = *((Mat*)param);
	cvtColor(frame, frame, CV_BGR2HSV);
	if (event == EVENT_LBUTTONDBLCLK) {
		hw = frame.at<Vec3b>(y,x)[0];
		sw = frame.at<Vec3b>(y,x)[1];
		vw = frame.at<Vec3b>(y,x)[2];
	}

	if (event == EVENT_LBUTTONDOWN) {
		Mat picker(100, 100, CV_8UC3, frame.at<Vec3b>(y,x));
		cvtColor(picker, picker, CV_HSV2BGR);
		imshow("picker", picker);
		cout << frame.at<Vec3b>(y,x) << endl;	// color in hsv
		//cout << x << ", " << y << endl;			// coordinate x,y
	}
}

/**
	cvt image to _image
	process _image to canvas
	then put canvas to hsvImg
*/
void whiteBalance(){

	cvtColor(image, _image, CV_BGR2HSV);

	for (int i=0; i<_image.rows; i++)
		for (int k=0; k<_image.cols; k++) {
			
			if (hw==0) {
				canvas.at<Vec3b>(i,k)[0] =  _image.at<Vec3b>(i,k)[0];
			} else {
				canvas.at<Vec3b>(i,k)[0] = (_image.at<Vec3b>(i,k)[0] * hc / hw) > 180 ? 180 : _image.at<Vec3b>(i,k)[0] * hc / hw;
			}
			if (sw==0) {
				canvas.at<Vec3b>(i,k)[1] = _image.at<Vec3b>(i,k)[1];
			} else {
				canvas.at<Vec3b>(i,k)[1] = (_image.at<Vec3b>(i,k)[1] * sc / sw) > 255 ? 255 : _image.at<Vec3b>(i,k)[1] * sc / sw;
			}
			if (vw==0) {
				canvas.at<Vec3b>(i,k)[2] = _image.at<Vec3b>(i,k)[2];
			}
			else {
				canvas.at<Vec3b>(i,k)[2] = (_image.at<Vec3b>(i,k)[2] * vc / vw) > 255 ? 255 : _image.at<Vec3b>(i,k)[2] * vc / vw;
			}
		}
	hsvImg=canvas;
}


/**
	find center of each color
	center[k][0], center[k][1] is y and x respectively
*/
void countingPixelPaintItAndFindItCenter(){
	for(int k = 0; k < 12; k++)
		{
			pixelCnt[k] = 0;
			center[k][0] = center[k][1] = 0;
		}

		// HSV
		for(int i = 0; i < hsvImg.rows; i++)
		{
			for(int j = 0; j < hsvImg.cols; j++)
			{
				int h = hsvImg.at<Vec3b>(i, j)[0];
				int s = hsvImg.at<Vec3b>(i, j)[1];
				int v = hsvImg.at<Vec3b>(i, j)[2];

				for(int k = 0; k < 12; k++)
				{
					if( thres[k][0] <= h && h <= thres[k][1] &&
						thres[k][2] <= s && s <= thres[k][3] &&
						thres[k][4] <= v && v <= thres[k][5] )
					{
						hsvImg.at<Vec3b>(i, j)[0] = (thres[k][0] + thres[k][1])/2;
						hsvImg.at<Vec3b>(i, j)[1] = 255;
						hsvImg.at<Vec3b>(i, j)[2] = 255;

						center[k][0] += j;
						center[k][1] += i;
						pixelCnt[k]++;
					}
				}
			}
		}
}

/**
	print pixel counting number for each color
*/
void printPixelNumberAtItCenter(){
	for(int k = 0; k < 12; k++)
		{
			if( pixelCnt[k] > 0 )
			{
				char text[200];
				//sprintf(text, "%d = %d", k, pixelCnt[k]);
				sprintf(text, "%d", k);
				//cout << "text : " << text << endl;

				center[k][0] /= pixelCnt[k];
				center[k][1] /= pixelCnt[k];

				putText(hsvImg, text, Point(center[k][0], center[k][1]), CV_FONT_HERSHEY_PLAIN, 2.0, CV_RGB(0, 0, 0),2, 2);
			}
		}
}

/**

*/
void checkForKeyboardInput(char c){
	if( c == '1' )	currColor = 0;
	if( c == '2' )	currColor = 1;
	if( c == '3' )	currColor = 2;
	if( c == '4' )	currColor = 3;
	if( c == '5' )	currColor = 4;
	if( c == '6' )	currColor = 5;
	if( c == '7' )	currColor = 6;
	if( c == '8' )	currColor = 7;
	if( c == '9' )	currColor = 8;
	if( c == '0' )	currColor = 9;
	if( c == '-' )	currColor = 10;
	if( c == '=' )	currColor = 11;

	switch(c)
	{
	case 'a': thres[currColor][0]++; break;
	case 'z': thres[currColor][0]--; break;
	case 's': thres[currColor][1]++; break;
	case 'x': thres[currColor][1]--; break;
	case 'd': thres[currColor][2]++; break;
	case 'c': thres[currColor][2]--; break;
	case 'f': thres[currColor][3]++; break;
	case 'v': thres[currColor][3]--; break;
	case 'g': thres[currColor][4]++; break;
	case 'b': thres[currColor][4]--; break;
	case 'h': thres[currColor][5]++; break;
	case 'n': thres[currColor][5]--; break;
	}

	cout << "color " << currColor << " : ";
	for(int i = 0; i < 6; i++)
		cout << thres[currColor][i] << " ";
	cout << endl;

	if( c == '6' ) // Save
	{
		ofstream record("../data/color.txt");
		for(int i = 0; i < 12; i++)
		{
			for(int j = 0; j < 6; j++)
				record << thres[i][j] << " ";
			record << endl;
		}
		record.close();
	}

	if( c == '5' ) // Load
	{
		ifstream file("../data/color.txt");
		for(int i = 0; i < 12; i++)
			for(int j = 0; j < 6; j++)
				file >> thres[i][j];
		file.close();
	}
}

// for drawing a cube
Mat intrinsics, distortion;
Mat rt, cubeCoor[4];
vector<Point2f> imagePoint;
vector<Point3f> worldPoint;

vector<double> rv(3), tv(3);
Mat rotateVector(rv), translateVector(tv);
/**
	
*/
void drawCube_init(){
	FileStorage fs("C:\\Users\\bui\\Desktop\\camera.yml", FileStorage::READ);
	fs["camera_matrix"] >> intrinsics;
	fs["distortion_coefficients"] >> distortion;

	worldPoint.push_back(Point3f(0.0f, 0.0f, 0.0f));
	worldPoint.push_back(Point3f(0.0f, 1.0f, 0.0f));
	worldPoint.push_back(Point3f(1.0f, 0.0f, 0.0f));
	worldPoint.push_back(Point3f(1.0f, 1.0f, 0.0f));

	cubeCoor[0] = (Mat_<double>(4,1) << 0, 0, 1, 1); 
	cubeCoor[1] = (Mat_<double>(4,1) << 0, 1, 1, 1);
	cubeCoor[2] = (Mat_<double>(4,1) << 1, 0, 1, 1);
	cubeCoor[3] = (Mat_<double>(4,1) << 1, 1, 1, 1);
}

Mat rotateMatrix;
double *r, *t;

void drawCube_solvePnP(){
	imagePoint.clear();
	imagePoint.push_back(Point2f(center[5][0], center[5][1]));
	imagePoint.push_back(Point2f(center[7][0], center[7][1]));
	imagePoint.push_back(Point2f(center[0][0], center[0][1]));
	imagePoint.push_back(Point2f(center[4][0], center[4][1]));

	/*
	imagePoint.push_back(Point2f(166, 335));
	imagePoint.push_back(Point2f(244, 275));
	imagePoint.push_back(Point2f(264, 388));
	imagePoint.push_back(Point2f(333, 325));
	*/
	solvePnP(Mat(worldPoint), Mat(imagePoint), intrinsics, distortion, rotateVector, translateVector, false, CV_ITERATIVE);
}

void drawCube_draw(){
	Rodrigues(rotateVector,rotateMatrix);
	r = rotateMatrix.ptr<double>();
	t = translateVector.ptr<double>();
	rt = (Mat_<double>(3,4) << r[0], r[1], r[2], t[0], r[3], r[4], r[5], t[1], r[6], r[7], r[8], t[2]);

	for (int k=0; k<4; k++){

		Mat ans = intrinsics * rt * cubeCoor[k];
		double* ans_array = ans.ptr<double>();
		ans_array[0] = ans_array[0]/ans_array[2];
		ans_array[1] = ans_array[1]/ans_array[2];
		ans_array[2] = ans_array[2]/ans_array[2];
		Mat newAns = (Mat_<double>(3,1) << ans_array[0], ans_array[1], ans_array[2]);

		Point newPoint = Point(ans_array[0],ans_array[1]);
		imagePoint.push_back(newPoint);

		/*
		// debug
		cout << "intrinsics" << endl << intrinsics << endl << endl;
		cout << "rotate" << endl << rotateMatrix << endl << endl;
		cout << "translate" << endl << translateVector << endl << endl;
		cout << "rt" << endl << rt << endl << endl;
		cout << "ans" << endl << ans << endl << endl;
		cout << "newAns" << endl << newAns << endl << endl;
		*/
	}

	for (int k=0; k<8; k++){
		circle(image, imagePoint[k], 5, Scalar(0,222,0),-1);
		line(image, imagePoint[k], imagePoint[(k+1)%8], Scalar(0,222,0),2);
	}
}

int main()
{
	cam.open(0);

	drawCube_init();

	while(true)
	{
		//cam >> image;
		image = imread("C:\\Users\\bui\\Desktop\\_000-sensor\\cv-whitebalance\\data\\image1384353593.bmp");
		setMouseCallback("image", onMouse, &image);
		
		// cvt image to hsvImg
		cvtColor(image, hsvImg, CV_BGR2HSV);

		// process image and put output to hsvImg
		//whiteBalance();

		countingPixelPaintItAndFindItCenter();
		cvtColor(hsvImg, hsvImg, CV_HSV2BGR);
		printPixelNumberAtItCenter();

		drawCube_solvePnP();
		drawCube_draw();

		imshow("hsvImg", hsvImg);
		imshow("image", image);

		char c = waitKey(10);
		if( c == 27 )
			break;
		//checkForKeyboardInput(c);
	}
	cam.release();

	return 0;
}
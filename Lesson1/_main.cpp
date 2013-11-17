
#include <iostream>
#include <fstream>
#include <time.h>

#include <opencv\cv.h>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace std;

cv::Mat image;
cv::Mat hsvImg;

cv::VideoCapture cam;

int thres[3][6] = {{0, 20, 50, 255, 50, 255}, {40, 80, 50, 255, 50, 255}, {100, 140, 50, 255, 50, 255}};
int currColor = 0;

int center[3][2];
int pixelCnt[3];

int main()
{
	int startTime = clock();

	cam.open(0);
	//cam.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cam.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	//cam.set(CV_CAP_PROP_FPS, 30);

	while(true)
	{
		//cout << "time : " << clock() - startTime << endl;
		//startTime = clock();

		//cam >> image;
		image = cv::imread("C:\\Users\\bui\\Desktop\\_000-sensor\\cv-whitebalance\\data\\image1384353593.bmp");
		cv::cvtColor(image, hsvImg, CV_BGR2HSV);
		cv::imshow("image", image);

		for(int k = 0; k < 3; k++)
		{
			pixelCnt[k] = 0;
			center[k][0] = center[k][1] = 0;
		}

		// HSV
		for(int i = 0; i < hsvImg.rows; i++)
		{
			for(int j = 0; j < hsvImg.cols; j++)
			{
				int h = hsvImg.at<cv::Vec3b>(i, j)[0];
				int s = hsvImg.at<cv::Vec3b>(i, j)[1];
				int v = hsvImg.at<cv::Vec3b>(i, j)[2];

				for(int k = 0; k < 3; k++)
				{
					if( thres[k][0] <= h && h <= thres[k][1] &&
						thres[k][2] <= s && s <= thres[k][3] &&
						thres[k][4] <= v && v <= thres[k][5] )
					{
						hsvImg.at<cv::Vec3b>(i, j)[0] = (thres[k][0] + thres[k][1])/2;
						hsvImg.at<cv::Vec3b>(i, j)[1] = 255;
						hsvImg.at<cv::Vec3b>(i, j)[2] = 255;

						center[k][0] += j;
						center[k][1] += i;
						pixelCnt[k]++;
					}
				}
			}
		}

		cv::cvtColor(hsvImg, hsvImg, CV_HSV2BGR);

		for(int k = 0; k < 3; k++)
		{
			if( pixelCnt[k] > 0 )
			{
				char text[200];
				sprintf(text, "%d = %d", k, pixelCnt[k]);
				cout << "text : " << text << endl;

				center[k][0] /= pixelCnt[k];
				center[k][1] /= pixelCnt[k];

				cv::putText(hsvImg, text, cv::Point(center[k][0], center[k][1]), CV_FONT_HERSHEY_PLAIN, 2.0, CV_RGB(0, 0, 0),2, 2);
			}
		}
		cv::imshow("hsvImg", hsvImg);

		char c = cv::waitKey(10);

		if( c == 27 )
			break;

		if( c == '1' )	currColor = 0;
		if( c == '2' )	currColor = 1;
		if( c == '3' )	currColor = 2;

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
			for(int i = 0; i < 3; i++)
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
			for(int i = 0; i < 3; i++)
				for(int j = 0; j < 6; j++)
					file >> thres[i][j];
			file.close();
		}
	}

	cam.release();

	return 0;
}
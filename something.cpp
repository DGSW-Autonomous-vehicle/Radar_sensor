#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <deque>

using namespace cv;
using namespace std;

RNG rng(12345);
char mode = 1;  //debug mode    0 = disable  1 = enable
int tail = 10;   //tail on object in debug
int flag_bird = 0;  //exist bird
Rect min_birdsize = Rect(0, 0, 20, 20);    //0 ,0, min width pixel , min height pixel

Rect ContoursNBoxing(Mat src);
Mat MovingMat(Mat src, Mat pre_src);
bool Find_nearby();

int main() {
	//init
	Mat frame;
	Mat pre_frame;
	Mat res;
	Mat cam;
	Rect TRect;
	Point2f center;
	Point cent = Point(0, 0);
	deque<Point> cents;
	Mat prev;
	Mat res2;
	HOGDescriptor hog;
	for (int t = 0; t < tail; t++)
		cents.push_back(Point(0, 0));
	float radius;

	//frame init
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	VideoCapture video(0);
	video >> frame;
	cvtColor(frame, frame, COLOR_BGR2GRAY);

	while (true) {
		//get frame
		frame.copyTo(pre_frame);
		video >> frame;
		if (mode)
			frame.copyTo(cam);
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		GaussianBlur(frame, frame, Size(5, 5), 1);

		res = MovingMat(frame, pre_frame);   //detect moving

		res2 = res | prev;

		dilate(res2, res2, Mat(), cv::Point(-1, -1), 5);
		

		TRect = ContoursNBoxing(res2);	//Contours & Boxing
		cent = ((TRect.br() + TRect.tl()) * 0.5); //get result center
		
		res.copyTo(prev);

		//debug
		if (mode) {
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			cents.pop_front();
			cents.push_back(cent);
			rectangle(res2, TRect, color, 2);
			
			//erode(res2, res2, Mat());
			//morphologyEx(res2, res2, 3, Mat()); 

			//Mat roi = res2(TRect);
			
			//if(!roi.empty())
				imshow("debug", res2);

		}
		if (waitKey(50) == 27) {
			break;
		}


	}
	return 0;
}

Mat MovingMat(Mat src, Mat pre_src) {
	Mat res;
	Mat T1;
	Mat T2;
	blur(src, src, Size(5, 5));
	T1 = pre_src - src;
	T2 = src - pre_src;
	res = T1 + T2;
	threshold(res, res, 7, 255, THRESH_BINARY);

	return res;
}

Rect ContoursNBoxing(Mat src) {
	
	//Contours & Boxing
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	Rect TRect = Rect(Point(0, 0), Point(0, 0));

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		//convexHull(contours[i], hull[i]);
		
		if ((TRect.height * TRect.width) < (boundingRect(Mat(contours_poly[i])).height * boundingRect(Mat(contours_poly[i])).width)) {
			TRect = boundingRect(Mat(contours_poly[i]));
			drawContours(src, contours_poly, i, cv::Scalar(255, 0, 0));
		}
	}

	return TRect;
}
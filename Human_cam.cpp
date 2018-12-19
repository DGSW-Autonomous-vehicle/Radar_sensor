//#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace cv;
using namespace std;

char mode = 1;  //debug mode    0 = disable  1 = enable
char human_flag = 0;

Rect min_birdsize = Rect(0, 0, 20, 20);    //0 ,0, min width pixel , min height pixel

Rect ContoursNBoxing(Mat src);
Mat MovingMat(Mat src, Mat pre_src);
vector<Rect> is_Human(Rect TRect, Mat cam);

int main(){
	FILE *fp;
	Rect TRect;

	Mat frame;
	Mat pre_frame;
	Mat res;
	Mat cam;
	Mat prev;
	Mat res2;

	Point2f center;
	Point cent = Point(0, 0);
	vector<Rect> found;
	
	VideoCapture video(0);
	video >> frame;
	cvtColor(frame, frame, COLOR_BGR2GRAY);

	char ch = '0';

	while (true){
		//init
		TRect = Rect(Point(0, 0), Point(0, 0));
		//get frame
		frame.copyTo(pre_frame);
		video >> frame;

		frame.copyTo(cam);
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		GaussianBlur(frame, frame, Size(5, 5), 1);

		res = MovingMat(frame, pre_frame);   //detect moving

		res2 = res | prev;

		dilate(res2, res2, Mat(), cv::Point(-1, -1), 5);

		TRect = ContoursNBoxing(res2);	//Contours & Boxing
		cent = ((TRect.br() + TRect.tl()) * 0.5); //get result center

		res.copyTo(prev);

		found = is_Human(TRect, cam);

		if (mode) {
			for (int i = 0; i < found.size(); i++) {//draw rectangle
				cout << "detected!!!!!!!!!!!!!!! " << endl;
				rectangle(res2, found[i].tl(), found[i].br(), Scalar(0, 255, 0), 3);
				rectangle(cam, found[i].tl(), found[i].br(), Scalar(0, 255, 0), 3);
			}
			rectangle(cam,TRect, Scalar(255, 0, 255), 1);
			rectangle(res2, TRect, Scalar(255, 255, 255), 2);

			imshow("move", res2);
			//imshow("opencv", cam);
		}
		ch = char(human_flag + '0');
		fp = fopen("human.txt", "w");
		putc(ch, fp);
		fclose(fp);
		imwrite("image.jpg",cam); 

		waitKey(10);
	}
	return 0;
}

vector<Rect> is_Human(Rect TRect, Mat cam) {
	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	Mat hog_roimat;
	Mat small_roi;
	vector<Rect> found;

	if ((TRect.height < 120 || TRect.width < 80 || TRect.width > 250)) {
		//cout << "not found";
		human_flag = 0;
	}
	else {
		human_flag = 1;
		//cout << TRect.height << endl;
		//cout << TRect.width << endl;

		hog_roimat = Mat(0, 0, CV_8UC1, Scalar(0, 0, 0));
		hog_roimat = cam(TRect);
		small_roi = Mat(0, 0, CV_8UC1, Scalar(0, 0, 0));
		//cout << TRect.tl() << endl;
		//cout << TRect.br() << endl;

		resize(hog_roimat, small_roi, cv::Size(hog_roimat.cols / 2, hog_roimat.rows / 2), 0, 0, CV_INTER_NN);
		hog.detectMultiScale(small_roi, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
		for (int i = 0; i < found.size(); i++) {//draw rectangle
			human_flag = 2;
		}
	}
	return found;
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

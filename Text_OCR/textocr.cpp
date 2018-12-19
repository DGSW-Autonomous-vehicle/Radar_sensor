#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <ctime>

using namespace cv;
using namespace std;

int main(void) {

	Mat mask = getStructuringElement(MORPH_RECT,Size(3, 3), Point(1, 1));
	
	int readBtn;
	String command;
	String fileName;
	String fileName_ext;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int count = 1;
	
	time_t c_time;
	struct tm *tm;
	c_time = time(NULL);
	tm = localtime(&c_time);

	while(1) {
			Mat image;
			Mat cannyimage;
			Mat contourimage;
			Mat notimage;
			Mat dilatedimage;
			fileName = "image_" + to_string(count);
			fileName_ext = fileName + ".jpg";
			command = "cd image/ && raspistill -t 1 -o " + fileName_ext;
			system(command.c_str());
			usleep(5000);
			count++;
			image = imread("image/" + fileName_ext, IMREAD_GRAYSCALE);
			/*Canny(image, cannyimage, 150, 250, 3);
			notimage = cannyimage;
			findContours(notimage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
			drawContours(notimage, contours, -1, Scalar(255), 3);
			dilate(notimage, dilatedimage, mask);
			findContours(dilatedimage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
			vector<vector<Point>> contour_roi(contours.size());
			vector<Rect> boundRect(contours.size());
			Rect TRect = Rect(Point(0, 0), Point(0, 0));
			int tl_x = 9999999, tl_y = 9999999;
			int br_x = 0, br_y = 0;

			for (int i = 0; i < contours.size(); i++) {
				approxPolyDP(Mat(contours[i]), contour_roi[i], 3, true);
				boundRect[i] = boundingRect(Mat(contour_roi[i]));
			}	
			for (int i = 0; i < contours.size(); i++) {
				int brx = boundRect[i].br().x;
				int bry = boundRect[i].br().y;
				int tlx = boundRect[i].tl().x;
				int tly = boundRect[i].tl().y;
				if (tlx < tl_x)
					tl_x = tlx;
				if (tly < tl_y)
					tl_y = tly;
				if (brx > br_x)
					br_x = brx;
				if (bry > br_y)
					br_y = bry;
			}
			cout << tl_x << endl;
			cout << tl_y << endl;
			cout << br_x << endl;
			cout << br_y << endl;
			cout << image.size() << endl;
			Rect roi = Rect(tl_x, tl_y, br_x - tl_x, br_y - tl_y);
			Mat roiimage = image(roi);*/
			imwrite("image/result.jpg",image);
			command = "cd image/ && tesseract result.jpg " + fileName;
		      	system(command.c_str());	
		
	}
	return 0;

}

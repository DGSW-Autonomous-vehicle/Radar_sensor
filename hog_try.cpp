#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>


using namespace cv;
using namespace std;


int main()
{
	bool mode = true;
	Mat img;
	Mat Cam;
	vector<Rect> found;
	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	String command;
	String fileName;
	fileName = "image.jpg";
	command = "cd image/ && raspistill -t 0.1 -o " + fileName;
	system(command.c_str());

	while (true){
		system(command.c_str());
		waitKey(10);
		Cam = imread("image/" + fileName, IMREAD_GRAYSCALE);
		img = Cam.clone();
		//resize(img, img, Size(320, 240));
		hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);

		if (mode) {
			for (int i = 0; i < found.size(); i++)//draw rectangle
				rectangle(Cam, found[i].tl(), found[i].br(), Scalar(0, 255, 0), 2);
			imshow("opencv", Cam);
		}
	}
	return 0;
}

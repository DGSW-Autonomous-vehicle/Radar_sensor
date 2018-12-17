#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <cstring>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture video(0);
	if (!video.isOpened()) {
		cout << "cam_Error" << endl;
		return -1;
	}
	string file;
	Mat frame;

	video.set(CAP_PROP_FPS, 30.0);

	while (true) {

		cout << file << endl;

		while (1)
		{
			video >> frame;
			int fps = video.get(CAP_PROP_FPS);
			int wait = int(1.0 / fps * 1000);

			if (waitKey(wait) == 27) break;

			imwrite("OpenTest.png", frame);
			//imshow("input", frame);

			sleep(wait);
		}
	}

	return 0;
}

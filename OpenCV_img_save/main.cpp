#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <cstring>

using namespace cv;
using namespace std;

int main()
{
	//웹캡으로 부터 데이터 읽어오기 위해 준비  
	VideoCapture video(0);
	if (!video.isOpened()) {
		cout << "카메라 에러" << endl;
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
			imshow("input", frame);

			Sleep(wait);
		}
	}

	return 0;
}
//============================================================================
// Name        : t.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <ACCamera.h>
#include <ACGradient.h>
#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/opencv.hpp>
//#include "highgui.h"


using namespace std;
using namespace cv;
using namespace ac;

int main() {
	Mat src, src_gray;
	/// 装载图像
	src = imread("/Users/qiang/Documents/workspace/t/src//roi11.png");
	if(!src.data)
		cout << "没拿到图。" << endl;

//	Mat grad;
	char* window_name = "Sobel Demo - Simple Edge Detector";

	Gradient grad =Gradient(src);
	vector<Mat> tgr;
	tgr = grad.getGradientValue();
	//cout << " 没停  " << tgr.size() << std::endl;
	for(int i=0;i<tgr.size();i++) {
		/// 创建显示窗口
		namedWindow(window_name, CV_WINDOW_AUTOSIZE);
		cout << " 没停" << endl;

		//imshow(window_name, tgr[i]);
		//wait(0);

	}

	return 0;

}

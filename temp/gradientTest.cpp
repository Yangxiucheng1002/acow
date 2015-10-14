/*
 * gradientTest.cpp
 *
 *  Created on: 2015年10月10日
 *      Author: qiang.liu
 */




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
	src = imread("/Users/qiang/project/temp/e1.jpg");
	if(!src.data) {
		cout << "没拿到图。" << endl;
		return 0;
	}

//	Mat grad;
	char* window_name = "Sobel Demo - Simple Edge Detector";

//	cout << " 0" << endl;
	cout << " 1 " << endl;
	Gradient grad =Gradient(src);

	//如果temp1里没有数据
	if(!grad.temp1.data)
		cout << " temp1里没有数据 " << endl;
	else
		cout << " === " << endl;

	cout << " =1= " << endl;
	//如果temp2里没有数据
	Mat t = *(grad.temp2);
	cout << " =2= " << endl;
	if(!t.data)
		cout << " temp2里没有数据 " << endl;
	else
		cout << " temp2里有数据 " << endl;
	cout << " =3= " << endl;

//	cout << grad._gradientValue.size() << " c " << endl;
	vector<Mat> tgr;
//	cout << " 1" << endl;
	tgr = grad.getGradientValue();
	cout << " 没停  " << tgr.size() << std::endl;
	for(int i=0;i<tgr.size();i++) {
		/// 创建显示窗口
		namedWindow(window_name, CV_WINDOW_AUTOSIZE);
		cout << " 没停" << endl;

		//imshow(window_name, tgr[i]);
		//wait(0);

	}

	return 0;

}



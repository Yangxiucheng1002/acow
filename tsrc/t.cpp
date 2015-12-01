//============================================================================
// Name        : t.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : 最小二乘法拟合曲线测试。
//============================================================================

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include "math/ACMath.h"

#include "math/ACSLM.h"


using namespace cv;
using namespace ac;

int main(int argc, char *argv[]) {
	// 输入一张图。
	Mat src;//,tar;
	// 点图
	//src = imread("/users/qiang/project/temp/testSLM_1.jpg");
	src = imread("/Users/qiang/project/temp/t1.png");
	//src.copyTo(tar);tar.setTo(Scalar(0));
	if(src.data) printf("can't load image.");
	Fit fit =  Fit();
	fit.linearFit(src,true);
	int a = fit.getFactor(0);
	int b = fit.getFactor(1);

	//fit.polyfit(src,3,true);

	// 块图

	double t[2] = { };
	if (LineFit(src, t)) {
		for (int i = 0; i < src.cols; i++) {
			//printf("==");
			uchar *p = src.ptr(i);
			int _Y = getY(i, t, 2);
			p[_Y] = 100;
		}
		imwrite("/users/qiang/project/temp/t.jpg", src);
		namedWindow("", CV_WINDOW_AUTOSIZE);
		imshow("",src);
		wait(0);
	}
	return 0;
}

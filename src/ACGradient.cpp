/*
 * Gradient.cpp
 *
 *  Created on: 2015年9月22日
 *      Author: qiang.liu
 */

#include <ACEnv.h>
#include <ACGradient.h>
#include "ACException.h"
//#include "opencv2/opencv.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

namespace ac {

Gradient::Gradient() {
	// TODO Auto-generated constructor stub

}

Gradient::~Gradient() {
	// TODO Auto-generated destructor stub
}

Gradient::Gradient(Mat src) {
	//默认采用索贝尔算法计算梯度
	Gradient(src, AC_GRAD_SOBEL_);
}

Gradient::Gradient(Mat src, int _gmethod) {
	// 不能给空数据。如果有。以报错处理。
	if (!src.data) {
		throw ACException(EXP_NULL_MAT);
	}

	// 默认构造
	Gradient();
	//初始化参数。
	_gradientValue = Mat(src.rows, src.cols, CV_16SC(src.channels())); //+1)); // 增加了一个通道，用来存储和值。不单独列了。
	_gradientSumValue = Mat(src.rows, src.cols, CV_16SC(1));

#ifdef DEBUG_MODE_
	t = (double) getTickCount(); /************ 计时开始 ************/
#endif

	// 高斯模糊，去噪。
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/****************************************************************************************
	 *                  分成RGB+HSL通道检测，再将图像相加                                        *
	 ****************************************************************************************/
	// 将源图切分通道
	vector<Mat> bgr, btr;
	split(src, bgr); // 貌似这里执行出错了，bgr里是空的。


	split(_gradientValue, btr);

	// 根据方法，计算各维度梯度值
	switch (_gmethod) {
	case AC_GRAD_SOBEL_: // 索贝尔算法
		for (int i = 0; i < bgr.size(); i++) {

			Mat _gray = bgr[i];
			if(_gray.data)
				cout << " 确实是这里有问题。" << endl;
			computeGradientBySobel(_gray, btr[i]);

			_gradientSumValue += btr[i]; // 一边分解，一边累加和值。
		}

		break;
	}

	// 计算梯度和值
	// computeGradientSumValue(src); // 这个方法不用了。

#ifdef DEBUG_MODE_
	t = ((double) getTickCount() - t) / getTickFrequency(); /************ 计时结束 ************/
	cout << "Times passed in millenseconds: " << t * 1000 << endl; /************ 计时输出 ************/
#endif

}

void Gradient::computeGradientBySobel(Mat src_gray, Mat grad) {

	// TODO : 检查MAT的值是否正确。
	if(src_gray.data) {
		throw ACException(EXP_NULL_MAT);
	}

	if(src_gray.channels()>1 || grad.channels()>1) {
		throw ACException(EXP_ONLY_ONE_CHANNEL_ACCEPT);
	}

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	/// 创建 grad_x 和 grad_y 矩阵
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// 求 X方向梯度
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// 求Y方向梯度
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// 合并梯度(近似)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

}

cv::Mat Gradient::getGradientSumValue(short threshold) {
	// TODO : 对MAT，将所有元素减去指定的值。
	Mat ret;

	return ret;
}

Mat Gradient::getGradientValue() {
	return _gradientValue;
}

} /* namespace ac */

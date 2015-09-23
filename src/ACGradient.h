/*
 * Gradient.h
 * 梯度。一张图，只单独按灰度取图像梯度值，是一个近似值，在小区域求精细解时，不准确。需要一个精确的求解数据。
 * 这里分别按RGB、HLS计算梯度，再将值累加起来，可以获得更大的增量，再按精度需求，去掉一个梯度阈值，可以得到更精准的边界。
 *
 * 还没去干扰。
 *
 *  Created on: 2015年9月22日
 *      Author: qiang.liu
 */

#include "opencv2/opencv.hpp"

#ifndef GRADIENT_H_
#define GRADIENT_H_

#ifndef DEBUG_MODE_
#define DEBUG_MODE_ true
#endif

/**
 * 梯度计算方法类型，默认为索贝尔算法计算x,y方向，做加权平均。
 *
 * #ifndef AC_GRAD_SOBEL_
 */
#define AC_GRAD_SOBEL_ 0

using namespace cv;

namespace ac {

class Gradient {
private:

#ifdef DEBUG_MODE_
	//如果是调试模式，增加计时功能。
	double t = 0 ;
#endif

	/**
	 * 默认构造，被设置为私有。避免值为空而导致的不一致性。
	 */
	Gradient();

	/**
	 * 用于存储多通道梯度值
	 * Mat M(rows,cols,CV_16S(1));
	 *
	 */
	//Mat _gradientValue;
	vector<Mat> _gradientValue;

	/**
	 * 用于存储各通道梯度的和值
	 * Mat M(rows,cols,CV_16S(1));
	 */
	//int _gradientSumValue[][]; // 这个先不用了。实在需要内存和CPU优化的话，就改这里。
	Mat _gradientSumValue;

	/**
	 * 用索贝尔算法计算一维矩阵的梯度，并将结果保存到_gradientValue中去。
	 * src 源图，一个单通道、8位的矩阵。
	 * grad_Tar 目标，结果梯度，一个单通道、8位的矩阵。
	 */
	Mat computeGradientBySobel(Mat src);
	/**
	 * 计算梯度和值
	 */
	// void computeGradientSumValue(Mat src); // 这个方法不用了。


public:
	virtual ~Gradient();

	/**
	 * 按给定源图像，生成梯度矩阵。
	 */
	Gradient(Mat src);

	/**
	 * 按给定源图像和梯度计算方法，生成梯度矩阵。
	 */
	Gradient(Mat src,int _gmethod);

	/**
	 * 取梯度和值
	 */
	Mat getGradientSumValue(short threshold);


	/**
	 * 取梯度值
	 */
	vector<Mat> getGradientValue();
	Mat getGradientValue(int channel);
};

} /* namespace ac */

#endif /* GRADIENT_H_ */

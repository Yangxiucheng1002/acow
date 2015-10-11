/*
 * ACMath.h
 *
 *  Created on: 2015年10月10日
 *      Author: qiang.liu
 */


#ifndef MATH_ACMATH_H_
#define MATH_ACMATH_H_

#ifndef BOOL
#define BOOL bool
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

//多项式拟合用函数
//#include "opencv2/opencv.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
//

/**
 * 数值计算。求解4X4矩阵，返回解存在b中
 * @param a 乘数矩阵
 * @param b 被乘数矩阵，同时做为返回解。
 * @return 错误码
 */
int GainParam4(double a[][4], double b[]); //


/**
 * 数值计算。求解3X3矩阵, 返回解存在b
 * @param a 乘数矩阵
 * @param b 被乘数矩阵，同时做为返回解。
 * @return 错误码
 */
int GainParam3(double a[][3], double b[]); //


/**
 * 数值计算。求解2X2矩阵, 返回解存在b
 * @param a 乘数矩阵
 * @param b 被乘数矩阵，同时做为返回解。
 * @return 错误码
 */
int GainParam2(double a[][2], double b[]); //

// --------------------------------------------------------------
//a、b分别是X Y坐标点集合，len是录入的坐标点数量.t是个中间过程用变量数组--长度由拟合阶数决定
//TODO:没有做降噪处理。
//TODO:应该将double改为uchar，减小内存，提高运算速度，但会降低准确性。

/**
 * 使用3次二项式拟合曲线
 * @param x x坐标
 * @param y y坐标
 * @param t 中间过程变量数组，长度由拟合除数决定。3次二项式，为4。矩阵计算的结果也保存在这里。
 * @param len 坐标数量
 * @return 是否成功拟合曲线
 */
BOOL LineFit3(double *x, double *y, double t[], int len);
/**
 *
 * @param mask
 * @param t
 * @return
 */
BOOL LineFit3(Mat mask,double t[]);


/**
 * 使用2次二项式拟合曲线
 * @param x x坐标
 * @param y y坐标
 * @param t 中间过程变量数组，长度由拟合除数决定。2次二项式，为3。矩阵计算的结果也保存在这里。
 * @param len 坐标数量
 * @return 是否成功拟合曲线
 */
BOOL LineFit2(double *x, double *y, double t[], int len);
/**
 *
 * @param mask
 * @param t
 * @return
 */
BOOL LineFit2(Mat mask,double t[]);

/**
 * 使用线性拟合曲线
 * @param x x坐标
 * @param y y坐标
 * @param t 中间过程变量数组，长度由拟合除数决定。线性，为2.矩阵计算的结果也保存在这里。
 * @param len 坐标数量
 * @return 是否成功拟合曲线
 */
BOOL LineFit(double *x, double *y, double t[], int len);
/**
 *
 * @param mask
 * @param t
 * @return
 */
BOOL LineFit(Mat mask,double t[]);

/**
 * 根据函数，给定x坐标，计算y坐标。
 * @param x x坐标
 * @param t 函数参数，根据拟合方式不同，数量不同。
 * @return y坐标
 */
int getY(int x,double t[],int len);

#endif /* MATH_ACMATH_H_ */

/*
 * ACMath.h
 *
 *  Created on: 2015年10月10日
 *      Author: qiang.liu
 */


#ifndef BOOL
#define BOOL bool
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif


#ifndef MATH_ACMATH_H_
#define MATH_ACMATH_H_

//.h
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//多项式拟合用函数

//数值计算
int GainParam4(double a[][4], double b[]); //求解4X4矩阵，返回解存在b中
int GainParam3(double a[][3], double b[]); //求解3X3矩阵, 返回解存在b
int GainParam2(double a[][2], double b[]); //求解2X2矩阵, 返回解存在b

//a、b分别是X Y坐标点集合，len是录入的坐标点数量.t是个中间过程用变量数组--长度由拟合阶数决定
BOOL LineFit3(double *a, double *b, double t[], int len); //使用3次二项式拟合曲线
BOOL LineFit2(double *a, double *b, double t[], int len); //使用2次二项式拟合曲线
BOOL LineFit(double *a, double *b, double t[], int len); //使用线性拟合曲线





#endif /* MATH_ACMATH_H_ */

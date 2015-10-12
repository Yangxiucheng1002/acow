/**
 * @file:		ACSLM.cpp
 * @Copyright: 	2015, qiang.liu.
 * @Author: 	qiang.liu
 * @Brief:		最小二乘法，拟合曲线。这里做了直线、二次三次多项式拟合。
 * @Date: 		2015年10月11日
 * @Version:	 
 * @Description: 
 *    
 * -----------------------------
 * @History:
 *    Date: 	2015年10月11日
 *    Version: 	  
 *    Changer: 	qiang.liu
 *    Description:
 *        
 * -----------------------------
 *
 * @declare:	这里要扫描一遍图，消耗一些时间。如果要做性能调整，需要在这里调整。
 */

#include <stdio.h>
#include "ACMath.h"
#include "ACException.h"

//多项式拟合用函数

/**
 * 求解2X2矩阵, 返回解存在b
 * @param a 乘数矩阵
 * @param b 被乘数矩阵。同时做为返回解。
 * @return
 */
int GainParam2(double a[][2], double b[]) {
	double x, y; //
	int i;

	y = a[0][0] * a[1][1] - a[0][1] * a[1][0];
	if (!y) {
		return 1;
	}

	x = a[0][0];
	if (!x) {
		if (!a[1][0]) {
			return 2;
		}
		for (i = 0; i < 2; i++) {
			x = a[0][i];
			a[0][i] = a[1][i];
			a[1][i] = x;
		}
		y = b[0];
		b[0] = b[1];
		b[1] = y;
	}
	x = a[1][0] / a[0][0];
	a[1][1] -= x * a[0][1];
	b[1] -= b[0] * x;
	x = a[0][1] / a[1][1];
	b[0] -= b[1] * x;

	b[0] /= a[0][0];
	b[1] /= a[1][1];

	return 0;

}

int GainParam3(double a[][3], double b[]) {
	double x, y;
	int i, j;
	y = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
			- a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
			+ a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);

	if (!y) {
		return 1;
	}

//行列式对角化

	//第一列挑选非零项
	for (i = 0; i < 3; i++) {
		x = a[i][0];
		if (x)
			break;
	}
	if (i == 3) {
		return 2;
	}
	if (i > 0) {
		j = i;
		for (i = 0; i < 3; i++) {
			x = a[0][i];
			a[0][i] = a[j][i];
			a[j][i] = x;
		}
		y = b[0];
		b[0] = b[j];
		b[j] = y;
	}
	//销零
	x = a[0][0];
	y = a[1][0] / x;
	b[1] -= b[0] * y;
	for (i = 1; i < 3; i++)
		a[1][i] -= a[0][i] * y;
	y = a[2][0] / x;
	b[2] -= b[0] * y;
	for (i = 1; i < 3; i++)
		a[2][i] -= a[0][i] * y;

	//第二列挑选非零项
	for (i = 1; i < 3; i++) {
		x = a[i][1];
		if (x)
			break;
	}
	if (i == 3) {
		return 3;
	}
	if (i > 1) {
		j = i;
		for (i = 0; i < 3; i++) {
			x = a[1][i];
			a[1][i] = a[j][i];
			a[j][i] = x;
		}
		y = b[1];
		b[1] = b[j];
		b[j] = y;
	}

//销零
	x = a[1][1];
	y = a[0][1] / x;
	b[0] -= b[1] * y;
	a[0][2] -= a[1][2] * y;
	y = a[2][1] / x;
	b[2] -= b[1] * y;
	a[2][2] -= a[1][2] * y;

	if (!a[2][2]) {
		return 4;
	}
	x = a[2][2];
	for (i = 0; i < 2; i++) {
		y = a[i][2] / x;
		b[i] -= b[2] * y;
	}

	//求解得到系数:
	for (i = 0; i < 3; i++) {
		b[i] /= a[i][i];
	}

	return 0;
}

int GainParam4(double a[][4], double b[]) {
	double x, y;
	int i, k1;

	x = 0;
	y = 0;
	x += a[1][0] * (a[2][1] * a[3][2] - a[2][2] * a[3][1])
			+ a[1][1] * (a[2][2] * a[3][1] - a[2][0] * a[3][2])
			+ a[1][2] * (a[2][0] * a[3][1] - a[2][1] * a[3][0]);
	y += -x * a[0][3];
	x = a[1][1] * (a[2][2] * a[3][3] - a[2][3] * a[3][2])
			+ a[1][2] * (a[2][3] * a[3][1] - a[2][1] * a[3][3])
			+ a[1][3] * (a[2][1] * a[3][2] - a[2][2] * a[3][1]);
	y += a[0][0] * x;
	x = a[1][0] * (a[2][2] * a[3][3] - a[2][3] * a[3][2])
			- a[1][2] * (a[2][0] * a[3][3] - a[2][3] * a[3][0])
			+ a[1][3] * (a[2][0] * a[3][2] - a[2][2] * a[3][0]);
	y += -x * a[0][1];
	x = a[1][0] * (a[2][2] * a[3][3] - a[2][3] * a[3][1])
			+ a[1][1] * (a[2][3] * a[3][0] - a[2][0] * a[3][3])
			+ a[1][3] * (a[2][0] * a[3][1] - a[2][1] * a[3][0]);
	y += x * a[0][2];

	if (!y) {
		return 1;
	}

//行列式对角化

	//第一列挑选非零项
	for (i = 0; i < 4; i++) {
		x = a[i][0];
		if (x)
			break;
	}
	if (i == 4) {
		return 2;
	}
	if (i > 0) {
		k1 = i;
		for (i = 0; i < 4; i++) {
			x = a[0][i];
			a[0][i] = a[k1][i];
			a[k1][i] = x;
		}
		y = b[0];
		b[0] = b[k1];
		b[k1] = y;
	}

//销零
	x = a[0][0];
	y = a[1][0] / x;
	b[1] -= b[0] * y;
	for (i = 1; i < 4; i++)
		a[1][i] -= a[0][i] * y;
	y = a[2][0] / x;
	b[2] -= b[0] * y;
	for (i = 1; i < 4; i++)
		a[2][i] -= a[0][i] * y;
	y = a[3][0] / x;
	b[3] -= b[0] * y;
	for (i = 1; i < 4; i++)
		a[3][i] -= a[0][i] * y;

	//第二列挑选非零项
	for (i = 1; i < 4; i++) {
		x = a[i][1];
		if (x)
			break;
	}
	if (i == 4) {
		return 3;
	}
	if (i > 1) {
		k1 = i;
		for (i = 0; i < 4; i++) {
			x = a[1][i];
			a[1][i] = a[k1][i];
			a[k1][i] = x;
		}
		y = b[1];
		b[1] = b[k1];
		b[k1] = y;
	}

//销零
	x = a[1][1];
	y = a[0][1] / x;
	b[0] -= b[1] * y;
	for (i = 2; i < 4; i++)
		a[0][i] -= a[1][i] * y;
	y = a[2][1] / x;
	b[2] -= b[1] * y;
	for (i = 2; i < 4; i++)
		a[2][i] -= a[0][i] * y;
	y = a[3][1] / x;
	b[3] -= b[1] * y;
	for (i = 2; i < 4; i++)
		a[3][i] -= a[0][i] * y;

//第三列
	for (i = 2; i < 4; i++) {
		x = a[i][2];
		if (x)
			break;
	}
	if (i == 4) {
		return 4;
	}
	if (i > 2) {
		k1 = i;
		for (i = 0; i < 4; i++) {
			x = a[2][i];
			a[2][i] = a[k1][i];
			a[k1][i] = x;
		}
		y = b[2];
		b[2] = b[k1];
		b[k1] = y;
	}
//销零
	x = a[2][2];
	y = a[0][2] / x;
	b[0] -= b[2] * y;
	a[0][3] -= a[2][3] * y;
	y = a[1][2] / x;
	b[1] -= b[2] * y;
	a[1][3] -= a[2][3] * y;
	y = a[3][2] / x;
	b[3] -= b[2] * y;
	a[3][3] -= a[2][3] * y;

	//第四列
	if (a[3][3] == 0) {
		return 5;
	}
	x = a[3][3];
	for (i = 0; i < 3; i++) {
		y = a[i][3] / a[3][3];
		b[i] -= b[3] * y;
	}

	//求解得到系数:
	for (i = 0; i < 4; i++) {
		b[i] /= a[i][i];
	}

	return 0;
}

//累加矩阵
/**
 *
 * @param mask
 * @param t
 * @return
 */
BOOL LineFit(Mat mask, double t[]) {
#if(_DEBUGMODE)
	double time = (double)getTickCount(); // 调试计时
#endif

	CV_Assert(mask.depth() != sizeof(uchar));// 只接受8U、单通道的二值图像。
	if (mask.channels() != 1) {
		printf("channels!=1");
		//throw ACException("只接受单通道mat数据。");
	}

	if (!mask.data) { // 检查数据
		printf("!data");
		//throw ac::ACException(EXP_NULL_MAT);
	}

	//double *a, *b;
	//int len;

	// 将mask点阵，变成坐标数组
	int nRows = mask.rows;
	int nCols = mask.cols;
	/*
	 if (mask.isContinuous()) {
	 nCols *= nRows;
	 nRows = 1;
	 }
	 */

	uchar* p;
	uchar x, y; // xy临时变量
	double m[2][2] = { 0 }; // 二维矩阵左侧变量
	double n[2] = { 0 }; // 乘数变量

	//TODO: 这里要扫描一遍图，目前只做了为了拟合曲线而做的矩阵计算，如果要提高性能，可以将多个计算放到这里，而不是多次扫描图像。
	// 根据坐标，计算矩阵运算的各个值
	for (int i = 0; i < nRows; ++i) {
		p = mask.ptr<uchar>(i);
		for (int j = 0; j < nCols; ++j) {
			//-------------------做曲线拟合运算部分，开始。----------------------
			//TODO: 降噪处理。还没有把重复点去掉；
			if ((p[j] == 0)) // 把（0，0）去掉
			{
				x = j; // 行是X,列是Y。
				y = i;
				printf("输入点坐标： %d , %d\n",x,y);
				m[0][0] += x;
				m[0][1] += 1;
				m[1][0] += x * x;
				n[0] += y;
				n[1] += x * y;
			}
			//-------------------做曲线拟合运算部分，结束。----------------------
		}
	}

	//-------------------做曲线拟合运算部分。----------------------
	m[1][1] = m[0][0];
	t[0] = n[0];
	t[1] = n[1];
	// 进行矩阵运算
	int ilab = GainParam2(m, t);
#if(_DEBUGMODE)
	//调试计时
	time = ((double)getTickCount() - time)/getTickFrequency();
	cout << "Times passed in seconds: " << time << endl;
#endif

	if (!ilab)
		return TRUE;
	else {
		if (ilab == 1)
			printf("线性拟合失败--2维数组行列式为零");
		else
			printf("线性拟合失败--2维数组行列式为零");
		return FALSE;
	}

}

BOOL LineFit(double *a, double *b, double t[], int len) {
	int i;
	double x, y; // xy临时变量
	double m[2][2] = { 0 }; // 二维矩阵左侧变量
	double n[2] = { 0 }; // 乘数变量

	// 根据坐标，计算矩阵运算的各个值
	for (i = 0; i < len; i++) {
		if (!(a[i] == 0 && b[i] == 0)) // 把（0，0）去掉
		{
			x = a[i];
			y = b[i];
			m[0][0] += x;
			m[0][1] += 1;
			m[1][0] += x * x;
			n[0] += y;
			n[1] += x * y;
		}
	}
	m[1][1] = m[0][0];
	t[0] = n[0];
	t[1] = n[1];

	// 进行矩阵运算
	i = GainParam2(m, t);
	if (!i)
		return TRUE;
	else {
		if (i == 1)
			printf("线性拟合失败--2维数组行列式为零");
		else
			printf("线性拟合失败--2维数组行列式为零");
		return FALSE;
	}

}

BOOL LineFit2(Mat mask, double t[]) {
	// 只接受8U、单通道的二值图像。
	CV_Assert(mask.depth() != sizeof(uchar));
	if (mask.channels() != 1) {
		//throw ACException("只接受单通道mat数据。");
	}
	// 检查数据
	if (!mask.data) {
		//throw ac::ACException(EXP_NULL_MAT);
	}

	//double *a, *b;
	//int len;

	// 将mask点阵，变成坐标数组
	int nRows = mask.rows;
	int nCols = mask.cols;
	uchar* p;

	int i;
	double m[3][3] = { 0 };
	double n[3] = { 0 };
	double x, y, z;

	//TODO: 这里要扫描一遍图，目前只做了为了拟合曲线而做的矩阵计算，如果要提高性能，可以将多个计算放到这里，而不是多次扫描图像。
	// 根据坐标，计算矩阵运算的各个值
	for (int i = 0; i < nRows; ++i) {
		p = mask.ptr<uchar>(i);
		for (int j = 0; j < nCols; ++j) {
			//-------------------做曲线拟合运算部分。----------------------
			//TODO: 降噪处理。还没有把重复点去掉；
			if (!(p[i] == 0)) // 把（0，0）去掉
			{
				x = i;
				y = j;
				z = x * x;
				m[0][0] += z;
				m[0][1] += x;
				m[0][2] += 1;
				m[1][0] += z * x;
				m[2][0] += z * z;
				n[0] += y;
				n[1] += x * y;
				n[2] += z * y;
			}
		}
	}

	m[1][1] = m[0][0];
	m[1][2] = m[0][1];
	m[2][1] = m[1][0];
	m[2][2] = m[0][0];
	for (i = 0; i < 3; i++) {
		t[i] = n[i];
	}

	i = GainParam3(m, t);
	if (!i)
		return TRUE;
	else {
		if (i == 1)
			printf("系数行列式值为零，二次拟合失败");
		else if (i == 2)
			printf("矩阵第一列全为零");
		else if (i == 3)
			printf("矩阵第二列迹为零");
		else
			printf("矩阵第三列迹为零");
	}
	return FALSE;

}

BOOL LineFit2(double *a, double *b, double t[], int len) {
	int i;
	double m[3][3] = { 0 };
	double n[3] = { 0 };
	double x, y, z;

	for (i = 0; i < len; i++) {
		x = a[i];
		y = b[i];
		z = x * x;
		m[0][0] += z;
		m[0][1] += x;
		m[0][2] += 1;
		m[1][0] += z * x;
		m[2][0] += z * z;
		n[0] += y;
		n[1] += x * y;
		n[2] += z * y;
	}
	m[1][1] = m[0][0];
	m[1][2] = m[0][1];
	m[2][1] = m[1][0];
	m[2][2] = m[0][0];
	for (i = 0; i < 3; i++) {
		t[i] = n[i];
	}

	i = GainParam3(m, t);
	if (!i)
		return TRUE;
	else {
		if (i == 1)
			printf("系数行列式值为零，二次拟合失败");
		else if (i == 2)
			printf("矩阵第一列全为零");
		else if (i == 3)
			printf("矩阵第二列迹为零");
		else
			printf("矩阵第三列迹为零");
	}
	return FALSE;
}

BOOL LineFit3(Mat mask, double t[]) {
	// 只接受8U、单通道的二值图像。
	CV_Assert(mask.depth() != sizeof(uchar));
	if (mask.channels() != 1) {
		//throw ACException("只接受单通道mat数据。");
	}
	// 检查数据
	if (!mask.data) {
		//throw ac::ACException(EXP_NULL_MAT);
	}

	// 将mask点阵，变成坐标数组
	int nRows = mask.rows;
	int nCols = mask.cols;
	uchar* p;
	int i;
	double m[4][4], n[4], x, y, z;

	for (i = 0; i < 4; i++) {
		m[i][0] = m[i][1] = 0.0;
		m[i][2] = m[i][3] = 0.0;
		n[i] = 0.0;
	}

	//TODO: 这里要扫描一遍图，目前只做了为了拟合曲线而做的矩阵计算，如果要提高性能，可以将多个计算放到这里，而不是多次扫描图像。
	// 根据坐标，计算矩阵运算的各个值
	for (int i = 0; i < nRows; ++i) {
		p = mask.ptr<uchar>(i);
		for (int j = 0; j < nCols; ++j) {
			//-------------------做曲线拟合运算部分。----------------------
			//TODO: 降噪处理。还没有把重复点去掉；
			if (!(p[i] == 0)) // 把（0，0）去掉
			{
				x = i;
				y = j;
				z = x * x * x;
				m[0][0] += z;
				m[0][1] += x * x;
				m[0][2] += x;
				m[0][3] += 1;
				n[0] += y;
				n[1] += x * y;
				n[2] += x * x * y;
				n[3] += z * y;
				m[1][0] += z * x;
				m[2][0] += z * x * x;
				m[3][0] += z * z;
			}
		}
	}

	m[1][1] = m[0][0];
	m[1][2] = m[0][1];
	m[1][3] = m[0][2];
	m[2][1] = m[1][0];
	m[2][2] = m[1][1];
	m[2][3] = m[0][1];
	m[3][1] = m[2][0];
	m[3][2] = m[1][0];
	m[3][3] = m[0][0];

	for (i = 0; i < 4; i++) {
		t[i] = n[i];
	}

	i = GainParam4(m, t);
	if (!i)
		return TRUE;
	else {
		if (i == 1)
			printf("系数行列式为零, 无解或无穷解");
		else if (i == 2)
			printf("矩阵第一列全为零");
		else if (i == 3)
			printf("矩阵第二列迹为零");
		else if (i == 4)
			printf("矩阵第三列迹为零");
		else
			printf("无定解，第四列迹为零");
	}
	return FALSE;
}

BOOL LineFit3(double *a, double *b, double t[], int len) {
	int i;
	double m[4][4], n[4], x, y, z;

	for (i = 0; i < 4; i++) {
		m[i][0] = m[i][1] = 0.0;
		m[i][2] = m[i][3] = 0.0;
		n[i] = 0.0;
	}
	for (i = 0; i < len; i++) {
		x = a[i];
		y = b[i];
		z = x * x * x;
		m[0][0] += z;
		m[0][1] += x * x;
		m[0][2] += x;
		m[0][3] += 1;
		n[0] += y;
		n[1] += x * y;
		n[2] += x * x * y;
		n[3] += z * y;
		m[1][0] += z * x;
		m[2][0] += z * x * x;
		m[3][0] += z * z;
	}
	m[1][1] = m[0][0];
	m[1][2] = m[0][1];
	m[1][3] = m[0][2];
	m[2][1] = m[1][0];
	m[2][2] = m[1][1];
	m[2][3] = m[0][1];
	m[3][1] = m[2][0];
	m[3][2] = m[1][0];
	m[3][3] = m[0][0];

	for (i = 0; i < 4; i++) {
		t[i] = n[i];
	}

	i = GainParam4(m, t);
	if (!i)
		return TRUE;
	else {
		if (i == 1)
			printf("系数行列式为零, 无解或无穷解");
		else if (i == 2)
			printf("矩阵第一列全为零");
		else if (i == 3)
			printf("矩阵第二列全为零");
		else if (i == 4)
			printf("矩阵第三列全为零");
		else
			printf("无定解，第四列全为零");
	}
	return FALSE;
}

int getY(int x, double t[],int len) {
	switch(len){
	case(2):{
			int y = t[0]+t[1]*x;
			printf("abxy:%f, %f : %d, %d \n",t[0],t[1],x,y);
			return y;
	}
	case(3):
			return t[0]+t[1]*x+t[2]*x*x;
	case(4):
			return t[0]+t[1]*x+t[2]*x*x+t[3]*x*x*x;
	default:
		return 0;
	}
	return 0;
}

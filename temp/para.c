/*
 * para.c
 *
 *
 *  Created on: 2015年9月25日
 *      Author: qiang.liu
 *
 * 写完，还没来得及写注释，已通过Matlab的polyfit验证（阶数高或者数据量太大会有double数据溢出的危险，低阶的都吻合），
 * 时间有点紧，程序注释，数学推导等时间充裕一点再贴上来
 *
 */

//
// 最小二乘法拟合.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include <stdio.h>
//#include <io.h>
//
#define ParaBuffer(Buffer,Row,Col) (*(Buffer + (Row) * (SizeSrc + 1) + (Col)))
//
/***********************************************************************************
***********************************************************************************/
static int GetXY(const char* FileName, double* X, double* Y, int* Amount)
{
        FILE* File = fopen(FileName, "r");
        if (!File)
                return -1;
        for (*Amount = 0; !feof(File); X++, Y++, (*Amount)++)
                if (2 != fscanf(File, (const char*)"%lf %lf", X, Y))
                        break;
                else
                	printf("getDataFromFile : %lf %lf ",X,Y);
        fclose(File);
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int PrintPara(double* Para, int SizeSrc)
{
        int i, j;
        for (i = 0; i < SizeSrc; i++)
        {
                for (j = 0; j <= SizeSrc; j++)
                        printf("%10.6lf ", ParaBuffer(Para, i, j));
                printf("\r\n");
        }
        printf("\r\n");
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int ParalimitRow(double* Para, int SizeSrc, int Row)
{
        int i;
        double Max, Min, Temp;
        for (Max = abs(ParaBuffer(Para, Row, 0)), Min = Max, i = SizeSrc; i; i--)
        {
                Temp = abs(ParaBuffer(Para, Row, i));
                if (Max < Temp)
                        Max = Temp;
                if (Min > Temp)
                        Min = Temp;
        }
        Max = (Max + Min) * 0.000005;
        for (i = SizeSrc; i >= 0; i--)
                ParaBuffer(Para, Row, i) /= Max;
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int Paralimit(double* Para, int SizeSrc)
{
        int i;
        for (i = 0; i < SizeSrc; i++)
                if (ParalimitRow(Para, SizeSrc, i))
                        return -1;
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int ParaPreDealA(double* Para, int SizeSrc, int Size)
{
        int i, j;
        for (Size -= 1, i = 0; i < Size; i++)
        {
                for (j = 0; j < Size; j++)
                        ParaBuffer(Para, i, j) = ParaBuffer(Para, i, j) * ParaBuffer(Para, Size, Size) - ParaBuffer(Para, Size, j) * ParaBuffer(Para, i, Size);
                ParaBuffer(Para, i, SizeSrc) = ParaBuffer(Para, i, SizeSrc) * ParaBuffer(Para, Size, Size) - ParaBuffer(Para, Size, SizeSrc) * ParaBuffer(Para, i, Size);
                ParaBuffer(Para, i, Size) = 0;
                ParalimitRow(Para, SizeSrc, i);
        }
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int ParaDealA(double* Para, int SizeSrc)
{
        int i;
        for (i = SizeSrc; i; i--)
                if (ParaPreDealA(Para, SizeSrc, i))
                        return -1;
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int ParaPreDealB(double* Para, int SizeSrc, int OffSet)
{
        int i, j;
        for (i = OffSet + 1; i < SizeSrc; i++)
        {
                for (j = OffSet + 1; j <= i; j++)
                        ParaBuffer(Para, i, j) *= ParaBuffer(Para, OffSet, OffSet);
                ParaBuffer(Para, i, SizeSrc) = ParaBuffer(Para, i, SizeSrc) * ParaBuffer(Para, OffSet, OffSet) - ParaBuffer(Para, i, OffSet) * ParaBuffer(Para, OffSet, SizeSrc);
                ParaBuffer(Para, i, OffSet) = 0;
                ParalimitRow(Para, SizeSrc, i);
        }
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int ParaDealB(double* Para, int SizeSrc)
{
        int i;
        for (i = 0; i < SizeSrc; i++)
                if (ParaPreDealB(Para, SizeSrc, i))
                        return -1;
        for (i = 0; i < SizeSrc; i++)
        {
                if (ParaBuffer(Para, i, i))
                {
                        ParaBuffer(Para, i, SizeSrc) /= ParaBuffer(Para, i, i);
                        ParaBuffer(Para, i, i) = 1.0;
                }
        }
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int ParaDeal(double* Para, int SizeSrc)
{
        PrintPara(Para, SizeSrc);
        Paralimit(Para, SizeSrc);
        PrintPara(Para, SizeSrc);
        if (ParaDealA(Para, SizeSrc))
                return -1;
        PrintPara(Para, SizeSrc);
        if (ParaDealB(Para, SizeSrc))
                return -1;
        return 0;
}
/***********************************************************************************
***********************************************************************************/
static int GetParaBuffer(double* Para, const double* X, const double* Y, int Amount, int SizeSrc)
{
        int i, j;
        for (i = 0; i < SizeSrc; i++)
                for (ParaBuffer(Para, 0, i) = 0, j = 0; j < Amount; j++)
                        ParaBuffer(Para, 0, i) += pow(*(X + j), 2 * (SizeSrc - 1) - i);
        for (i = 1; i < SizeSrc; i++)
                for (ParaBuffer(Para, i, SizeSrc - 1) = 0, j = 0; j < Amount; j++)
                        ParaBuffer(Para, i, SizeSrc - 1) += pow(*(X + j), SizeSrc - 1 - i);
        for (i = 0; i < SizeSrc; i++)
                for (ParaBuffer(Para, i, SizeSrc) = 0, j = 0; j < Amount; j++)
                        ParaBuffer(Para, i, SizeSrc) += (*(Y + j)) * pow(*(X + j), SizeSrc - 1 - i);
        for (i = 1; i < SizeSrc; i++)
                for (j = 0; j < SizeSrc - 1; j++)
                        ParaBuffer(Para, i, j) = ParaBuffer(Para, i - 1, j + 1);
        return 0;
}
/***********************************************************************************
***********************************************************************************/
int Cal(const double* BufferX, const double* BufferY, int Amount, int SizeSrc, double* ParaResK)
{
        double* ParaK = (double*)malloc(SizeSrc * (SizeSrc + 1) * sizeof(double));
        GetParaBuffer(ParaK, BufferX, BufferY, Amount, SizeSrc);
        ParaDeal(ParaK, SizeSrc);
        for (Amount = 0; Amount < SizeSrc; Amount++, ParaResK++)
                *ParaResK = ParaBuffer(ParaK, Amount, SizeSrc);
        free(ParaK);
        return 0;
}
/***********************************************************************************
***********************************************************************************/
int main(int argc, char* argv[])
{
        int Amount;
        double BufferX[1024], BufferY[1024], ParaK[6];
        if (GetXY((const char*)"test.txt", (double*)BufferX, (double*)BufferY, &Amount))
                return 0;
        Cal((const double*)BufferX, (const double*)BufferY, Amount, sizeof(ParaK) / sizeof(double), (double*)ParaK);
        for (Amount = 0; Amount < sizeof(ParaK) / sizeof(double); Amount++)
                printf("ParaK[%d] = %lf!\r\n", Amount, ParaK[Amount]);
        return 0;
}

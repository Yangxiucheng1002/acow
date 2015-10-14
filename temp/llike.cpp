
#include <stdio.h>

#ifndef BOOL
#define BOOL bool
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

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

//.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//多项式拟合用函数

int GainParam2(double a[][2], double b[])
{
    double x, y;
    int i;

    y=a[0][0]*a[1][1]-a[0][1]*a[1][0];
    if(!y)
    {
        return 1;
    }

    x=a[0][0];
    if(!x)
    {
        if(!a[1][0])
        {
            return 2;
        }
        for(i=0;i<2;i++)
        {
            x=a[0][i];
            a[0][i]=a[1][i];
            a[1][i]=x;
        }
        y=b[0];
        b[0]=b[1];
        b[1]=y;
    }
    x=a[1][0]/a[0][0];
    a[1][1]-=x*a[0][1];
    b[1]-=b[0]*x;
    x=a[0][1]/a[1][1];
    b[0]-=b[1]*x;

    b[0]/=a[0][0];
    b[1]/=a[1][1];

    return 0;

}

int GainParam3(double a[][3], double b[])
{
    double x,y;
    int i, j;
    y=a[0][0]*(a[1][1]*a[2][2]-a[1][2]*a[2][1])-a[0][1]*(a[1][0]*a[2][2]-a[1][2]*a[2][0])+
        a[0][2]*(a[1][0]*a[2][1]-a[1][1]*a[2][0]);

    if(!y)
    {
        return 1;
    }

//行列式对角化

    //第一列挑选非零项
    for(i=0;i<3;i++)
    {
        x=a[i][0];
        if(x)break;
    }
    if(i==3)
    {
        return 2;
    }
    if(i>0)
    {
        j=i;
        for(i=0;i<3;i++)
        {
            x=a[0][i];
            a[0][i]=a[j][i];
            a[j][i]=x;
        }
        y=b[0];
        b[0]=b[j];
        b[j]=y;
    }
    //销零
    x=a[0][0];
    y=a[1][0]/x;
    b[1]-=b[0]*y;
    for(i=1;i<3;i++)
        a[1][i]-=a[0][i]*y;
    y=a[2][0]/x;
    b[2]-=b[0]*y;
    for(i=1;i<3;i++)
        a[2][i]-=a[0][i]*y;

    //第二列挑选非零项
    for(i=1;i<3;i++)
    {
        x=a[i][1];
        if(x)break;
    }
    if(i==3)
    {
        return 3;
    }
    if(i>1)
    {
        j=i;
        for(i=0;i<3;i++)
        {
            x=a[1][i];
            a[1][i]=a[j][i];
            a[j][i]=x;
        }
        y=b[1];
        b[1]=b[j];
        b[j]=y;
    }

//销零
    x=a[1][1];
    y=a[0][1]/x;
    b[0]-=b[1]*y;
    a[0][2]-=a[1][2]*y;
    y=a[2][1]/x;
    b[2]-=b[1]*y;
    a[2][2]-=a[1][2]*y;

    if(!a[2][2])
    {
        return 4;
    }
    x=a[2][2];
    for(i=0;i<2;i++)
    {
        y=a[i][2]/x;
        b[i]-=b[2]*y;
    }

    //求解得到系数:
    for(i=0;i<3;i++)
    {
        b[i]/=a[i][i];
    }

    return 0;
}


int GainParam4(double a[][4], double b[])
{
    double x, y;
    int i, k1;

    x=0;
    y=0;
    x+=a[1][0]*(a[2][1]*a[3][2]-a[2][2]*a[3][1])+a[1][1]*(a[2][2]*a[3][1]-a[2][0]*a[3][2])+
        a[1][2]*(a[2][0]*a[3][1]-a[2][1]*a[3][0]);
    y+=-x*a[0][3];
    x=a[1][1]*(a[2][2]*a[3][3]-a[2][3]*a[3][2])+a[1][2]*(a[2][3]*a[3][1]-a[2][1]*a[3][3])+
        a[1][3]*(a[2][1]*a[3][2]-a[2][2]*a[3][1]);
    y+=a[0][0]*x;
    x=a[1][0]*(a[2][2]*a[3][3]-a[2][3]*a[3][2])-a[1][2]*(a[2][0]*a[3][3]-a[2][3]*a[3][0])+
        a[1][3]*(a[2][0]*a[3][2]-a[2][2]*a[3][0]);
    y+=-x*a[0][1];
    x=a[1][0]*(a[2][2]*a[3][3]-a[2][3]*a[3][1])+a[1][1]*(a[2][3]*a[3][0]-a[2][0]*a[3][3])+
        a[1][3]*(a[2][0]*a[3][1]-a[2][1]*a[3][0]);
    y+=x*a[0][2];

    if(!y)
    {
        return 1;
    }

//行列式对角化

    //第一列挑选非零项
    for(i=0;i<4;i++)
    {
        x=a[i][0];
        if(x)break;
    }
    if(i==4)
    {
        return 2;
    }
    if(i>0)
    {
        k1=i;
        for(i=0;i<4;i++)
        {
            x=a[0][i];
            a[0][i]=a[k1][i];
            a[k1][i]=x;
        }
        y=b[0];
        b[0]=b[k1];
        b[k1]=y;
    }


//销零
    x=a[0][0];
    y=a[1][0]/x;
    b[1]-=b[0]*y;
    for(i=1;i<4;i++)
        a[1][i]-=a[0][i]*y;
    y=a[2][0]/x;
    b[2]-=b[0]*y;
    for(i=1;i<4;i++)
        a[2][i]-=a[0][i]*y;
    y=a[3][0]/x;
    b[3]-=b[0]*y;
    for(i=1;i<4;i++)
        a[3][i]-=a[0][i]*y;


    //第二列挑选非零项
    for(i=1;i<4;i++)
    {
        x=a[i][1];
        if(x)break;
    }
    if(i==4)
    {
        return 3;
    }
    if(i>1)
    {
        k1=i;
        for(i=0;i<4;i++)
        {
            x=a[1][i];
            a[1][i]=a[k1][i];
            a[k1][i]=x;
        }
        y=b[1];
        b[1]=b[k1];
        b[k1]=y;
    }

//销零
    x=a[1][1];
    y=a[0][1]/x;
    b[0]-=b[1]*y;
    for(i=2;i<4;i++)
        a[0][i]-=a[1][i]*y;
    y=a[2][1]/x;
    b[2]-=b[1]*y;
    for(i=2;i<4;i++)
        a[2][i]-=a[0][i]*y;
    y=a[3][1]/x;
    b[3]-=b[1]*y;
    for(i=2;i<4;i++)
        a[3][i]-=a[0][i]*y;

//第三列
    for(i=2;i<4;i++)
    {
        x=a[i][2];
        if(x)break;
    }
    if(i==4)
    {
        return 4;
    }
    if(i>2)
    {
        k1=i;
        for(i=0;i<4;i++)
        {
            x=a[2][i];
            a[2][i]=a[k1][i];
            a[k1][i]=x;
        }
        y=b[2];
        b[2]=b[k1];
        b[k1]=y;
    }
//销零
    x=a[2][2];
    y=a[0][2]/x;
    b[0]-=b[2]*y;
    a[0][3]-=a[2][3]*y;
    y=a[1][2]/x;
    b[1]-=b[2]*y;
    a[1][3]-=a[2][3]*y;
    y=a[3][2]/x;
    b[3]-=b[2]*y;
    a[3][3]-=a[2][3]*y;

    //第四列
    if(a[3][3]==0)
    {
        return 5;
    }
    x=a[3][3];
    for(i=0;i<3;i++)
    {
        y=a[i][3]/a[3][3];
        b[i]-=b[3]*y;
    }

    //求解得到系数:
    for(i=0;i<4;i++)
    {
        b[i]/=a[i][i];
    }

    return 0;
}


//累加矩阵

BOOL LineFit(double *a, double *b, double t[], int len)
{
    int i;
    double x,y;
    double m[2][2]={0};
    double n[2]={0};

    for(i=0;i<len;i++)
    {
        x=a[i];
        y=b[i];
        m[0][0]+=x;
        m[0][1]+=1;
        m[1][0]+=x*x;
        n[0]+=y;
        n[1]+=x*y;
    }
    m[1][1]=m[0][0];
    t[0]=n[0];
    t[1]=n[1];

    i=GainParam2(m,t);
    if(!i)
        return TRUE;
    else
    {
        if(i==1)
            printf("线性拟合失败--2维数组行列式为零");
        else
            printf("线性拟合失败--2维数组行列式为零");
        return FALSE;
    }

}
BOOL LineFit2(double *a, double *b, double t[], int len)
{
    int i;
    double m[3][3]={0};
    double n[3]={0};
    double x,y,z;


    for(i=0;i<len;i++)
    {
        x=a[i];
        y=b[i];
        z=x*x;
        m[0][0]+=z;
        m[0][1]+=x;
        m[0][2]+=1;
        m[1][0]+=z*x;
        m[2][0]+=z*z;
        n[0]+=y;
        n[1]+=x*y;
        n[2]+=z*y;
    }
    m[1][1]=m[0][0];
    m[1][2]=m[0][1];
    m[2][1]=m[1][0];
    m[2][2]=m[0][0];
    for(i=0;i<3;i++)
    {
        t[i]=n[i];
    }

    i=GainParam3(m, t);
    if(!i)
        return TRUE;
    else
    {
        if(i==1)
            printf("系数行列式值为零，二次拟合失败");
        else if(i==2)
            printf("矩阵第一列全为零");
        else if(i==3)
            printf("矩阵第二列迹为零");
        else
            printf("矩阵第三列迹为零");
    }
    return FALSE;
}

BOOL LineFit3(double *a, double *b, double t[], int len)
{
    int i;
    double m[4][4],n[4], x, y, z;

    for(i=0;i<4;i++)
    {
        m[i][0]=m[i][1]=0.0;
        m[i][2]=m[i][3]=0.0;
        n[i]=0.0;
    }
    for(i=0;i<len;i++)
    {
        x=a[i];
        y=b[i];
        z=x*x*x;
        m[0][0]+=z;
        m[0][1]+=x*x;
        m[0][2]+=x;
        m[0][3]+=1;
        n[0]+=y;
        n[1]+=x*y;
        n[2]+=x*x*y;
        n[3]+=z*y;
        m[1][0]+=z*x;
        m[2][0]+=z*x*x;
        m[3][0]+=z*z;
    }
    m[1][1]=m[0][0];
    m[1][2]=m[0][1];
    m[1][3]=m[0][2];
    m[2][1]=m[1][0];
    m[2][2]=m[1][1];
    m[2][3]=m[0][1];
    m[3][1]=m[2][0];
    m[3][2]=m[1][0];
    m[3][3]=m[0][0];


    for(i=0;i<4;i++)
    {
        t[i]=n[i];
    }

    i=GainParam4(m,t);
    if(!i)
        return TRUE;
    else
    {
        if(i==1)
            printf("系数行列式为零, 无解或无穷解");
        else if(i==2)
            printf("矩阵第一列全为零");
        else if(i==3)
            printf("矩阵第二列迹为零");
        else if(i==4)
            printf("矩阵第三列迹为零");
        else
            printf("无定解，第四列迹为零");
    }
    return FALSE;
}

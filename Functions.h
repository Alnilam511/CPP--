#pragma once
#pragma warning(disable: 4505)

#include <math.h>
#include <fstream> 

//获取文件名称
//TCHAR* tchar = GetFileName(szFile); 调用
static TCHAR* GetFileName(TCHAR szfile[256])
{
	char nchar[256] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, szfile, -1, nchar, 256, NULL, NULL);

	char* file = nchar;
	char szDrive[_MAX_DRIVE];   //磁盘名
	char szDir[_MAX_DIR];       //路径名
	char szFname[_MAX_FNAME];   //文件名
	char szExt[_MAX_EXT];       //后缀名
	_splitpath_s(file, szDrive, szDir, szFname, szExt);

	static wchar_t tchar[256] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, szFname, -1, tchar, 256 * sizeof(TCHAR));
	return tchar;
}

//获取数据个数
static int GetLine(std::ifstream& fin)
{
	char c;
	int line = 0;
	while (fin.get(c))
	{
		if (c == '\n')
		{
			line++;
		}
	}
	return line;
}

//赋值
static double* GetNum(std::ifstream& fin, int n)
{
	static double num[10240] = { 0 };
	for (int i = 0; i < n; i++)
	{
		fin >> num[i];
	}
	return num;
}

//最大值
static double Maximum(double x, double y)
{
	double z;
	if (x > y)
		z = x;
	else
		z = y;
	return z;
}

//最小值
static double Minimum(double x, double y)
{
	double z;
	if (x < y)
		z = x;
	else
		z = y;
	return z;
}

//总值
static double Sum(double num[10240],int n)
{
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum = sum + num[i];
	}
	return sum;
}

//均值
static double Average(double num[10240], int n)
{
	double sum = 0, average = 0;
	sum = Sum(num, n);
	average = sum / n;
	return average;
}

//方差
static double Variance(double num[10240], int n)
{
	double average = 0, variance = 0;
	average = Average(num, n);
	for (int i = 0; i < n; i++)
	{
		variance = variance + (num[i] - average) * (num[i] - average);
	}
	variance = variance / n;
	return variance;
}

//峭度
static double Kurtosis(double num[10240], int n)
{
	double average = 0, variance = 0, kurtosis = 0;
	average = Average(num, n);
	variance = Variance(num, n);
	for (int i = 0; i < n; i++)
	{
		kurtosis = kurtosis + (num[i] - average) * (num[i] - average) * (num[i] - average) * (num[i] - average);
	}
	kurtosis = kurtosis / n;
	kurtosis = kurtosis / (variance * variance);
	return kurtosis;
}

//自相关系数
static double AutoCorrelation(double num[10240], int n)
{
	double average = 0, variance = 0;
	average = Average(num, n);
	variance = Variance(num, n);
	double cov = 0, R = 0;
	for (int i = 0; i < n - 1; i++)
	{
		cov = cov + (num[i] - average) * (num[i + 1] - average);
	}
	cov = cov / (n - 1);
	R = cov / variance;
	return R;
}

//互相关系数
static double CrossCorrelation(double num1[10240], double num2[10240], int n1, int n2)
{
	double average1 = 0, variance1 = 0, average2 = 0, variance2 = 0;
	average1 = Average(num1, n1);
	variance1 = Variance(num1, n1);
	average2 = Average(num2, n2);
	variance2 = Variance(num2, n2);
	double cov = 0, R = 0;
	for (int i = 0; i < n1; i++)
	{
		cov = cov + (num1[i] - average1) * (num2[i] - average2);
	}
	cov = cov / (n1);
	R = cov / (sqrt(variance1) * sqrt(variance2));
	return R;
}

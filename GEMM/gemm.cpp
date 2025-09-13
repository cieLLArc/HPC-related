#include <iostream>
#include <stdio.h>
using namespace std;

int main()
{
	// 给定m*n的矩阵C由A,B相乘得到
	// 已知矩阵C是m*n的,且A,B相乘得到C,故A,B相乘的内标为k

	int m = 0, n = 0, k = 0;
	int C[100][100] = {0}; // 初始化矩阵C为0
	int A[100][100] = {0}; // 初始化矩阵A为0
	int B[100][100] = {0}; // 初始化矩阵B为0

	// 给定A,B矩阵,计算其结果C
	// 输入:A的行数m,列数k,B的行数k,列数n,即A的行数m和列数k，C的列数n

	// 向矩阵C中填充A,B相乘的计算结果
	for (int i = 0; i < m; i++) // 根据矩阵乘法的定义,从第一行开始填充
	{
		for (int j = 0; j < n; j++) //
		{
			for (int p = 0; p < k; p++)
			{
				C[i][j] += A[i][p] * B[p][j];
			}
		}
	}
	cout << "A" << endl;
	// system("pause");
	return 0;
}

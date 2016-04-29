#include<Windows.h>
#include<assert.h>
#include<fstream>
#include<iostream>
using namespace std;
bool readBmp(char *bmpName, unsigned char *&pBmpBuf, LONG &bmpWidth, LONG &bmpHeight)
{
	FILE *fp = fopen(bmpName, "rb");//�����ƶ���ʽ��ָ����ͼ���ļ�

	if (fp == 0)
		return 0;

	//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER

	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��

	BITMAPINFOHEADER head;

	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp); //��ȡͼ����ߡ�ÿ������ռλ������Ϣ

	bmpWidth = head.biWidth;

	bmpHeight = abs(head.biHeight);

	WORD biBitCount = head.biBitCount;//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����

	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�

	pBmpBuf = new unsigned char[lineByte * bmpHeight];

	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	fclose(fp);//�ر��ļ�

	return 1;//��ȡ�ļ��ɹ�
}
void findSubBmp()//�������ж�RGB
{
	unsigned char *src = NULL, *des = NULL;
	LONG srcWidth, srcHeight, desWidth, desHeight;
	readBmp("src.bmp", src, srcWidth, srcHeight);
	readBmp("des.bmp", des, desWidth, desHeight);
	int srclineByte = (srcWidth * 24 / 8 + 3) / 4 * 4;
	int deslineByte = (desWidth * 24 / 8 + 3) / 4 * 4;
	int maxValue = 10, sum = 0, maxv = 0, maxi = 0, maxj = 0, maxv2 = 0, maxi2 = 0, maxj2 = 0;
	for (int i = 0; i < srcHeight; i += 8)
	{
		for (int j = 0; j < srcWidth; j += 8)
		{
			sum = 0;
			for (int m = 0; srcHeight - i>desHeight && m < desHeight; ++m)
			{
				for (int n = 0; srcWidth - j>desWidth && n < desWidth; ++n)
				{
					unsigned char b = src[(i + m)*srclineByte + (j + n) * 3];
					unsigned char g = src[(i + m)*srclineByte + (j + n) * 3 + 1];
					unsigned char r = src[(i + m)*srclineByte + (j + n) * 3 + 2];

					unsigned char b2 = des[(m)*deslineByte + (n)* 3];
					unsigned char g2 = des[(m)*deslineByte + (n)* 3 + 1];
					unsigned char r2 = des[(m)*deslineByte + (n)* 3 + 2];

					if (abs(b - b2) < maxValue && abs(g - g2) < maxValue && abs(r - r2) < maxValue) sum++;
				}
			}
			if (sum>maxv)
			{
				maxv = sum;
				maxi = i;
				maxj = j;
			}
			else if (sum>maxv2)
			{
				maxv2 = sum;
				maxi2 = i;
				maxj2 = j;
			}
		}
	}
	cout << maxv << " " << maxj << " " << maxi << endl;
	cout << maxv2 << " " << maxj2 << " " << maxi2 << endl;
	HDC hdc = ::GetDC(NULL);
	for (int i = maxi; i < maxi + desHeight; ++i)
	{
		for (int j = maxj; j < maxj + desWidth; ++j)
		{
			unsigned char b = src[i*srclineByte + j * 3];
			unsigned char g = src[i*srclineByte + j * 3 + 1];
			unsigned char r = src[i*srclineByte + j * 3 + 2];
			COLORREF c = RGB(r, g, b);
			::SetPixel(hdc, j - maxj, maxi + desHeight - i, c);
		}
	}
}


int main()
{
	findSubBmp();
	system("pause");
}
#include<Windows.h>
#include<assert.h>
#include<fstream>
#include<iostream>
#include <opencv2\opencv.hpp>
#include "processImage.h"
#include "gameInfo.h"
#include "ControlMouse.h"
#include "ai.h"
using namespace std;
using namespace cv;
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
Mat getCurrentImage(HWND hWnd)
{
	HDC hDC = ::GetWindowDC(hWnd);//�õ����
	assert(hDC);

	HDC hMemDC = ::CreateCompatibleDC(hDC);//��������dc

	assert(hMemDC);

	RECT rc;
//	::GetWindowRect(hWnd, &rc);//�˾��ΰ���������߿�
	::GetClientRect(hWnd, &rc);//�õ����沿�־��δ�С
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);//����λͼ
	assert(hBitmap);

	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);

	::PrintWindow(hWnd, hMemDC, PW_CLIENTONLY);//�õ�����

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	BITMAPINFOHEADER bi = { 0 };
	CONST int nBitCount = 24;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount = nBitCount;
	bi.biCompression = BI_RGB;
	DWORD dwSize = width * 3 * height;
	unsigned char *data = new unsigned char[dwSize];
	::GetDIBits(hMemDC, hBitmap, 0, height, data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);//�����������data��
	Mat image(height, width, CV_8UC3, data);//��ucharת��Ϊmat���ͣ�����opencv
	flip(image, image, 0);//ͼ�����·�ת

	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hBitmap);
	::DeleteObject(hMemDC);
	::ReleaseDC(hWnd, hDC);
	return image;
}
int main()
{
	HWND hWnd = ::FindWindow("UnityWndClass", ("¯ʯ��˵"));
	ProcessImage processImage;
	GameInfo gameInfo;
	AI aiControl(hWnd);
	while (1)
	{
		Mat image = getCurrentImage(hWnd);
		processImage.process(image, gameInfo);
		aiControl.process(gameInfo);
		Sleep(1000);
	}
	
	//ControlMouse control(hWnd);
	//control.playCard(450, 745);
//	Mat image = getCurrentImage(hWnd);
////	imshow("test", image);
//	processImage process;
//	GameInfo cinfo;
//	process.process(image,cinfo);
//	ControlMouse controlMouse(hWnd);
//	for (int i = 0; i < cinfo.currentNum; ++i)
//		cout << "cardnum:" << cinfo.handCard[i].spend << endl;
//	for (int i = 0; i < cinfo.currentNum; ++i)
//	{
//		if (cinfo.handCard[i].spend >= 2)
//		{
//			cout << cinfo.handCard[i].x << " " << cinfo.handCard[i].y << endl;
//			controlMouse.touchPosition(cinfo.handCard[i].x, cinfo.handCard[i].y);
//			Sleep(100);
//		}
//	}
//	//ȷ��
//	controlMouse.touchPosition(500, 600);
//	Sleep(100);
	system("pause");
}
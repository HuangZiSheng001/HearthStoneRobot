#include<Windows.h>
#include<assert.h>
#include<fstream>
#include<iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
void SaveHwndToBmpFile(HWND hWnd)
{
	HDC hDC = ::GetWindowDC(hWnd);//�õ����
	if (hDC == NULL) return;

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
	imshow("test", image);
	waitKey(0);
	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hBitmap);
	::DeleteObject(hMemDC);
	::ReleaseDC(hWnd, hDC);
}
int main()
{
	HWND hWnd = ::FindWindow("UnityWndClass", ("¯ʯ��˵"));
	SaveHwndToBmpFile(hWnd);
}
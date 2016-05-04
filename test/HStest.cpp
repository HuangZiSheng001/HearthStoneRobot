#include <opencv2\opencv.hpp>
#include <fstream>
#include<vector>
using namespace std;
using namespace cv;
/** @������ */
const int MAXV = 1;
fstream f;
int num[11][9] = { { 0, 1, 1, 1, 0, 1, 1, 1, 1 }, { 0, 1, 1, 0, 1, 1, 0, 1, 1 }, { 1, 0, 1, 1, 0, 1, 0, 1, 0 }, { 0, 1, 0, 0, 1, 1, 0, 0, 1 },
{ 0, 0, 1, 0, 0, 1, 0, 1, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 1, 0, 1, 1, 1, 1 }, { 1, 1, 1, 0, 1, 0, 0, 1, 0 },
{ 0, 0, 1, 0, 1, 1, 1, 0, 0 }, { 1, 1, 1, 1, 0, 1, 0, 0, 0 }, { 0, 1, 1, 1, 0, 1, 1, 1, 0 } };
void find(Mat src)//ʹ��ͼ����ҷ���
{
	Mat des = imread("HS/tep_2.bmp", 1);
	Mat image = imread("HS/tpl.bmp", 1);
	Mat tpl[16];
	for (int i = 0; i < 4;++i)
	for (int j = 0; j < 4; ++j)
	{
		image(Rect(j * 45, i * 45, 45, 45)).copyTo(tpl[i * 4 + j]);
		//imshow("tpl", tpl[i*4+j]);
		//waitKey(0);
	}
	tpl[0] = imread("HS/0.bmp", 1);
	tpl[1] = imread("HS/1.bmp", 1);
	tpl[2] = imread("HS/2.bmp", 1);
	tpl[3] = imread("HS/3.bmp", 1);
	int resultsum = 0, ri = 0, rj = 0,rn=0;
	for (int k = 0; k < 14; ++k)
	{
		int maxsum = 0, maxi = 0, maxj = 0;
		imshow("tpl", tpl[k]);
		for (int i = 0; i < src.rows; i+=2)
		{
			for (int j = 0; j<src.cols; j+=2)
			{
				int sum = 0;
				for (int m = 0; src.rows - i>tpl[k].rows&& m < tpl[k].rows; m++)
				{
					for (int n = 0; src.cols - j>tpl[k].cols&& n < tpl[k].cols; n++)
					{
						uchar r = src.at<Vec3b>(i + m, j + n)[0];
						uchar g = src.at<Vec3b>(i + m, j + n)[1];
						uchar b = src.at<Vec3b>(i + m, j + n)[2];

						uchar r2 = tpl[k].at<Vec3b>(m, n)[0];
						uchar g2 = tpl[k].at<Vec3b>(m, n)[1];
						uchar b2 = tpl[k].at<Vec3b>(m, n)[2];
					//	cout << r2 << " " << g2 << " " << b2 << endl;
						if (r2+g2+b2!=255*3 && abs(r - r2) < MAXV && abs(g - g2) < MAXV && abs(b - b2) < MAXV)
						{
							sum++;
						}
					}
				}
				if (sum>maxsum)
				{
					maxsum = sum;
					maxi = i;
					maxj = j;
				}
				/*if (src.cols - j > tpl[k].cols && src.rows - i > tpl[k].rows)
				{
					Rect r(i, j, tpl[rn].rows, tpl[rn].cols);
					Mat temp;
					src(r).copyTo(temp);
					cout << sum << endl;
					imshow("����", temp);
					waitKey(0);
				}*/
			}
		}
		cout << "num:" << k << "maxsum:" << maxsum << endl;
		if (maxsum > resultsum)
		{
			resultsum = maxsum;
			ri = maxi;
			ri = maxj;
			rn = k;
		}
	}
	Rect r(ri, ri, tpl[rn].rows, tpl[rn].cols);
	Mat temp;
	src(r).copyTo(temp);
	cout << "result::num:" << rn << ";max:" << resultsum << endl;
	imshow("����", temp);
	waitKey(0);
}
void recoNum(Mat src)//����ʶ��
{
	Mat sub[9];
	int h = src.rows / 3;
	int w = src.cols / 3;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			src(Rect(j*w, i*h, w, h)).copyTo(sub[i * 3 + j]);
			/*imshow("sub", sub[i*3 + j]);
			waitKey(0);*/
		}
	}
	int temp[9] = { 0 };
	for (int i = 0; i < 9; ++i)
	{
		int num = 0;
		for (int j = 0; j < sub[i].cols; ++j)
		{
			for (int k = 0; k < sub[i].rows; ++k)
			{
				if (sub[i].at<uchar>(k, j) == 255)
				{
					num++;
				}
			}
		}
		//cout << num << " ";
		if (num>w*h / 2) temp[i] = 1, cout << "1" <<" ";
		else cout << "0" << " ";
		if (i % 3 == 2) f << endl;
	}
	int maxsum = 0,maxn=0;
	for (int i = 0; i < 11; ++i)
	{
		int sum = 0;
		for (int j = 0; j < 9; ++j)
		{
			if (temp[j] == num[i][j])
			{
				++sum;
			}
		}
		if (sum>maxsum)
		{
			maxsum = sum;
			maxn = i;
		}
	}
	cout << "ʶ����������ǣ�"<<maxn << endl;
}
void find2(Mat src)//��ȡ���ֲ�������
{
	Mat image;
	src(Rect(0, 0, src.rows / 4, src.cols / 3)).copyTo(image);
	imshow("windows",image);
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	Mat thr;
	threshold(gray, thr, 250, 255, CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat cz;
	dilate(thr, cz, element);
	erode(cz, cz, element);
	/*imshow("gray2", cz);
	waitKey(0);*/
	vector<std::vector<Point>> contours;
	findContours(cz,			//ͼ��
		contours,				//������
		//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
		CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
		CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������
	//��ӡ������Ϣ
	std::cout << "������Χ������" << contours.size() << "��" << std::endl;
	Rect r0 = boundingRect(Mat(contours[0]));
	Mat matio;
	thr(r0).copyTo(matio);
	rectangle(src, r0, Scalar(0), 2);
	//imshow("222" , matio);
	//waitKey(0);
	recoNum(matio);
	f << endl;
}
void preprocess()//Ԥ�����õ�����0-10��ģ��
{
	f.open("result.txt", ios::out);
	for (int i = 0; i < 11; ++i)
	{
		stringstream ss;
		ss << "HS/tpl_" << i << ".bmp";
		string s = ss.str();
		Mat tpl = imread(s, 1);
		f << i << endl;
		find2(tpl);
		waitKey(0);
	}
	f.close();
}
void process()//Ԥ�����ƽ׶�ͼ��
{
	/// ����ԭͼ��, ����3ͨ��ͼ������
	Mat src = imread("HS/test.bmp", 1);
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	Mat thr;
	threshold(gray, thr, 50, 255, CV_THRESH_BINARY);
	imshow("gray", thr);
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	dilate(thr, thr, element);
	erode(thr, thr, element);
	//erode(thr, thr, element);

	imshow("fushi", thr);
	waitKey(0);
	vector<std::vector<Point>> contours;
	findContours(thr,			//ͼ��
		contours,				//������
		//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
		CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
		CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������
	//��ӡ������Ϣ
	std::cout << "������Χ������" << contours.size() << "��" << std::endl;
	std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours)
	{
		std::cout << "ÿ�������ĳ���: " << itContours->size() << std::endl;
	}




	Mat result(thr.size(), CV_8U, Scalar(255));
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);
	imshow("��ȡ��Χ����", result);

	//��ȥ̫������̫�̵�����
	int cmin = 700;
	int cmax = 900;
	std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}

	//��������

	//��������������Ϊ�����壬����������ָʾ�����ﻭ����������������ɫ���ߴ�
	//	drawContours(result, contours, -1, Scalar(0), 2);
	//�������ػ��ڰװ���
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 1);
	imshow("ȥ����С��", result); //��Ҫע����ȡ�����ķ������кܶ��֣�����CV_RETR_LIST������������
	waitKey(0);
	//��������״������
	//��Ӿ���
	Mat matio[10];
	int num = contours.size();
	for (int i = 0; i < contours.size(); ++i)
	{
		Rect r0 = boundingRect(Mat(contours[i]));
		src(r0).copyTo(matio[i]);
		rectangle(src, r0, Scalar(0), 2);
		imshow("" + i, matio[i]);
		find2(matio[i]);
	}
	imshow("��Ӿ���", src);

	//	findContours(thr,			//ͼ�� 
	//	contours,				//������
	//	//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
	//	CV_RETR_LIST,			//��ȡ�����ķ����������ȡ����������
	//	CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ������
	////��������
	//drawContours(result, contours, -1, Scalar(0), 2);
	//imshow("��ȡ��������", result);
}
void process2()//������ƽ׶γ���
{
	/// ����ԭͼ��, ����3ͨ��ͼ������
	Mat src = imread("HS/secene_1.bmp", 1);
	Mat image;
	src.copyTo(image);
	for (int i = 0; i<src.rows; i++)//��ֵ������ɫ���£������Ϊ��ɫ
	{
		for (int j = 0; j<src.cols; j++)
		{
			uchar r = src.at<Vec3b>(i, j)[0];
			uchar g = src.at<Vec3b>(i, j)[1];
			uchar b = src.at<Vec3b>(i, j)[2];
			if (r == 255 && g == 255 && b == 255)
			{
				image.at<Vec3b>(i, j)[0] = 255;
				image.at<Vec3b>(i, j)[1] = 255;
				image.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				image.at<Vec3b>(i, j)[0] = 0;
				image.at<Vec3b>(i, j)[1] = 0;
				image.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);//ת��Ϊ��ͨ�����ڽ���������ȡ
	imshow("gray", gray);

	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//��ʴ����Ч������
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//imshow("fushi", thr);
	vector<std::vector<Point>> contours;
	findContours(gray,			//ͼ��
		contours,				//������
		//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
		CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
		CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������
	//��ӡ������Ϣ
	std::cout << "������Χ������" << contours.size() << "��" << std::endl;
	std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours)
	{
		std::cout << "ÿ�������ĳ���: " << itContours->size() << std::endl;
	}




	Mat result(gray.size(), CV_8U, Scalar(255));
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);
	imshow("��ȡ��Χ����", result);

	//��ȥ̫������̫�̵�����
	int cmin = 10;
	int cmax = 100;
	std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}

	//��������������Ϊ�����壬����������ָʾ�����ﻭ����������������ɫ���ߴ�
	//	drawContours(result, contours, -1, Scalar(0), 2);
	//�������ػ��ڰװ���
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 1);
	imshow("ȥ����С��", result); //��Ҫע����ȡ�����ķ������кܶ��֣�����CV_RETR_LIST������������
	waitKey(0);
	//��������״������
	//��Ӿ���
	Mat matio[100];
	int num = contours.size();
	for (int i = 0; i < contours.size(); ++i)
	{
		Rect r0 = boundingRect(Mat(contours[i]));
		if (r0.width >= r0.height) continue;
		src(r0).copyTo(matio[i]);
		rectangle(src, r0, Scalar(0), 2);
		imshow("iiiiiiiii", matio[i]);
		//find2(matio[i]);
	}
	imshow("��Ӿ���", src);

	//	findContours(thr,			//ͼ�� 
	//	contours,				//������
	//	//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
	//	CV_RETR_LIST,			//��ȡ�����ķ����������ȡ����������
	//	CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ������
	////��������
	//drawContours(result, contours, -1, Scalar(0), 2);
	//imshow("��ȡ��������", result);
}
int main(int argc, char** argv)
{
	process2();
	waitKey(0);
	return(0);
}
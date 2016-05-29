#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;
/** @������ */
int num[11][9] = { { 0, 1, 1, 1, 0, 1, 1, 1, 1 }, { 0, 1, 1, 0, 1, 1, 0, 1, 1 }, { 1, 0, 1, 1, 0, 1, 0, 1, 0 }, { 0, 1, 0, 0, 1, 1, 0, 0, 1 },
{ 0, 0, 1, 0, 0, 1, 0, 1, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 1, 0, 1, 1, 1, 1 }, { 1, 1, 1, 0, 1, 0, 0, 1, 0 },
{ 0, 0, 1, 0, 1, 1, 1, 0, 0 }, { 1, 1, 1, 1, 0, 1, 0, 0, 0 }, { 0, 1, 1, 1, 0, 1, 1, 1, 0 } };
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
		cout << num << " " << w*h << endl;
		if (num>=w*h / 2) temp[i] = 1, cout << "1" <<",";
		else cout << "0" << ",";
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
	cout << "ʶ����������ǣ�"<<maxn<<" "<<maxsum/9.0 << endl;
}
void process4()
{
	Mat image = imread("HS/numg.png");
	imshow("ԭͼ", image);
	Mat src;
	image(Rect(0,0,image.cols/2,image.rows)).copyTo(src);//��ȡ�������
	for (int i = 0; i<src.rows; i++)//��ֵ������ɫ���£������Ϊ��ɫ
	{
		for (int j = 0; j<src.cols; j++)
		{
			uchar b = src.at<Vec3b>(i, j)[0];
			uchar g = src.at<Vec3b>(i, j)[1];
			uchar r = src.at<Vec3b>(i, j)[2];
			if ((r == 255 && g == 255 && b == 255) || (r == 0 && g == 255 && b == 0) || (r == 255 && g == 0 && b == 0))
			{
				src.at<Vec3b>(i, j)[0] = 255;
				src.at<Vec3b>(i, j)[1] = 255;
				src.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				src.at<Vec3b>(i, j)[0] = 0;
				src.at<Vec3b>(i, j)[1] = 0;
				src.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	imshow("��ȡ���ֲ���", src);

	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);//ת��Ϊ��ͨ�����ڽ���������ȡ
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

	Mat matio;
	int num = contours.size();
	for (int i = 0; i < contours.size(); ++i)
	{
		Rect r0 = boundingRect(Mat(contours[i]));
		if (r0.width >= r0.height) continue;
		src(r0).copyTo(matio);
		rectangle(src, r0, Scalar(255), 2);
		imshow("", matio);
		recoNum(matio);
	}
	imshow("��Ӿ���", src);
	waitKey(0);
}
int main(int argc, char** argv)
{
	process4();

	return(0);
}
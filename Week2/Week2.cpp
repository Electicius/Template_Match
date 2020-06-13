
// Week2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "stdafx.h"
#include<string.h>
#include <iostream>   
#include <opencv2/core/core.hpp>   
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
using namespace std;  
using namespace cv;

typedef struct mycircle{
	Point center;    //Բ������
	int r;           //�뾶
	int red;         //��ɫ
	int green;
	int blue;
	int Du;          //��
	int index;       //����
}MyCir;

int Match(Mat temp,Mat src,Mat dst,MyCir &circle){  //ģ��ƥ��ʶ�����
	int m=temp.rows;
	int n=temp.cols;
	int s=1e20,x1,y1;
	int flag = 0;
	for (int i = 0; i < src.rows-m+1; i++){
		if( flag == 1) break;
		for (int j = 0; j <src.cols-n+1; j++){
			 int s1 = 0;
			 flag = 0;
			 for (int a = i; a < i+m; a++){  
				if( flag == 2 ) break;
                for (int b = j; b < j+n; b++){
					s1 += abs((src.at<uchar>(a,b) - temp.at<uchar>(a-i, b-j)));  //���Բ��
					if( s1 > 500 ) {       //��ֵ�Ż���������ֵ��������ǰѭ��
						flag = 2;
						break;
					}              
				}
			 }
			 if( s1 == 0 ){
				s = s1;
				x1 = i; y1 = j;
				flag = 1;
		        break;
			 }
			 else if( s1 < s ){
				s = s1;
				x1 = i; y1 = j;
			 }
		}
	}
	circle.center.y = (int)(x1+m/2);   //��ȡԲ������
	circle.center.x = (int)(y1+n/2);
	circle.r=25;
	return 1;
}

void drawsth(Mat src,MyCir cir){  //�����ֺ�Բ
	if(cir.Du == 1){ cir.red = 241; cir.green = 176; cir.blue = 222; }
	else if (cir.Du == 2) { cir.red = 255; cir.green = 202; cir.blue = 0; }
	else if (cir.Du == 3) { cir.red = 188; cir.green = 222; cir.blue = 176; }
	else  { cir.red = 134; cir.green = 202; cir.blue = 176; }
	circle(src,cir.center,cir.r,Scalar(cir.blue,cir.green,cir.red),-1);
	char *num[9]={"1","2","3","4","5","6","7","8","9"};
	char **num1=num;
	char *text;
	for(int i=1;i<10;i++){
		if(cir.index==i) {text= *(num1+i-1);break;}
	}
	Point center_t;
	center_t.x=cir.center.x-10;
	center_t.y=cir.center.y+8;
	cv::putText(src,text,center_t,cv::FONT_HERSHEY_COMPLEX,1, cv::Scalar(0, 0, 0),2, 8, 0);
	//imshow("img",src);
	waitKey(0);
}


void gray(Mat& M)   //��ֵ��
{
	int nThreshold = 0;
	int nNewThreshold = 0;//�ȶ����ȵ���ֵ
	int hist[256];//���岢��ʼ���Ҷ�ͳ������hist
	memset(hist, 0, sizeof(hist));
	int IS1, IS2;//״̬���Խ��Ҷȷ�Ϊ���࣬IS1 IS2�ֱ��ʾ��1��2����������
	double IP1, IP2;//�ֱ�����1��2�������� 
	double meanvalue1, meanvalue2;//�ֱ��� ��1 ��2�ĻҶȾ�ֵ��
	int IterationTimes;//�����������
	int max = 0, min = 255;//��ʼ���Ҷ����ֵ��Сֵ
	for (int counter8 = 0; counter8 < M.rows; counter8++)
	{
		for (int counter9 = 0; counter9 < M.cols; counter9++)
		{
			int grey = M.at<uchar>(counter8, counter9);
			if (grey > max)max = grey;
			if (grey < min)min = grey;
			hist[grey] = hist[grey] + 1;//ÿ������һ�����ص�ĻҶ�ֵ ��Ӧ�Ҷȼ��������ֵ�ͼ�һ
		}
	}
	nNewThreshold = (int)((max + min) / 2);//����ֵ��ʼ��

	for (IterationTimes = 0; nThreshold != nNewThreshold&&IterationTimes < 100; IterationTimes++)//���е������ϸ�����ֵ
	{
		nThreshold = nNewThreshold;
		IP1 = 0.0;
		IP2 = 0.0;
		IS1 = 0;
		IS2 = 0;//�����е�ֵ��ʼ��Ϊ0

		for (int k = min; k < nThreshold; k++)//������1�������غ����ظ���
		{
			IS1 = IS1 + hist[k];//ѭ��ͳ�����ظ���
			IP1 += (double)k*hist[k];//ͳ����1��������
		}
		meanvalue1 = IP1 / IS1;//����Ȩƽ��  �ҶȾ�ֵ
		for (int l = nThreshold + 1; l <= max; l++)//������2�������غ����ظ���
		{
			IS2 = IS2 + hist[l];//ѭ��ͳ�����ظ���
			IP2 += (double)l*hist[l];//ͳ����2��������
		}
		meanvalue2 = IP2 / IS2;//������2��ֵ��

		nNewThreshold = (int)((meanvalue1 + meanvalue2) / 2);//���ȵ���ֵ
	}
	for (int count4 = 0; count4 < M.rows; count4++)
	{
		for (int count5 = 0; count5 < M.cols; count5++)
		{
			if (M.at<uchar>(count4, count5) < nThreshold&&M.at<uchar>(count4, count5) > 0) M.at<uchar>(count4, count5) = 0;
			if (M.at<uchar>(count4, count5) >= nThreshold&&M.at<uchar>(count4, count5) > 0) M.at<uchar>(count4, count5) = 255;
		}
	}
	imshow("��ֵ��", M);
	waitKey(0);
}

int ChangeWidth( int width ){    //�����߿��ֵ��������߿��
	if( width >0 && width <=8 ) width = 4;
	if( width >8 && width <=24) width = 12;
	if( width >24 && width <=48) width = 32;
	return width;
}

int main() {
	MyCir *circle;
	circle = new MyCir[9];
	Mat *temp;
	temp = new Mat[9];
	Mat src = imread("src.bmp", 0);
	char **filepath;
	char *path[9] = { "1temp.bmp","2temp.bmp","3temp.bmp","4temp.bmp","5temp.bmp","6temp.bmp","7temp.bmp","8temp.bmp","9temp.bmp" };
	filepath = path;
	for (int i = 0; i < 9; i++) {
		*(temp + i) = imread(*(filepath + i), 0);
	}

	
	Mat dst;
	dst.create(src.rows, src.cols, CV_8UC3);
	dst = imread("background.bmp");
	for (int i = 0; i < 6; i++) {
		int maxi = 0, maxj = 0;
		Match(*(temp + i), src, dst, *(circle + i));
		(circle + i)->index = i + 1;
	}
	imwrite("dst.bmp", dst);

	for( int i = 0; i < 6 ; i++){
		cout<<"��"<<i+1<<"��Բ������"<<": "<<(circle + i)->center.x<<" , "<<(circle + i)->center.y<<endl;
	}
	Mat M = src.clone();
	Mat M0;
	Mat M1 = dst.clone();
	Canny(M, M0, 50, 200, 3);//ԭͼ���б�Ե���
	//imshow("��Ե���",M0);
	waitKey(0);
	gray(M);

	Mat Minf = M.clone();

	vector<Vec4i> lines;
	HoughLinesP(M0, lines, 1, CV_PI / 180, 20, 20, 12);//���л���ֱ�߼��
	for (size_t i = 0; i < lines.size(); i++)
	{
		line(dst, Point(lines[i][0], lines[i][1]),Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
	}
	int min = 999999;
	imshow("����ֱ�߼��.bmp", dst);
	waitKey(0);
	int linetable[10][10] = { 0 };   //��ʾ��ͨ��
	for (size_t i = 0; i < lines.size(); i++)
	{
		int x;
		int y;
		int line_number[2];
		float distance;//�������������ֱ�ߵľ���
		for (int j = 0;j<9;j++)
		{
			
				 x = circle[j].center.x;
				 y = circle[j].center.y;
				distance = powf(lines[i][0] - x, 2) + powf(lines[i][1] - y, 2);
				distance = sqrtf(distance);
				if (distance < min)
				{
					min = distance;
					line_number[0] = j;	
				}
		}
		lines[i][0] = circle[line_number[0]].center.x;
		lines[i][1] = circle[line_number[0]].center.y;
		
		min = 999999;
		for (int j = 0; j<9; j++){
			x = circle[j].center.x;
			y = circle[j].center.y;
			distance = powf(lines[i][2] - x, 2) + powf(lines[i][3] - y, 2);
			distance = sqrtf(distance);
			if (distance < min){
				min = distance;
				line_number[1] = j;			
			}
		}
		lines[i][2] = circle[line_number[1]].center.x;
		lines[i][3] = circle[line_number[1]].center.y;
		min = 999999;
		linetable[line_number[0]][line_number[1]] = 1;
		linetable[line_number[1]][line_number[0]] = 1;
   }
	for (int i = 0; i < 9; i++){         //��ȡ��
		int num = 0;
		for (int j = 0; j < 9; j++){
			if (j == i){
				continue;
			}
			num += linetable[i][j];
		}
		(circle+i)->Du = num;
	}

	int width[10][10] = { 0 };  //�洢�߿��
	int dis[10][10] = { 0 };    //�洢ֱ�߳���
	int iw = 0; 
	int jstart = 0;
	int jend = 0;
	double dist = 0;
	double sin = 0.0;
	for (int i = 0; i < 9; i++)           //��ȡ�߿�ͳ���
	{
		for (int j = 0; j < 9; j++)
		{
			if( linetable[i][j] > 0 && i != j  ){
				iw =  ( circle[i].center.y + circle[j].center.y ) / 2;
				jstart = (circle[i].center.x-circle[j].center.x)<0?circle[i].center.x:circle[j].center.x;
				jend  = (circle[j].center.x-circle[i].center.x)>0?circle[j].center.x:circle[i].center.x;
				for( int jw = jstart; jw < jend; jw++ ){
					if( Minf.at<uchar>(iw,jw) == 0 )  width[i][j] ++;
				}
				dis[i][j] = powf(circle[i].center.x - circle[j].center.x, 2) + powf(circle[i].center.y - circle[j].center.y,2);
				dis[i][j] = sqrt(double(dis[i][j])) - 2 * 25;
				dis[j][i] = dis[i][j];

				sin = double( abs(circle[i].center.y - circle[j].center.y ) ) / ( dis[i][j] + 50) ;
				width[i][j] = int ( width[i][j] * sin );
			}
		}
	}

	for (int i = 0; i < 6; i++)           
	{
		for (int j = 0; j < 6; j++)
		{
			if( linetable[i][j] > 0 && i != j ){   //����
				line(M1, circle[i].center, circle[j].center, Scalar(105,105,105),ChangeWidth(width[i][j]), 8);
			} 
		}
	}

	imshow("��ͨ.bmp", M1);
	waitKey(0);
	for (int i = 0; i < 6; i++) {
		drawsth(M1, *(circle + i));
	}
	imshow("img",M1);
	waitKey(0);
	int xh = 0;
	int yh = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if( linetable[i][j] > 0 && i != j ){
				Point half;
				half.x = (circle[i].center.x + circle[j].center.x) / 2;
				half.y = (circle[i].center.y + circle[j].center.y) / 2;
				Point center_t;
				center_t.x=half.x+10;
				center_t.y=half.y+8;
				char str[12];
				itoa(dis[i][j],str,10);
				cv::putText(M1,str,center_t,cv::FONT_HERSHEY_COMPLEX,0.5, cv::Scalar(0, 0, 255),1.2, 8, 0);
			} 
		}
	}
	imshow("Final.bmp", M1);
	imwrite("Final.bmp", M1);
	waitKey(0);
}












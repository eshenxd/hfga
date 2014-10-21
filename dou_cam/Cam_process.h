#pragma once


#include "header.h"

struct CamPara
{
	int hour,minute,second;
	int FD_iterations;
	int show_width,show_height;
	int Align_iterations;
	int pic_width,pic_height;
	float window_scale,window_scale1;

	CvCapture* capture1;
	CvCapture* capture2;
	IplImage *frame1,*frame2;
	IplImage* show_img;
	CvRect area1,area2;
	CvRect regions1,regions2;

	int Frame_num;//֡��
	int Frame_num2;

	 
};

class CamProcess
{
public:
	int process_para_inti();//����0��ʾ��ʼ���ɹ���-1���ʾʧ��

	int process_operate(CamPara*);

	void process_release();

protected:

private:
	CvCapture* capture1;
	CvCapture* capture2;
	IplImage *frame1,*frame2;
	IplImage* show_img;
	CvRect area1,area2;
	CvRect regions1,regions2;

};

static unsigned __stdcall process1(void* );//����ͷ1���̴߳�����
static unsigned __stdcall process2(void* );//����ͷ2���̴߳�����
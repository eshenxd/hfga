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

	int Frame_num;//帧数
	int Frame_num2;

	 
};

class CamProcess
{
public:
	int process_para_inti();//返回0表示初始化成功，-1则表示失败

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

static unsigned __stdcall process1(void* );//摄像头1新线程处理函数
static unsigned __stdcall process2(void* );//摄像头2新线程处理函数
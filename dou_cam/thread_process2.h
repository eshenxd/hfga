#pragma once

#include "header.h"
#include "Cam_process.h"
#include "detect_and_draw.h"



class Process2
{
public:
	Process2(CamPara*);//参数是参数集合

	~Process2();

	bool same_time();

	int camProcess();

	int face_detectAnddraw();

protected:
private:
	CamPara* P_para;

	FILE* stream_1;
	char* cascade_name;
	CvHaarClassifierCascade* cascade;

	int num; //检测到对齐的人脸数
	int savenum;//已保存的人脸数
	int a[2];

	SYSTEMTIME sys;

	IplImage* fram_copy;
	IplImage* fram_copy1;
};




#pragma once

#include "header.h"
#include "Cam_process.h"
#include "detect_and_draw.h"



class Process1
{
public:
	Process1(CamPara*);//参数1是参数集合，参数2是face.xml的存储路径

	~Process1();
	
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
#pragma once

#include "header.h"
#include "Cam_process.h"



class config_HFGA
{
public:
	int Read_configfiles();//�������ļ�������ã�����0��ʾ���óɹ���-1ʧ��
	void free_config();
	bool config_same();
	
	
	CamPara* get_config();//��ȡ�����ļ�����
protected:
private:
	//int tmp;
	int hour,minute,second;
	int FD_iterations;
	int show_width,show_height;
	int Align_iterations;
	int pic_width,pic_height;
	float window_scale,window_scale1;
};
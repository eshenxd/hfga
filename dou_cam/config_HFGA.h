#pragma once

#include "header.h"
#include "Cam_process.h"



class config_HFGA
{
public:
	int Read_configfiles();//读配置文件完成配置，返回0表示配置成功，-1失败
	void free_config();
	bool config_same();
	
	
	CamPara* get_config();//获取配置文件参数
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
/**************************************
      功能：hfga双摄像头采集程序（new）
	        为解决xp系统不兼容问题而重
			写
	  时间：2014年9月11日
	  作者：Ethan
	  问题：保存图片数定在5，但是如果
	        检测时间短，保存数会小于5，
			没有对这种文件数少的情况进
			行处理
**************************************/
#include "header.h"
#include "config_HFGA.h"
#include "Cam_process.h"



int main(int argc , char* argv[])
{
	CamPara* para=new CamPara;

	config_HFGA* config=new config_HFGA;

	config->Read_configfiles();

	para=config->get_config();

	CamProcess* cam=new CamProcess;

	

	cam->process_para_inti();

	cam->process_operate(para);

	system("pause");

	return 0;
}
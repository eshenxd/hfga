/**************************************
      ���ܣ�hfga˫����ͷ�ɼ�����new��
	        Ϊ���xpϵͳ�������������
			д
	  ʱ�䣺2014��9��11��
	  ���ߣ�Ethan
	  ���⣺����ͼƬ������5���������
	        ���ʱ��̣���������С��5��
			û�ж������ļ����ٵ������
			�д���
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
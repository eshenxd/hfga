/**************************************
      功能：摄像头处理函数
	  时间：2014年9月9日
	  作者：Ethan
**************************************/

#include "Cam_process.h"
#include "thread_process1.h"
#include "thread_process2.h"


int CamProcess::process_para_inti()
{
	
	frame1=NULL;

	frame2=NULL;
	
	capture1=cvCaptureFromCAM(0);

	capture2=cvCaptureFromCAM(1);

	if(!capture1||!capture2)
	{
		cout<<"请接入摄像头！"<<endl;

		return -1;
	}

	area1=cvRect(0,0,0,0);

	area2=cvRect(0,0,0,0);

	regions1=cvRect(0,0,0,0);

	regions2=cvRect(0,0,0,0);

	return 0;
}

int CamProcess::process_operate(CamPara* para)
{
	para->capture1=capture1;

	para->capture2=capture2;

	para->frame1=frame1;

	para->frame2=frame2;

	para->area1=area1;

	para->area2=area2;

	para->regions1=regions1;

	para->regions2=regions2;

	para->Frame_num=1;

	para->Frame_num2=1;


	HANDLE hThread1;
	HANDLE hThread2;

	unsigned uiThread1ID;
	unsigned uiThread2ID;

	hThread1=(HANDLE)_beginthreadex(NULL,NULL,process1,para,0,&uiThread1ID);
	hThread2=(HANDLE)_beginthreadex(NULL,NULL,process2,para,0,&uiThread2ID);

	WaitForSingleObject(hThread1,INFINITE); 
	WaitForSingleObject(hThread2,INFINITE); //INFINITE表示等待无限长,单位为毫秒

	
	//_endthread();
	//_endthreadex(uiThread1ID);
	//_endthread(uiThread2ID);

	CloseHandle(hThread1);
	CloseHandle(hThread2);
    
	return 0;

}


static unsigned __stdcall process1(void* pThis)
{
	cout<<"thread1 start!"<<endl;

	CamPara* para=(CamPara*)pThis;

	Process1* cam1Process=new Process1(para);

	for(;;)
	{
		int flag=-1;
		flag=cam1Process->camProcess();

		if(flag==-1)break;
	}
	

	return 0;
}

static unsigned __stdcall process2(void* pThis)
{
	cout<<"thread2 start!"<<endl;

	CamPara* para2=(CamPara*)pThis;

	Process2* cam2Process=new Process2(para2);

	for(;;)
	{
		int flag=-1;
		flag=cam2Process->camProcess();

		if(flag==-1)break;
	}

	return 0;
}
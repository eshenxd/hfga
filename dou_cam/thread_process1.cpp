/**************************************
      ���ܣ��������̵߳Ľӿں���
	  ʱ�䣺2014��9��9��
	  ���ߣ�Ethan
**************************************/

#include "thread_process1.h"
#include "FaceAlign.h"
#include "GetLocalIPAddress.h"


Process1::Process1(CamPara* p)
{
	P_para=p;

	cascade_name="face.xml";

	cascade  = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
}

Process1::~Process1()
{

}

bool Process1::same_time()
{
	
	GetLocalTime( &sys );

	if (((sys.wHour==P_para->hour)&&(sys.wMinute==P_para->minute)&&(sys.wSecond==P_para->second)))return true;
	else return false;
}

int Process1::camProcess()
{
	bool flag=false;

	flag=same_time();

	if(flag)
	{
		WinExec("�ϴ�������2.bat", SW_HIDE );//�ϴ��ļ�������
		Sleep(1000);
	}

	P_para->frame1=cvQueryFrame(P_para->capture1);

	if(!P_para->frame1)
	{
		cout<<"û������ͷ����..."<<endl;
		return -1;
	}

	fram_copy=cvCreateImage(cvSize(P_para->frame1->width,P_para->frame1->height),
		P_para->frame1->depth,P_para->frame1->nChannels);

	fram_copy1=cvCreateImage(cvSize(P_para->frame1->width,P_para->frame1->height),
		P_para->frame1->depth,P_para->frame1->nChannels);

	cvCopy(P_para->frame1,fram_copy);

	cvCopy(P_para->frame1,fram_copy1);

	//����Ƿ��⵽����,��a[0],a[1]�洢
	//0��ʾû��⵽������1��ʾ��⵽����

	
	if(P_para->Frame_num==1/*��ʾ��һ�μ�⵽����*/)
	{
		a[0]=0;

		num=0;

		int faces_total=face_detectAnddraw();

		if (faces_total)
		{
			a[1]=1;
		}
		else
			a[1]=0;
	}
	else
	{
		a[0]=a[1];
		int faces_total=face_detectAnddraw();

		if (faces_total)
		{
			a[1]=1;
		}
		else
		{
			a[1]=0;
			P_para->Frame_num=1;
		}
	}


	//���������ͼƬ
	//��a[0]=0,a[1]=1ʱ����Ҫ�����µ��ļ��У�˵�������µ�������
	//��Ҳ��������⣬�ļ����е��ļ���һ����٣�
	//��a[0]=1,a[1]=1ʱ��˵����⵽������ͬһ���ˣ�����ͬһ���ļ����У�
	//���������Ϊû��⵽����������Ҫ���棬ֱ�ӽ�����һ֡
	if((a[0]==0)&&(a[1]==1))
	{
		//��������
		IplImage *faces=cvCreateImage(cvSize(P_para->area1.width,P_para->area1.height),P_para->frame1->depth,P_para->frame1->nChannels);
		cvSetImageROI(fram_copy1,P_para->area1);
		cvCopy(fram_copy1,faces);
		//cvSaveImage(Save_path_1,faces);
		int flag_estimate=0;
		flag_estimate=align_pic(faces,P_para->regions1,P_para->Align_iterations,num+1);//��������
		//image=align_pic(frame_copy_1);//��������
		if (flag_estimate)//��⵽����Ҫ�������
		{
			num++;
		}
		else//��ֹѡ�������鲻������
		{
			//num=0;
			a[1]=0;//˵����⵽���Ƿ���������Ҫ���¼��ʹ����µ��ļ���
		}
		//cvReleaseImage(&image);
		P_para->Frame_num++;

		cvResetImageROI(fram_copy1);
		cvReleaseImage(&faces);
	}


	else if((a[0]==1)&&(a[1]==1))
	{
		//��������
		
		IplImage *faces=cvCreateImage(cvSize(P_para->area1.width,P_para->area1.height),P_para->frame1->depth,P_para->frame1->nChannels);
		cvSetImageROI(fram_copy1,P_para->area1);
		cvCopy(fram_copy1,faces);


		if (1/*flag_estimate*/)
		{
			num++;

			IP* ip=new IP;

			string LIp=ip->getIP();

			if (num==6)//��ֹ�����յ��ļ���,����Ϊ��ʾ��ͼƬ
			{
				//�������Ȥ�����ͼƬ

				char path_1[50];

				if((stream_1=fopen("setfile_1.ini","r"))==NULL)
					cout<<"setfile_1.ini�򿪳�������"<<endl;
				else
				{
					fscanf(stream_1,"%s",path_1);
					fclose(stream_1);
				}

				/*
				    ��������Ա���IP��ַ�����ļ��еĴ���
				*/
				
				

				char *strDir_1=path_1;

				sprintf(strDir_1,"%s%s%s",strDir_1,LIp.c_str(),"\\\\");
				if(CreateDirectory(strDir_1,0))//�����ļ���
					cout<<"Creat new file_1!"<<endl;

				
				unsigned long long filesum_1;
				if((stream_1=fopen("filesum_1.ini","r"))==NULL)
					cout<<"filesum_1.ini�򿪳�������"<<endl;
				else
				{
					fscanf(stream_1,"%llu",&filesum_1);
					sprintf(strDir_1,"%s%llu%s",strDir_1,filesum_1,"\\\\");
					if(CreateDirectory(strDir_1,0))//�����ļ���
						cout<<"Creat new file_1!"<<endl;
					fclose(stream_1);
				}

				//�����Ѵ������ļ�����Ŀ
				filesum_1++;
				stream_1=fopen("filesum_1.ini","w");
				fprintf(stream_1,"%llu",filesum_1);
				fclose(stream_1);
			}//�������������½��ļ���

			if ((num>=6)&&(num<=18)&&(num%3==0))//ֻ�洢5��ͼƬ
			{


				//����ԭʼͼƬ

				char path_1[50];

				if((stream_1=fopen("setfile_1.ini","r"))==NULL)
					cout<<"setfile_1.ini�򿪳�������"<<endl;
				else
				{
					fscanf(stream_1,"%s",path_1);
					fclose(stream_1);
				}
				char *strDir_1=path_1;
				sprintf(strDir_1,"%s%s%s",strDir_1,LIp.c_str(),"\\\\");


				char Save_path_1[100];
				unsigned long long filesum_1;
				if((stream_1=fopen("filesum_1.ini","r"))==NULL)
					cout<<"filesum_1.ini�򿪳�������"<<endl;
				else
				{
					fscanf(stream_1,"%llu",&filesum_1);
					filesum_1--;
					sprintf(strDir_1,"%s%llu%s",strDir_1,filesum_1,"\\\\");
					fclose(stream_1);
				}

				GetLocalTime( &sys ); 
				sprintf(Save_path_1,"%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s",strDir_1,sys.wYear,"-",sys.wMonth,"-",sys.wDay,"-",
					sys.wHour,"-",sys.wMinute,"-",sys.wSecond,"-",sys.wMilliseconds,".jpg");

				
				cvSaveImage(Save_path_1,faces);//�������Ȥ�����ͼƬ

				savenum++;

				
			}//���ֵ���ͬһ���ˣ���������ͼƬ
			if(num>18&&savenum==5&&a[1]==0)
			{
				a[0]=a[1]=0;

				P_para->Frame_num=1;

				num=0;
			}//�Ѵ���5��ͼƬ��������ǰ����
			
			
			delete ip;

		}
		

		cvResetImageROI(fram_copy1);
		cvReleaseImage(&faces);
		}

	cvReleaseImage(&fram_copy);

	cvReleaseImage(&fram_copy1);

	return 0;
}

int Process1::face_detectAnddraw()
{
	CvMemStorage* storage= cvCreateMemStorage(0);

	P_para->area1.x=cvRound(fram_copy->width*P_para->window_scale);

	P_para->area1.y=cvRound(fram_copy->height*P_para->window_scale);

	P_para->area1.width=cvRound(fram_copy->width*(1-P_para->window_scale*2));

	P_para->area1.height=cvRound(fram_copy->height*(1-P_para->window_scale*2));


	IplImage *img_ROI=cvCreateImage(cvSize(P_para->area1.width,P_para->area1.height),fram_copy->depth,fram_copy->nChannels);
	
	CvPoint p1,p2;

	p1.x=P_para->area1.x;

	p1.y=P_para->area1.y;

	p2.x=P_para->area1.x+P_para->area1.width;

	p2.y=P_para->area1.y+P_para->area1.height;


   
	cvSetImageROI(fram_copy,P_para->area1);

	cvCopy(fram_copy,img_ROI);

	cvResetImageROI(fram_copy);

	//��ʾ"Interest Area"
	CvFont font;    
	double hScale=1;   
	double vScale=1;    
	int lineWidth=2;// �൱��д�ֵ�����    
	char showMsg[]="IA"; //Interest area1
	// ��ʼ������   
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ�   
	// cvPoint Ϊ��ʵ�x��y����   
	cvPutText(fram_copy,showMsg,cvPoint(P_para->area1.x,P_para->area1.y-10),&font,CV_RGB(255,100,255));//��ͼƬ������ַ� 

	cvRectangle(fram_copy,p1,p2,CV_RGB(0,0,255),1,8,0);


	double Scale = 1.3;

	IplImage* gray = cvCreateImage( cvSize(img_ROI->width,img_ROI->height),img_ROI->depth, 1 );

	IplImage* small_img = cvCreateImage( cvSize( cvRound (img_ROI->width/Scale),
		cvRound (img_ROI->height/Scale)),
		img_ROI->depth, 1 );

	IplImage *img_show=cvCreateImage(cvSize(P_para->show_width,P_para->show_height),fram_copy->depth,fram_copy->nChannels);

	int i,faces_num;
	int  x1, y1, x2, y2, w, h;

	cvCvtColor( img_ROI, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	cvClearMemStorage( storage );

	CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
		1.2, P_para->FD_iterations, CV_HAAR_DO_CANNY_PRUNING,
		cvSize(50, 50) );

	faces_num=faces->total;

	if (faces->total==1)
	{
		for( i = 0; i < (faces ? faces->total : 0); i++ )
		{
			CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
			CvPoint center;
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*Scale);
			center.y = cvRound((r->y + r->height*0.5)*Scale);
			radius = cvRound((r->width + r->height)*0.25*Scale);
			CvPoint p1,p2;
			p1.x=center.x-radius+P_para->area1.x;
			p1.y=center.y-radius+P_para->area1.y;
			p2.x=center.x+radius+P_para->area1.x;
			p2.y=center.y+radius+P_para->area1.y+cvRound(2*radius*0.1);//����ֵcvRound(2*radius*0.1)

			if ((num>0)&&(num<6))
				cvRectangle(fram_copy,p1,p2,CV_RGB(0,255,255),1,8,0);//���ϵ���ͼ��
			//cvCircle( img_ROI, center, radius, colors[i%8], 3, 8, 0 );
			if ((num>=6)&&(num<34))
				cvRectangle(fram_copy,p1,p2,CV_RGB(0,255,0),1,8,0);//���ϵ���ͼ��
			if(num>34)
				cvRectangle(fram_copy,p1,p2,CV_RGB(255,0,0),1,8,0);//���ϵ���ͼ��

			//����������λ��
			x1 = p1.x-P_para->area1.x;
			x2 = p2.x-P_para->area1.x;
			y1 = p1.y-P_para->area1.y;
			y2 = p2.y-P_para->area1.y;
			w = x2 - x1;
			h = y2 - y1;

			/* ensure odd width and height */
			w = ( w % 2 )? w : w+1;
			h = ( h % 2 )? h : h+1;

			P_para->regions1.x=x1;
			P_para->regions1.y=y1;
			P_para->regions1.width=w;
			P_para->regions1.height=h;
		}

		//��ʾ"Prepare Begin or OK"
		if ((num>0)&&(num<6))//Ԥ���׶�
		{
			char  showMsg[]="Prepare..."; 
			cvPutText(fram_copy,showMsg,cvPoint(P_para->area1.x+P_para->regions1.x+P_para->regions1.width,P_para->area1.y+P_para->regions1.y),&font,CV_RGB(0,255,255));//��ͼƬ������ַ� 
		}

		if ((num>=6)&&(num<18))//�ɼ���10����Ƭ
		{
			char  showMsg[]="Begin..."; 
			cvPutText(fram_copy,showMsg,cvPoint(P_para->area1.x+P_para->regions1.x+P_para->regions1.width,P_para->area1.y+P_para->regions1.y),&font,CV_RGB(0,255,0));//��ͼƬ������ַ� 
		}
		if(num>18)//�ɼ���35����Ƭ,����
		{
			char showMsg[]="OK...";
			cvPutText(fram_copy,showMsg,cvPoint(P_para->area1.x+P_para->regions1.x+P_para->regions1.width,P_para->area1.y+P_para->regions1.y),&font,CV_RGB(255,0,0));//��ͼƬ������ַ� 
		}

	}
	cvClearSeq(faces);

	cvResize(fram_copy,img_show);//�Ŵ�����
	cvShowImage( "�����ɼ�1", img_show);
	cvWaitKey(2);
	cvReleaseImage(&img_show);
	cvReleaseImage( &gray );
	cvReleaseImage( &small_img );
	cvReleaseImage(&img_ROI);

	cvReleaseMemStorage(&storage);

	if (faces_num==1)//ֻ����һ������
		return faces_num;
	else
		return 0;
}
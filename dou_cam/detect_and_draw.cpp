/***************************************
	
	��������ͼ���ʾ

	***************************************/

#include "detect_and_draw.h"
#include "header.h"
#include "cam_para.h"
#include "config.h"
#include "cam_process.h"

char* cascade_name="face.xml";
CvHaarClassifierCascade* cascade  = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
CvHaarClassifierCascade* cascade1  = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );

int detect_and_draw( IplImage* img,int num,unsigned int handle_config,unsigned int handle_para/*float Windows_scale,int FD_iterarions,int show_width,int show_height,CvRect area,CvRect regions*/)
{
	//CvRect area; 
	//CvRect regions;
	Config_HFGA* config=(Config_HFGA*)handle_config;
	CamPara* para=(CamPara*)handle_para;
	/*char* cascade_name="face.xml";*//*    "haarcascade_profileface.xml";*/

	CvMemStorage* storage= cvCreateMemStorage(0);
	/*CvHaarClassifierCascade* cascade  = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );*/
	//------�������Ȥ����-------
	para->area1.x=cvRound(img->width*config->window_scale);
	para->area1.y=cvRound(img->height*config->window_scale);
	para->area1.width=cvRound(img->width*(1-config->window_scale*2));
	para->area1.height=cvRound(img->height*(1-config->window_scale*2));

	IplImage *img_ROI=cvCreateImage(cvSize(para->area1.width,para->area1.height),img->depth,img->nChannels);
	CvPoint p1,p2;
	p1.x=para->area1.x;
	p1.y=para->area1.y;
	p2.x=para->area1.x+para->area1.width;
	p2.y=para->area1.y+para->area1.height;

	cvSaveImage("1.jpg",img);
	cvSetImageROI(img,para->area1);
	cvCopy(img,img_ROI);
	cvShowImage("test",img_ROI);
	cvWaitKey(0);

	cvResetImageROI(img);

	//��ʾ"Interest Area"
	CvFont font;    
	double hScale=1;   
	double vScale=1;    
	int lineWidth=2;// �൱��д�ֵ�����    
	char showMsg[]="IA"; //Interest area1
	// ��ʼ������   
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ�   
	// cvPoint Ϊ��ʵ�x��y����   
	cvPutText(img,showMsg,cvPoint(para->area1.x,para->area1.y-10),&font,CV_RGB(255,100,255));//��ͼƬ������ַ� 

	cvRectangle(img,p1,p2,CV_RGB(0,0,255),1,8,0);

	double Scale = 1.3;
	IplImage* gray = cvCreateImage( cvSize(img_ROI->width,img_ROI->height),img_ROI->depth, 1 );
	IplImage* small_img = cvCreateImage( cvSize( cvRound (img_ROI->width/Scale),
		cvRound (img_ROI->height/Scale)),
		img_ROI->depth, 1 );
	IplImage *img_show=cvCreateImage(cvSize(config->show_width,config->show_height),img->depth,img->nChannels);

	int i,faces_num;
	int  x1, y1, x2, y2, w, h;

	cvCvtColor( img_ROI, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	cvClearMemStorage( storage );


	CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
		1.2, config->FD_iterations, CV_HAAR_DO_CANNY_PRUNING,
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
			p1.x=center.x-radius+para->area1.x;
			p1.y=center.y-radius+para->area1.y;
			p2.x=center.x+radius+para->area1.x;
			p2.y=center.y+radius+para->area1.y+cvRound(2*radius*0.1);//����ֵcvRound(2*radius*0.1)

			if ((num>0)&&(num<6))
				cvRectangle(img,p1,p2,CV_RGB(0,255,255),1,8,0);//���ϵ���ͼ��
			//cvCircle( img_ROI, center, radius, colors[i%8], 3, 8, 0 );
			if ((num>=6)&&(num<34))
				cvRectangle(img,p1,p2,CV_RGB(0,255,0),1,8,0);//���ϵ���ͼ��
			if(num>34)
				cvRectangle(img,p1,p2,CV_RGB(255,0,0),1,8,0);//���ϵ���ͼ��

			//����������λ��
			x1 = p1.x-para->area1.x;
			x2 = p2.x-para->area1.x;
			y1 = p1.y-para->area1.y;
			y2 = p2.y-para->area1.y;
			w = x2 - x1;
			h = y2 - y1;

			/* ensure odd width and height */
			w = ( w % 2 )? w : w+1;
			h = ( h % 2 )? h : h+1;

			para->regions1.x=x1;
			para->regions1.y=y1;
			para->regions1.width=w;
			para->regions1.height=h;
		}

		//��ʾ"Prepare Begin or OK"
		if ((num>0)&&(num<6))//Ԥ���׶�
		{
			char  showMsg[]="Prepare..."; 
			cvPutText(img,showMsg,cvPoint(para->area1.x+para->regions1.x+para->regions1.width,para->area1.y+para->regions1.y),&font,CV_RGB(0,255,255));//��ͼƬ������ַ� 
		}

		if ((num>=6)&&(num<34))//�ɼ���10����Ƭ
		{
			char  showMsg[]="Begin..."; 
			cvPutText(img,showMsg,cvPoint(para->area1.x+para->regions1.x+para->regions1.width,para->area1.y+para->regions1.y),&font,CV_RGB(0,255,0));//��ͼƬ������ַ� 
		}
		if(num>34)//�ɼ���35����Ƭ,����
		{
			char showMsg[]="OK...";
			cvPutText(img,showMsg,cvPoint(para->area1.x+para->regions1.x+para->regions1.width,para->area1.y+para->regions1.y),&font,CV_RGB(255,0,0));//��ͼƬ������ַ� 
		}

	}

	cvClearSeq(faces);

	cvResize(img,img_show);//�Ŵ�����
	cvShowImage( "�����ɼ�1", img_show);
	//cvWaitKey(0);
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



int detect_and_draw1( IplImage* img,int num,void* pThis)
{
	//CvRect area; 
	//CvRect regions;
	Config_para* para=(Config_para*)pThis;
	//char* cascade_name="face.xml";/*    "haarcascade_profileface.xml";*/

	CvMemStorage* storage= cvCreateMemStorage(0);
	//CvHaarClassifierCascade* cascade  = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
	//------�������Ȥ����-------
	para->area2.x=cvRound(img->width*para->window_scale1);
	para->area2.y=cvRound(img->height*para->window_scale1);
	para->area2.width=cvRound(img->width*(1-para->window_scale1*2));
	para->area2.height=cvRound(img->height*(1-para->window_scale1*2));

	IplImage *img_ROI=cvCreateImage(cvSize(para->area2.width,para->area2.height),img->depth,img->nChannels);
	CvPoint p1,p2;
	p1.x=para->area2.x;
	p1.y=para->area2.y;
	p2.x=para->area2.x+para->area2.width;
	p2.y=para->area2.y+para->area2.height;

	cvSetImageROI(img,para->area2);
	cvCopy(img,img_ROI);
	cvResetImageROI(img);

	//��ʾ"Interest para->area2"
	CvFont font;    
	double hScale=1;   
	double vScale=1;    
	int lineWidth=2;// �൱��д�ֵ�����    
	char showMsg[]="IA"; //Interest para->area2
	// ��ʼ������   
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ�   
	// cvPoint Ϊ��ʵ�x��y����   
	cvPutText(img,showMsg,cvPoint(para->area2.x,para->area2.y-10),&font,CV_RGB(255,100,255));//��ͼƬ������ַ� 

	cvRectangle(img,p1,p2,CV_RGB(0,0,255),1,8,0);

	double Scale = 1.3;
	IplImage* gray = cvCreateImage( cvSize(img_ROI->width,img_ROI->height),img_ROI->depth, 1 );
	IplImage* small_img = cvCreateImage( cvSize( cvRound (img_ROI->width/Scale),
		cvRound (img_ROI->height/Scale)),
		img_ROI->depth, 1 );
	IplImage *img_show=cvCreateImage(cvSize(para->show_width,para->show_height),img->depth,img->nChannels);

	int i,faces_num;
	int  x1, y1, x2, y2, w, h;

	cvCvtColor( img_ROI, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	cvClearMemStorage( storage );


	CvSeq* faces = cvHaarDetectObjects( small_img, cascade1, storage,
		1.2,para->FD_iterations, CV_HAAR_DO_CANNY_PRUNING,
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
			p1.x=center.x-radius+para->area2.x;
			p1.y=center.y-radius+para->area2.y;
			p2.x=center.x+radius+para->area2.x;
			p2.y=center.y+radius+para->area2.y+cvRound(2*radius*0.1);//����ֵcvRound(2*radius*0.1)

			if ((num>0)&&(num<6))
				cvRectangle(img,p1,p2,CV_RGB(0,255,255),1,8,0);//���ϵ���ͼ��
			//cvCircle( img_ROI, center, radius, colors[i%8], 3, 8, 0 );
			if ((num>=6)&&(num<34))
				cvRectangle(img,p1,p2,CV_RGB(0,255,0),1,8,0);//���ϵ���ͼ��
			if(num>34)
				cvRectangle(img,p1,p2,CV_RGB(255,0,0),1,8,0);//���ϵ���ͼ��

			//����������λ��
			x1 = p1.x-para->area2.x;
			x2 = p2.x-para->area2.x;
			y1 = p1.y-para->area2.y;
			y2 = p2.y-para->area2.y;
			w = x2 - x1;
			h = y2 - y1;

			/* ensure odd width and height */
			w = ( w % 2 )? w : w+1;
			h = ( h % 2 )? h : h+1;

			para->regions2.x=x1;
			para->regions2.y=y1;
			para->regions2.width=w;
			para->regions2.height=h;
		}

		//��ʾ"Prepare Begin or OK"
		if ((num>0)&&(num<6))//Ԥ���׶�
		{
			char  showMsg[]="Prepare..."; 
			cvPutText(img,showMsg,cvPoint(para->area2.x+para->regions2.x+para->regions2.width,para->area2.y+para->regions2.y),&font,CV_RGB(0,255,255));//��ͼƬ������ַ� 
		}

		if ((num>=6)&&(num<34))//�ɼ���10����Ƭ
		{
			char  showMsg[]="Begin..."; 
			cvPutText(img,showMsg,cvPoint(para->area2.x+para->regions2.x+para->regions2.width,para->area2.y+para->regions2.y),&font,CV_RGB(0,255,0));//��ͼƬ������ַ� 
		}
		if(num>34)//�ɼ���35����Ƭ,����
		{
			char showMsg[]="OK...";
			cvPutText(img,showMsg,cvPoint(para->area2.x+para->regions2.x+para->regions2.width,para->area2.y+para->regions2.y),&font,CV_RGB(255,0,0));//��ͼƬ������ַ� 
		}

	}

	cvClearSeq(faces);

	cvResize(img,img_show);//�Ŵ�����
	cvShowImage( "�����ɼ�2", img_show);
	//cvWaitKey(0);
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
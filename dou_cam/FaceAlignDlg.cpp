/***************************************
	
	人脸对齐

	***************************************/

#include "FaceAlign.h"

char* cascade_name_1=
	"face.xml";/*    "haarcascade_profileface.xml";*/
CvMemStorage* storage_1= cvCreateMemStorage(0);
CvHaarClassifierCascade* cascade_1  = (CvHaarClassifierCascade*)cvLoad( cascade_name_1, 0, 0, 0 );
CvRect Align_rect;
asmfitting fit_asm;

/************************************************************************/
/* 初始化ASM模型，载入人脸检测数据库和ASM模型数据并且初始化旋转
矩阵
/*/
/************************************************************************/
bool init_asm()
{
	if (fit_asm.Read("Point.amf") == false)//my.amf添加在工程的文件夹中
	{
		printf("Can't read Point.amf data !\n");
		return false;
	}
	else
		return true;
}
/************************************************************************/
/*  该函数实现以人眼为基准点进行人脸对齐，主要是旋转和归一化大小
先利用ASM找到人脸68个点，然后再定位到眼睛，进行处理
@ 参数 image 是输入图像，8bit无符号三通道图像
@ 返回值是对齐后的人脸图片
*/
/************************************************************************/
int align_pic(IplImage * image,CvRect rect,int Align_iterations,int flag_num)//评估人脸
{
   if ((flag_num>=6)&&(flag_num<=34)&&(flag_num%3==0))//只做保存的10张评估
   {
	   int nFaces;
	   asm_shape *shapes = NULL, *detshapes = NULL;

	   nFaces=1;
	   detshapes = new asm_shape[nFaces];
	   detshapes[0].Resize(2);
	   detshapes[0][0].x=rect.x;
	   detshapes[0][0].y=rect.y;
	   detshapes[0][1].x=rect.x+rect.width;
	   detshapes[0][1].y=rect.y+rect.height;

	   shapes = new asm_shape[nFaces];
	   for(int i = 0; i < nFaces; i++)
	   {
		   InitShapeFromDetBox(shapes[i], detshapes[i], fit_asm.GetMappingDetShape(), fit_asm.GetMeanFaceWidth());
	   }


	   fit_asm.Fitting2(shapes, nFaces, image, Align_iterations);


	   CvPoint2D32f  left_point = (shapes[0])[31];
	   CvPoint2D32f  right_point = (shapes[0])[36];
	   CvPoint2D32f  nose_point=(shapes[0])[67];

	   CvPoint rota_point = cvPoint(cvRound((left_point.x + right_point.x)/2), 
		   cvRound((left_point.y + right_point.y)/2));
	   float xDis, yDis;
	   xDis = right_point.x - left_point.x;
	   yDis = fabs(right_point.y - left_point.y);

	   float m_angle = cvFastArctan(yDis, xDis);

	   if (m_angle>10)//斜脸
	   {
		   delete[] shapes;
		   free_shape_memeory(&detshapes);
		   return 0;
	   }
	   else if (fabs((left_point.x+right_point.x)/2.0-nose_point.x)/abs(left_point.x+right_point.x)>0.1)//侧脸
	   {
		   delete[] shapes;
		   free_shape_memeory(&detshapes);
		   return 0;
	   }
	   else//采集的人脸符合要求
	   {
		   delete[] shapes;
		   free_shape_memeory(&detshapes);
		   return 1;
	   }

   }
   else
   {
	   return 1;
   }

}
void free_shape_memeory(asm_shape** shapes)
{
	delete [](*shapes);
}
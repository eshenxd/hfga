/***************************************
	
	��������

	***************************************/

#include "FaceAlign.h"

char* cascade_name_1=
	"face.xml";/*    "haarcascade_profileface.xml";*/
CvMemStorage* storage_1= cvCreateMemStorage(0);
CvHaarClassifierCascade* cascade_1  = (CvHaarClassifierCascade*)cvLoad( cascade_name_1, 0, 0, 0 );
CvRect Align_rect;
asmfitting fit_asm;

/************************************************************************/
/* ��ʼ��ASMģ�ͣ���������������ݿ��ASMģ�����ݲ��ҳ�ʼ����ת
����
/*/
/************************************************************************/
bool init_asm()
{
	if (fit_asm.Read("Point.amf") == false)//my.amf����ڹ��̵��ļ�����
	{
		printf("Can't read Point.amf data !\n");
		return false;
	}
	else
		return true;
}
/************************************************************************/
/*  �ú���ʵ��������Ϊ��׼������������룬��Ҫ����ת�͹�һ����С
������ASM�ҵ�����68���㣬Ȼ���ٶ�λ���۾������д���
@ ���� image ������ͼ��8bit�޷�����ͨ��ͼ��
@ ����ֵ�Ƕ���������ͼƬ
*/
/************************************************************************/
int align_pic(IplImage * image,CvRect rect,int Align_iterations,int flag_num)//��������
{
   if ((flag_num>=6)&&(flag_num<=34)&&(flag_num%3==0))//ֻ�������10������
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

	   if (m_angle>10)//б��
	   {
		   delete[] shapes;
		   free_shape_memeory(&detshapes);
		   return 0;
	   }
	   else if (fabs((left_point.x+right_point.x)/2.0-nose_point.x)/abs(left_point.x+right_point.x)>0.1)//����
	   {
		   delete[] shapes;
		   free_shape_memeory(&detshapes);
		   return 0;
	   }
	   else//�ɼ�����������Ҫ��
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
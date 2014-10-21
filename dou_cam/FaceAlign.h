#pragma once

#include "..\opencv\cv.h"
#include "..\opencv\highgui.h"
#include "..\opencv\cxcore.h"
#include "..\opencv\ml.h"
#include "..\opencv\cvaux.h"

#include "minmax.h"
//#include "stdafx.h"

#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <atlimage.h>
#include <time.h>
#include <math.h>
#include "iostream"
using namespace std;

//隐藏DOS界面
#pragma comment(linker,   "/subsystem:\"windows\"   /entry:\"mainCRTStartup\"")

#include "../include/asmfitting.h"
#include "../include/asmbuilding.h"
#include "../include/asmlibrary.h"

#pragma comment(lib, "../lib/cv210.lib")
#pragma comment(lib, "../lib/cxcore210.lib")
#pragma comment(lib, "../lib/highgui210.lib")
#pragma comment(lib, "../lib/cvaux210.lib")
#pragma comment(lib, "../lib/asmlibrary.lib")


bool init_asm();
	
IplImage * align_pic(IplImage * src,int Align_iterations);//对齐人脸

void rota_pic(IplImage * src_img, IplImage * dst_img, CvPoint rota_point, float rota_angle);

bool detect_all_faces(IplImage* image,int FD_iterations);

int align_pic(IplImage * image,CvRect rect,int Align_iterations,int flag_num);

void free_shape_memeory(asm_shape** shapes);

int detect_and_draw( IplImage* img,int num,float Windows_scale,int FD_iterarions,int show_width,int show_height);


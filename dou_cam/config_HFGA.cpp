/**************************************
      功能：读配置文件
	  时间：2014年9月9日
	  作者：Ethan
**************************************/
#include "config_HFGA.h"

int config_HFGA::Read_configfiles()
{
	FILE *stream;
	if ((stream=fopen("hour.ini","r"))==NULL)
	{
		cout<<"打开hour.ini失败！"<<endl;
		return -1;
	}
	else
	{
		//hour=0;
		fscanf(stream,"%d",&hour);
		fclose(stream);
		//config->hour=hour;
	} 


	if ((stream=fopen("minute.ini","r"))==NULL)
	{
		cout<<"打开minute.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%d",&minute);
		fclose(stream);
	} 

	if ((stream=fopen("second.ini","r"))==NULL)
	{
		cout<<"打开second.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%d",&second);
		fclose(stream);
	} 


	if ((stream=fopen("Windows_scale.ini","r"))==NULL)
	{
		cout<<"打开Windows_scale.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%f",&window_scale);
		fclose(stream);
	} 

	if ((stream=fopen("Windows_scale1.ini","r"))==NULL)
	{
		cout<<"打开Windows_scale1.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%f",&window_scale1);
		fclose(stream);
	} 

	if ((stream=fopen("FD_iterations.ini","r"))==NULL)
	{
		cout<<"打开FD_iterations.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%d",&FD_iterations);
		fclose(stream);
	} 

	if ((stream=fopen("show_width.ini","r"))==NULL)
	{
		cout<<"打开show_width.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%d",&show_width);
		fclose(stream);
	} 
	if ((stream=fopen("show_height.ini","r"))==NULL)
	{
		cout<<"打开show_height.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%d",&show_height);
		fclose(stream);
	} 
	if ((stream=fopen("Align_iterations.ini","r"))==NULL)
	{
		cout<<"打开Align_iterations.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%d",&Align_iterations);
		fclose(stream);
	}

	if ((stream=fopen("pic_width.ini","r"))==NULL)
	{
		cout<<"打开pic_width.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%d",&pic_width);
		fclose(stream);
	} 
	if ((stream=fopen("pic_height.ini","r"))==NULL)
	{
		cout<<"打开pic_height.ini失败！"<<endl;
		return -1;
	}
	else
	{
		fscanf(stream,"%d",&pic_height);
		fclose(stream);
	}  

	return 0;
}

void config_HFGA::free_config()
{

}

bool config_HFGA::config_same()
{
	SYSTEMTIME sys;
	GetLocalTime( &sys );

	if (((sys.wHour==hour)&&(sys.wMinute==minute)&&(sys.wSecond==second)))return true;
	else return false;
	
}


CamPara* config_HFGA::get_config()
{
	CamPara* para=new CamPara;

	para->hour=hour;

	para->minute=minute;

	para->second=second;

	para->FD_iterations=FD_iterations;

	para->Align_iterations=Align_iterations;

	para->pic_height=pic_height;

	para->pic_width=pic_width;

	para->show_height=show_height;

	para->show_width=show_width;

	para->window_scale=window_scale;

	para->window_scale1=window_scale1;

	return para;
}
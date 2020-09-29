#include "ts.h"

int tsfd;//触摸屏文件描述

int ts_open(void)	//打开触摸屏
{ 
	tsfd=open("/dev/input/event0",O_RDWR);
	if(tsfd==-1)
	{
		printf("打开触摸屏驱动失败\r\n");
		return -1;
	}
	return 0;
}

int get_x_y(int *x,int *y)	//获取触摸屏点击坐标
{ 
	struct input_event EV1;
	int count=0,reslut;
	while(1)
	{
		reslut=read(tsfd,&EV1,sizeof(EV1));
		if(reslut == -1) 
		{
			printf("read event0 fail!\n"); 
			return -1;
		}
		if(EV1.type==EV_ABS && EV1.code==ABS_X && EV1.value>=0 && EV1.value<1024) 
		{
			*x=EV1.value*800/1024;	//黑色触摸屏	
		}
		if(EV1.type==EV_ABS && EV1.code==ABS_Y && EV1.value>=0 && EV1.value<600) 
		{
			*y=EV1.value*480/600; 
		}
		if(EV1.type==EV_KEY && EV1.code==BTN_TOUCH && EV1.value==0) 
		{
			break; 
		}
	}
	return 0;
}
 
int ts_close(void)	//关闭触摸屏
{
	close(tsfd);
	return 0;
}

//获取滑动方向
int get_directory(void)
{
	struct input_event buf;
	int x1=-1, y1=-1;
	int x2=-1, y2=-1;

	// 取得起始坐标(x1, y1)
	while(1)
	{
		bzero(&buf,sizeof(buf));	
		read(tsfd,&buf,sizeof(buf)); // x/y/p/syn

		if(buf.type == EV_ABS && buf.code == ABS_X)
			x1 = buf.value;

		if(buf.type == EV_ABS && buf.code == ABS_Y)
			y1 = buf.value;
		
		if(x1 != -1 && y1 != -1)
			break;
	}

	// 取得终止坐标(x2, y2)
	while(1)
	{
		bzero(&buf, sizeof(buf));	
		read(tsfd, &buf, sizeof(buf)); // x/y/p/syn

		if(buf.type == EV_ABS && buf.code == ABS_X)
			x2 = buf.value;

		if(buf.type == EV_ABS && buf.code == ABS_Y)
			y2 = buf.value;

		// 手指松开了，走人！
		if(buf.type == EV_KEY &&
		   buf.code == BTN_TOUCH &&
		   buf.value == 0)
			break;
	}
// 返回方向
	int delta_x = x1-x2;
	int delta_y = y1-y2;

	if(x2 == -1 || y2 == -1)
		return -1;

	if(abs(delta_x) > abs(delta_y))
	{
		if(delta_x > 0)	
			return LEFT;
		else
			return RIGHT;
	}
	else	
	{
		if(delta_y > 0)
			return UP;
		else
			return DOWN;
	}
}

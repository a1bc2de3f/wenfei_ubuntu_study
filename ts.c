#include "ts.h"

int tsfd;//�������ļ�����

int ts_open(void)	//�򿪴�����
{ 
	tsfd=open("/dev/input/event0",O_RDWR);
	if(tsfd==-1)
	{
		printf("�򿪴���������ʧ��\r\n");
		return -1;
	}
	return 0;
}

int get_x_y(int *x,int *y)	//��ȡ�������������
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
			*x=EV1.value*800/1024;	//��ɫ������	
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
 
int ts_close(void)	//�رմ�����
{
	close(tsfd);
	return 0;
}

//��ȡ��������
int get_directory(void)
{
	struct input_event buf;
	int x1=-1, y1=-1;
	int x2=-1, y2=-1;

	// ȡ����ʼ����(x1, y1)
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

	// ȡ����ֹ����(x2, y2)
	while(1)
	{
		bzero(&buf, sizeof(buf));	
		read(tsfd, &buf, sizeof(buf)); // x/y/p/syn

		if(buf.type == EV_ABS && buf.code == ABS_X)
			x2 = buf.value;

		if(buf.type == EV_ABS && buf.code == ABS_Y)
			y2 = buf.value;

		// ��ָ�ɿ��ˣ����ˣ�
		if(buf.type == EV_KEY &&
		   buf.code == BTN_TOUCH &&
		   buf.value == 0)
			break;
	}
// ���ط���
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

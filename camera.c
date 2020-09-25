#include "camera.h"

pthread_t id;
int flag=1;

void *monitor(void *arg)
{
	while(flag)
	{
		get_x_y(&ts_x,&ts_y);
	}	
}

//实现监控功能
int fc_camera(void)
{
	flag=1;
	camera_t *mycamera=malloc(sizeof(camera_t));
	showbmp("/wenfei/images/cameraui.bmp");
	pthread_create(&id,NULL,monitor,NULL);	
	//初始化摄像头
	yuvcamera_init(mycamera,"/dev/video7");
	char rgbdata[mycamera->width*mycamera->height*3];
	while(1)
	{
		if(ts_x>0 && ts_x<80 && ts_y>0 && ts_y<480)//实时监控
		{
		//获取摄像头采集到RGB数据
		yuvcamera_getrgb(mycamera,rgbdata);
		//将获取到RGB数据填充开发板的lcd
		yuvcamera_show(mycamera,p_mmap,rgbdata);
		}
		
		if(ts_x>720 && ts_x<800 && ts_y>0 && ts_y<480)//退出监控
		{
			flag = 0;
			break;
		}
	}
	//取消刚才新建的线程
	pthread_cancel(id);
	function=MAIN;
	printf("返回主界面\n");
	//关闭摄像头
	yuvcamera_close(mycamera);
	ts_x=-1;ts_y=-1;
	return 0;
}

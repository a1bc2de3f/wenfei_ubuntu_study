#include "login.h"

//欢迎gif
int fc_welcome(void)
{
	showbmp("/wenfei/gif/gif1.bmp");
	usleep(500*1000);
	showbmp("/wenfei/gif/gif2.bmp");
	usleep(500*1000);
	showbmp("/wenfei/gif/gif3.bmp");
	usleep(500*1000);
	showbmp("/wenfei/gif/gif4.bmp");
	usleep(500*1000);
	showbmp("/wenfei/gif/gif5.bmp");
	usleep(500*1000); 
	showbmp("/wenfei/gif/gif6.bmp");
	usleep(500*1000); 
	function=1;
}

//登录界面
int fc_login(void)
{
	char password_input[5]="0000";//4位密码
	char lock_path[]="/wenfei/images/lock0.bmp";//解锁进度图片路径
	int i=0,num=0;//输入密码数字个数、次数标识
	showbmp(lock_path);
	while(1)
	{
		get_x_y(&ts_x,&ts_y);//等待点击并获取点击坐标
		if(ts_x<=311&&ts_x>=212&&ts_y>=109&&ts_y<=208)	//1
		{
			password_input[i]='1';//存入密码数组
			i++;				  //次数加一
		}
		if(ts_x<=450&&ts_x>=350&&ts_y>=109&&ts_y<=208)	//2
		{
			password_input[i]='2';
			i++;
		}
		if(ts_x<=590&&ts_x>=490&&ts_y>=109&&ts_y<=208)	//3
		{
			password_input[i]='3';
			i++;
		}
		if(ts_x<=311&&ts_x>=212&&ts_y>=232&&ts_y<=330)	//4
		{
			password_input[i]='4';
			i++;
		}
		if(ts_x<=450&&ts_x>=350&&ts_y>=232&&ts_y<=330)	//5
		{
			password_input[i]='5';
			i++;
		}
		if(ts_x<=590&&ts_x>=490&&ts_y>=232&&ts_y<=330)	//6
		{
			password_input[i]='6';
			i++;
			printf("6\n");
		}
		if(ts_x<=311&&ts_x>=212&&ts_y>=350&&ts_y<=451)	//7
		{
			password_input[i]='7';
			i++;
		}
		if(ts_x<=450&&ts_x>=350&&ts_y>=350&&ts_y<=451)	//8
		{
			password_input[i]='8';
			i++;
		}
		if(ts_x<=590&&ts_x>=490&&ts_y>=350&&ts_y<=451)	//9
		{
			password_input[i]='9';
			i++;
		}
		if(ts_x<=744&&ts_x>=647&&ts_y>=384&&ts_y<=422)	//删除
		{
			i--;
			if(i<0)	i=0;
		}
		//刷新进度界面
		lock_path[19]=i+'0';
		showbmp(lock_path);
		
		if(i==4)//核对密码
		{
			i=0;//重新计数
			num++;//核对次数加一
			usleep(200*1000);
			if(strcmp("4291",password_input)==0)//密码正确
			{
				printf("Login successfully! \n");
				printf("进入系统\n");
				function=MAIN;				//进入主界面
				break;
			}
			else				//密码错误，重回进度初始界面
			{
				lock_path[19]=i+'0';
				showbmp(lock_path);
			}
		}
		
		if(num==3)//3次核对次数用完，结束进程
		{
			showbmp("/wenfei/images/image1.bmp");
			function=END;
			break;
		}
	}
}
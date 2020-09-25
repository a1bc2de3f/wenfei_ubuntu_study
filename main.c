#include "main.h"

int ts_x,ts_y;	//获取点击的x,y坐标
int function=0;	//功能界面切换

int main(void)
{
	lcd_open();	//打开lcd
	ts_open();	//打开触摸屏
	while(1)
	{
		switch(function)
		{
			case WELCOME:	fc_welcome();
							break;
			case LOGIN:	 	fc_login();
							break;		
			case MAIN:	 	fc_main();
							break;
			case PHOTO:	 	fc_photo();
							break;		
			case CAMERA:	fc_camera();
							break;
			case END:	 	lcd_close();ts_close();
							return;							
		}		 
	}
}

//主界面
int fc_main(void)
{
	showbmp("/wenfei/images/main.bmp");
	while(1)
	{
		get_x_y(&ts_x,&ts_y);//等待点击并获取点击坐标
		if(ts_x<=579&&ts_x>=404&&ts_y>=100&&ts_y<=272)	//进入毕业季的回忆
		{
			function=PHOTO;
			printf("进入相册\n");
			break;
		}
		if(ts_x<=761&&ts_x>=587&&ts_y>=101&&ts_y<=273)	//CAMERA
		{
			function=CAMERA;
			printf("进入摄像头模式\r\n");
			break;
		}
		if(ts_x<=579&&ts_x>=404&&ts_y>=278&&ts_y<=452)	//REBOOT
		{
			function=WELCOME;
			ts_x=-1;ts_y=-1;
			printf("重启\r\n");
			break;
		}
		if(ts_x<=394&&ts_x>=200&&ts_y>=100&&ts_y<=272)	//LOGOUT
		{
			function=LOGIN;
			ts_x=-1;ts_y=-1;
			printf("退出登录\r\n");
			break;
		}
		if(ts_x<=761&&ts_x>=587&&ts_y>=278&&ts_y<=452)	//END
		{
			function=END;
			printf("END!!!\r\n");
			break;
		}		
	}
}

//相册
int fc_photo(void)
{
	int show_start=0;	//开启相册标识
	int dir=-1;	//滑动反向标识
	int photo_sign=0;	//图片切换标识
	int photo_max=0;	//图片上限
	char photo_path[]="/wenfei/images/fosu0_0.bmp";//初始相册路径
	showbmp("/wenfei/images/fosu.bmp");//显示学校地图
	while(1)
	{
		get_x_y(&ts_x,&ts_y);
		if(ts_x<=462&&ts_x>=316&&ts_y>=64&&ts_y<=152)	//C4教学楼
		{
			photo_path[19]='0';//C4教学楼bmp图片格式：...fosu0_x.bmp(x=0,1,2...)
			photo_max=PHOTO_C4_MAX;
			photo_sign=0;
			show_start=1;		//开启相册
		}		
		if(ts_x<=324&&ts_x>=191&&ts_y>=373&&ts_y<=464)	//图书馆
		{
			photo_path[19]='1';//图书馆bmp图片格式：...fosu1_x.bmp(x=0,1,2...)
			photo_max=PHOTO_LIBRARY_MAX;
			photo_sign=0;
			show_start=1;
		}	
		if(ts_x<=739&&ts_x>=672&&ts_y>=110&&ts_y<=175)	//返回主界面
		{
			function=MAIN;
			printf("返回主界面\n");
			break;
		}	
		if(show_start)	//进入相册
		{
			photo_path[21]='0';//加载初始图片
			showbmp(photo_path);
			while(1)
			{
				dir=get_directory();//获取滑动方向
				if(dir==UP||dir==DOWN)	//上下滑动：返回地图界面
				{
					showbmp("/wenfei/images/fosu.bmp");
					show_start=0;//禁止进入相册
					break;
				}
				if(dir==LEFT)	//左滑：切换图片（向左飞入特效）
				{
					photo_sign++;//下一张图片
					if(photo_sign>photo_max)	photo_sign=0;//到尽头了，来个轮回
					photo_path[21]=photo_sign+'0';
					pic_left(photo_path);
				}
				if(dir==RIGHT)	//右滑：切换图片（向右飞入特效）
				{
					photo_sign--;
					if(photo_sign<0)	photo_sign=photo_max;
					photo_path[21]=photo_sign+'0';
					pic_right(photo_path);
				}

			}
		}
	}
}



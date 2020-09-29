#include "main.h"

int ts_x,ts_y;	//获取点击的x,y坐标
int function=WELCOME;	//功能切换

int main(void)
{
	lcd_open();	//打开lcd
	ts_open();	//打开触摸屏
	init_gesture();	//手势识别初始化
	while(1)
	{
		switch(function)
		{
			case WELCOME:	fc_welcome();	//gif
							break;
			case LOGIN:	 	fc_login();		//登录界面
							break;		
			case MAIN:	 	fc_main();		//主界面
							break;
			case PHOTO:	 	fc_photo();		//毕业季的回忆
							break;		
			case CAMERA:	fc_camera();	//监控
							break;
			case MUSIC:		fc_music();		//音乐
							break;
			case END:	 	lcd_close();	//结束进程
							ts_close();
							close_gesture();
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
		if(ts_x<=384&&ts_x>=218&&ts_y>=121&&ts_y<=362)	//进入毕业季的回忆
		{
			function=PHOTO;
			printf("进入毕业季的回忆相册\n");
			break;
		}
		if(ts_x<=580&&ts_x>=411&&ts_y>=121&&ts_y<=362)	//CAMERA
		{
			function=CAMERA;
			printf("进入摄像头模式\r\n");
			break;
		}
		if(ts_x<=772&&ts_x>=606&&ts_y>=121&&ts_y<=362)	//MUSIC
		{
			function=MUSIC;
			printf("进入音乐模式\r\n");
			break;
		}
		if(ts_x<=193&&ts_x>=24&&ts_y>=121&&ts_y<=362)	//REBOOT
		{
			function=WELCOME;
			ts_x=-1;ts_y=-1;
			printf("重启\r\n");
			break;
		}	
	}
}

//相册
int fc_photo(void)
{
	int show_start=0;	//开启相册标识
	int dir=-1;			//滑动/手势方向标识
	int photo_sign=0;	//图片切换标识
	int photo_max=0;	//图片上限
	char photo_path[]="/wenfei/images/fosu0_0.bmp";//初始相册路径
	showbmp("/wenfei/images/fosu.bmp");//显示学校地图
	system("madplay /wenfei/music/music0.mp3 &");
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
			system("killall -KILL madplay");
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
				//dir=get_directory();//获取滑动方向
				dir=read_gesture();//获取手势方向
				if(dir==UP||dir==DOWN)	//上下滑动：返回地图界面
				{
					showbmp("/wenfei/images/fosu.bmp");
					show_start=0;//禁止进入相册
					break;
				}
				if(dir==LEFT)	//左滑：切换图片
				{
					photo_sign++;//下一张图片
					if(photo_sign>photo_max)	photo_sign=0;//到尽头了，来个轮回
					photo_path[21]=photo_sign+'0';
					showbmp(photo_path);
				}
				if(dir==RIGHT)	//右滑：切换图片（向右飞入特效）
				{
					photo_sign--;
					if(photo_sign<0)	photo_sign=photo_max;
					photo_path[21]=photo_sign+'0';
					showbmp(photo_path);
				}

			}
		}
	}
}

int fc_music(void)
{	
	int i=0,status=1;
	showbmp("/wenfei/images/music_start.bmp");
	char music_path[]="madplay /wenfei/music/music0.mp3 &";
	system(music_path);
	while(1)
	{
		get_x_y(&ts_x,&ts_y);
		if(ts_x>345&&ts_x<456&&ts_y>186&&ts_y<290)			//暂停/播放
		//if(ts_x>345&&ts_x<456&&ts_y>79&&ts_y<187)			//暂停/播放
		{
			if(status)
			{
				system("killall -STOP madplay");
				showbmp("/wenfei/images/music_stop.bmp");						
				printf("暂停");
				status=0;
			}
			else
			{
				system("killall -CONT madplay");	
				showbmp("/wenfei/images/music_start.bmp");
				printf("继续");	
				status=1;
			}
		}
		if(ts_x>241&&ts_x<306&&ts_y>209&&ts_y<269)			//上一首
		//if(ts_x>232&&ts_x<290&&ts_y>113&&ts_y<159)			//上一首
		{	
			system("killall -KILL madplay");
			i--;
			if(i<0)	i=2;
			music_path[27]=i+'0';
			system(music_path);		
			printf("上一首\n");		
		}	
		if(ts_x>495&&ts_x<559&&ts_y>209&&ts_y<269)			//下一首
		//if(ts_x>509&&ts_x<563&&ts_y>113&&ts_y<159)			//下一首
		{
			system("killall -KILL madplay");
			i++;
			if(i>2)	i=0;
			music_path[27]=i+'0';
			system(music_path);		
			printf("下一首\n");		
		}	
		if(ts_x>716&&ts_x<781&&ts_y>22&&ts_y<88)			//退出	
		//if(ts_x>361&&ts_x<437&&ts_y>383&&ts_y<470)			//退出
		{
			function=MAIN;
			system("killall -KILL madplay");						
			printf("退出\n");
			break;			
		}		
	}
} 



#include "gesture.h"

int gesture_fd; //打开IIC驱动的描述符

//初始化手势识别模块
int init_gesture(void)
{
	system("insmod /wenfei/driver_ko/IIC_drv.ko");  //安装驱动
	gesture_fd = open("/dev/IIC_drv", O_RDWR);      //打开驱动
	if(gesture_fd < 0)
	{
		perror("open error!");
		return -1;	
	}
}

//读取手势方向
int read_gesture(void)
{
    char tmp;
	read(gesture_fd,&tmp,1);
		
    if(tmp>=1 && tmp<=4)
    {
        printf("%x\n",tmp);
        switch(tmp)
        {	
            case 1:printf("test-Up\n");return UP;//向上
            case 2:printf("test-Down\n");return DOWN;//向下
            case 3:printf("test-Left\n");return LEFT;//向左
            case 4:printf("test-Right\n");return RIGHT;//向右
/*             case 5:printf("test-Forward\n");break;//前
            case 6:printf("test-Backward\n");break;//后
            case 7:printf("test-Clockwise\n");break;//顺时针
            case 8:printf("test-AntiClockwise\n");break;//逆时针
            case 9:printf("test-Wave\n");break;//挥动 */
            default:return -1;
        }
    }
}

int close_gesture(void)
{
    close(gesture_fd);
    return 0;
}
#include "showbmp.h"

int lcdfd;
int *p_mmap = NULL;

int lcd_open(void)
{
	//打开lcd驱动
	lcdfd=open("/dev/fb0",O_RDWR);
	if(lcdfd==-1)
	{
		printf("打开lcd驱动失败\r\n");
		return -1;
	}
	//映射得到lcd的首地址
	p_mmap = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, lcdfd, 0);
	if(p_mmap == MAP_FAILED)
	{
		printf("mmap fail!\n");
		return -1;
	}
}

int showbmp(const char *pathname)
{
	//读取图片数据
	int fd_bmp;
	char bmp_buf[800*480*3]={0};//图片RGB信息存放
	fd_bmp=open(pathname,O_RDWR);
	if(fd_bmp==-1)
	{
		printf("打开图片失败\r\n");
		return -1;
	}
	lseek(fd_bmp,54,SEEK_SET);//略过前54个字节
	read(fd_bmp,bmp_buf,800*480*3);//读取bmp图片的RGB信息
	close(fd_bmp);
	//图片数据处理后显示
	int i,x,y;
	int lcdbuf[800*480]={0},writebuf[800*480]={0};
	for(i=0;i<800*480;i++)//RGB信息合并为4字节
		lcdbuf[i]=bmp_buf[3*i+2]<<16|bmp_buf[3*i+1]<<8|bmp_buf[3*i]<<0;
	
	int *p_mmap_usr = p_mmap;
	for(y=0; y<480; y++)		//颠倒处理
	{
		for(x=0; x<800; x++)
		{
			*(p_mmap_usr+800*(479-y)+x)=lcdbuf[800*y+x];
		}
	}
	p_mmap_usr = NULL;
	return 0;
}

int lcd_close(void)
{
	munmap(p_mmap, 800*480*4);
	p_mmap = NULL;	
	close(lcdfd);
	return 0;
}

/*
	功能：切换BMP图片特效”向左飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_left_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_left(char *pathname)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_left(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_left(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_left(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画“向左飞入”效果算法
	for(j=799; j>=0; j--)
	{
		for(i=0; i<480; i++)
		{
			p_mmap[800*i+j] = mi_buf[800*i+j];
		}
		usleep(500);
	} 
	
	return 0;
}

/*
	功能：切换BMP图片特效”向右飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_right_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_right(char *pathname)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_right(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_right(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_right(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画“向右飞入”效果算法
	for(j=0; j<800; j++)
	{
		for(i=0; i<480; i++)
		{
			p_mmap[800*i+j] = mi_buf[800*i+j];
		}
		usleep(500);
	} 
		
	return 0;
}




 
 


 
 

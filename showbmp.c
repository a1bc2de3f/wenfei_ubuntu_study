#include "showbmp.h"

int lcdfd;
int *p_mmap = NULL;

int lcd_open(void)
{
	//��lcd����
	lcdfd=open("/dev/fb0",O_RDWR);
	if(lcdfd==-1)
	{
		printf("��lcd����ʧ��\r\n");
		return -1;
	}
	//ӳ��õ�lcd���׵�ַ
	p_mmap = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, lcdfd, 0);
	if(p_mmap == MAP_FAILED)
	{
		printf("mmap fail!\n");
		return -1;
	}
}

int showbmp(const char *pathname)
{
	//��ȡͼƬ����
	int fd_bmp;
	char bmp_buf[800*480*3]={0};//ͼƬRGB��Ϣ���
	fd_bmp=open(pathname,O_RDWR);
	if(fd_bmp==-1)
	{
		printf("��ͼƬʧ��\r\n");
		return -1;
	}
	lseek(fd_bmp,54,SEEK_SET);//�Թ�ǰ54���ֽ�
	read(fd_bmp,bmp_buf,800*480*3);//��ȡbmpͼƬ��RGB��Ϣ
	close(fd_bmp);
	//ͼƬ���ݴ������ʾ
	int i,x,y;
	int lcdbuf[800*480]={0},writebuf[800*480]={0};
	for(i=0;i<800*480;i++)//RGB��Ϣ�ϲ�Ϊ4�ֽ�
		lcdbuf[i]=bmp_buf[3*i+2]<<16|bmp_buf[3*i+1]<<8|bmp_buf[3*i]<<0;
	
	int *p_mmap_usr = p_mmap;
	for(y=0; y<480; y++)		//�ߵ�����
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
	���ܣ��л�BMPͼƬ��Ч��������롰��
	������
		char *pathname��ͼƬ��·����
		int *pic_left_fd����ӳ������ָ�롣
	����ֵ���ɹ�  0
			ʧ��  -1	
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

	bmp_fd = open(pathname , O_RDONLY);//1����BMP��ʽͼƬ
	if(bmp_fd == -1)
	{
		printf("pic_left(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2������bmpͼƬ��ǰ54��λ��
	if(ret == -1)
	{
		perror("pic_left(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4��ȡ��ͼƬ����
	if(ret == -1)
	{
		printf("pic_left(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5���ر�ͼƬ

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6��24bits ת 32bits���Ʊ���
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7�����ͼƬ���µߵ�����
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8�����⶯����������롱Ч���㷨
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
	���ܣ��л�BMPͼƬ��Ч�����ҷ��롰��
	������
		char *pathname��ͼƬ��·����
		int *pic_right_fd����ӳ������ָ�롣
	����ֵ���ɹ�  0
			ʧ��  -1	
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

	bmp_fd = open(pathname , O_RDONLY);//1����BMP��ʽͼƬ
	if(bmp_fd == -1)
	{
		printf("pic_right(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2������bmpͼƬ��ǰ54��λ��
	if(ret == -1)
	{
		perror("pic_right(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4��ȡ��ͼƬ����
	if(ret == -1)
	{
		printf("pic_right(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5���ر�ͼƬ

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6��24bits ת 32bits���Ʊ���
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7�����ͼƬ���µߵ�����
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8�����⶯�������ҷ��롱Ч���㷨
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




 
 


 
 

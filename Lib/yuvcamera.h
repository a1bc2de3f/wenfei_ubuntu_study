#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <setjmp.h>
#include <stdbool.h>
#include "jpeglib.h"
#define FALSE 0

#define TRUE 1
typedef struct {
  uint8_t* start;
  size_t length;
} buffer_t;

typedef struct {
  int fd;
  uint32_t width;
  uint32_t height;
  size_t buffer_count;
  buffer_t* buffers;
  buffer_t head;
} camera_t;
//指针camera如果直接使用指针作为实参，需要分配堆空间
//rgbbuf的大小是宽*高*3字节
extern int yuvcamera_init(camera_t *camera,char *cameradriverpath);
extern int yuvcamera_getrgb(camera_t *camera,unsigned char *rgbbuf);
extern int yuvcamera_show(camera_t *camera,unsigned int *lcdmem,unsigned char *rgbbuf);
extern int yuvcamera_save(char *jpgname,unsigned char *rgbbuf,camera_t *camera);
extern int yuvcamera_close(camera_t *camera);
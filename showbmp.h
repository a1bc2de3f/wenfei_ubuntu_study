#ifndef _SHOWBMP_H
#define _SHOWBMP_H

#include <stdio.h>//fseek、fread、fclose、printf、
#include <sys/types.h>//open,lseek
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <strings.h>//bzero
#include <unistd.h>//lseek,read
#include <sys/mman.h>//mmap,munmap

#include "main.h"

extern int *p_mmap;

int lcd_open(void);
int showbmp(const char *pathname);
int lcd_close(void);
int pic_left(char *pathname);
int pic_right(char *pathname);


#endif


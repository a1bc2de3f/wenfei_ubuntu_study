#ifndef __TS_H
#define __TS_H

#include <stdio.h>//printf
#include <linux/input.h>//struct input_event
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//read
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

enum {UP, DOWN, LEFT, RIGHT};//滑动方向枚举

int ts_open(void);//打开触摸屏
int get_x_y(int *x,int *y);
int ts_close(void);
int get_directory(void);

#endif

#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include "showbmp.h"
#include "ts.h"
#include "login.h"
#include "camera.h"

//功能切换宏
#define WELCOME 0
#define LOGIN   1
#define MAIN    2
#define PHOTO   3
#define CAMERA	4
#define END		5
#define MUSIC	6

//相册上限宏
#define PHOTO_LIBRARY_MAX 2
#define PHOTO_C4_MAX 	  3

extern int ts_x,ts_y;//获取点击的x,y坐标
extern int function;//功能界面切换

int fc_main(void);
int fc_photo(void);

#endif

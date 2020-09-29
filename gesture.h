#ifndef __GESTURE_H
#define __GESTURE_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "main.h"

int init_gesture(void);
int read_gesture(void);
int close_gesture(void);

#endif

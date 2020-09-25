#ifndef __CAMERA_H
#define __CAMERA_H

#include <stdio.h>
#include <pthread.h>
#include "yuvcamera.h"
#include <strings.h>

#include "main.h"


void *monitor(void *arg);
int fc_camera(void);

#endif


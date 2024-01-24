/*Header file to include ioctl declaration to change parent of current process*/
#include <linux/ioctl.h>
#ifndef DR_DOOM_DRIVER_H
#define DR_DOOM_DRIVER_H

#define CHANGE_PARENT_TO_PID _IOR('a','a',int32_t*)

#endif

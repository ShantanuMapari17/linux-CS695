/*Header file for ioctl ops*/
#include<linux/ioctl.h>
#ifndef SPOCK_DRIVER_H
#define SPOCK_DRIVER_H

#define VIRTUAL_TO_PHYSICAL _IOR('a','a',int32_t*)
#define WRITE_TO_PHYSICAL _IOW('a','b',int32_t*)

#endif

/* 1 Program to allocate heap memory to a varaible and passing v_addr to get physical address using ioctl
** 2 Write user given value to the physical address
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#include "spock-driver.h"

int main(){
    int *ptr = (int *)malloc(sizeof(int));
    int val;
    *ptr = 55;
    printf("ptr = %p\n", ptr);
    printf("Value = %d\n", *ptr);

    int fd = open("/dev/my_spock", O_RDWR);

    if(fd < 0){
        printf("Cannot open device file...\n");
        return 0;
    }

    printf("Device file opened...\n");

    if(ioctl(fd,VIRTUAL_TO_PHYSICAL, &ptr) < 0){
        printf("IOCTL failed...\n");
        return 0;
    }

    printf("IOCTL-getting physical adress success...\n");

    printf("Enter value to write to physical address: ");
    scanf("%d", &val);

    if(ioctl(fd,WRITE_TO_PHYSICAL, &val) < 0){
        printf("IOCTL failed...\n");
        return 0;
    }

    printf("IOCTL-writing to physical address success...\n");
    printf("Value written to physical address is %d\n", *ptr);

    return 0;
    

}

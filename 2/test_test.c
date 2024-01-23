#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#include "spock-driver.h"

int main(){
    int x=56;
    printf("Adress of x : 0x%llx PID of process : %d\n",&x,getpid());
    int fd = open("/dev/my_spock", O_RDWR);

    if(ioctl(fd,VIRTUAL_TO_PHYSICAL, &x) < 0){
        printf("IOCTL failed...\n");
        return 0;
    }
    printf("IOCTL-getting physical adress success...\n");
    
    sleep(180);
    return 0;
}
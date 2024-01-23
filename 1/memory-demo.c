#include <unistd.h>
#include <stdio.h>
#include<stdlib.h>

int main(){
    printf("Demo program to print virtual memory address of the variable \n");
    int *x=malloc(40*sizeof(int));
    x[0]=10;
    printf("Pid of the process is := %d \n", getpid());
    printf("Value of x : %d \t Address of x : 0x%lx \n\n",x[0],x);
    sleep(180);
    return 0;
}
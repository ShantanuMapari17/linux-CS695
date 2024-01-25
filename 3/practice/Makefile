obj-m += hello_proc.o
# obj-m += lkm4.o


KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`
clean:
	make -C $(KDIR) M=`pwd` clean
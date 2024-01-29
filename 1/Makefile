obj-m += lkm1.o
# obj-m += lkm2.o
# obj-m += lkm3.o
# obj-m += lkm4.o


KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`
clean:
	make -C $(KDIR) M=`pwd` clean


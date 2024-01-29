obj-m += get_pagefaults.o
obj-m += hello_procfs.o


KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`
clean:
	make -C $(KDIR) M=`pwd` clean
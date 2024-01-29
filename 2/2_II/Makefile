TARGETS = control_station soldier
MODULE_NAME = dr_doom_driver

SOURCES = $(TARGETS:=.c) $(MODULE_NAME).c
obj-m += $(MODULE_NAME).o

all: $(TARGETS) $(MODULE_NAME)

$(TARGETS): $(SOURCES)
	$(CC) -o $@ $@.c

$(MODULE_NAME): $(SOURCES)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	rm -f $(TARGETS)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
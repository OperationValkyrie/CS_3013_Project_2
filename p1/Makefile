# Jonathan Chang																																																																																																																																																																																																																																																																																																																																																																																												# Jonathan Chang
# CS 3013 C02
# Project 2

obj-m := av.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

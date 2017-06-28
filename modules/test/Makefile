ifneq ($(KERNELRELEASE),)
#kbuild syntax. dependency relationshsip of files and target modules are listed here.  
mymodule-objs := hello.o
obj-m := hello.o
  
else
PWD  := $(shell pwd) 
KVER ?= $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
  
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *.symvers 
endif

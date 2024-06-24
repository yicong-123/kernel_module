obj-m := km.o
km-objs := km_main.o km_proc.o km_debug.o

# 获取当前使用的内核版本
KERNEL_PATH := $(shell uname -r)

# 当前内核目录
KDIR := /lib/modules/$(KERNEL_PATH)/build
# KDIR := /lib/modules/5.3.0-28-generic/build


all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.mod *.order
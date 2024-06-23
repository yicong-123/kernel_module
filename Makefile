obj-m := km_main.o

# 获取当前使用的内核版本
KERNEL_PATH := $(shell uname -r)

# 当前内核目录
KDIR := /lib/modules/$(KERNEL_PATH)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.mod *.order
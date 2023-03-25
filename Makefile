obj-m += kvs_driver.o
KERNEL_DIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

default: module kvs_app

module:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	${RM} kvs_app

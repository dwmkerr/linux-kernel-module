# obj-m specifie we're a kernel module.
obj-m += babel.o

# Set the path to the Kernel build utils.
KBUILD=/lib/modules/$(shell uname -r)/build/
 
default:
	$(MAKE) -C $(KBUILD) M=$(PWD) modules
	cc babel_client.c -o babel_client

clean:
	$(MAKE) -C $(KBUILD) M=$(PWD) clean

menuconfig:
	$(MAKE) -C $(KBUILD) M=$(PWD) menuconfig

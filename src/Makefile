obj-m += panel-raspberrypi-dsi.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf vc4-kms-dsi-raspberrypi.dtbo
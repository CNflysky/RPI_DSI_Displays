obj-m += panel-rpi-dsi-display.o
DTBO_NAME := vc4-kms-dsi-rpidisp
MODEL_FILE := /proc/device-tree/model
MODEL_EXISTS := $(wildcard $(MODEL_FILE))
PI_MODEL := pi4
MODULE_NAME := panel-rpi-dsi-display.ko
CONFIG_TXT := /boot/firmware/config.txt
OVERLAY_DIR := /boot/firmware/overlays
MODULE_INSTALL_DIR := /lib/modules/$(shell uname -r)/kernel/drivers/gpu/drm/panel

ifneq ($(MODEL_EXISTS),)
    MODEL_FILE_STR := $(shell cat $(MODEL_FILE))
    ifeq ($(findstring Pi 4,$(MODEL_FILE_STR)),Pi 4)
        PI_MODEL := pi4
    else ifeq ($(findstring Pi 5,$(MODEL_FILE_STR)),Pi 5)
        PI_MODEL := pi5
	else
		@echo "Failed to detect Pi Model!"
		@exit 1
		endif
endif

modules:
	@echo "Detected Pi model: $(PI_MODEL)"
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf *.dtbo

w280bf036i: modules
	dtc -I dts -O dtb -o $(DTBO_NAME).dtbo dts/vc4-kms-dsi-w280bf036i-$(PI_MODEL).dts

tdo-qhd0500d5: modules
	dtc -I dts -O dtb -o $(DTBO_NAME).dtbo dts/vc4-kms-dsi-tdo-qhd0500d5-$(PI_MODEL).dts

install: remove
	install -m 644 -D $(MODULE_NAME) $(MODULE_INSTALL_DIR)
	install -m 644 -D $(DTBO_NAME).dtbo $(OVERLAY_DIR)
	depmod -a
	echo "ignore_lcd=1" >> $(CONFIG_TXT)
	echo "dtoverlay=$(DTBO_NAME)" >> $(CONFIG_TXT)
	echo "Please reboot to apply changes"

remove:
	rm -rf $(MODULE_INSTALL_DIR)/$(MODULE_NAME)
	rm -rf $(OVERLAY_DIR)/$(DTBO_NAME).dtbo
	sed -i "/ignore_lcd=1/d" $(CONFIG_TXT)
	sed -i "/dtoverlay=$(DTBO_NAME)/d" $(CONFIG_TXT)
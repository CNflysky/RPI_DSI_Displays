obj-m += panel-rpi-dsi-display.o
DTBO_NAME := vc4-kms-dsi-rpidisp
MODEL_FILE := /proc/device-tree/model
MODEL_EXISTS := $(wildcard $(MODEL_FILE))
PI_MODEL := pi4
MODULE_NAME := panel-rpi-dsi-display.ko
CONFIG_TXT := /boot/firmware/config.txt
OVERLAY_DIR := /boot/firmware/overlays
KERNELRELEASE ?= $(shell uname -r)
MODULE_INSTALL_DIR := /lib/modules/$(KERNELRELEASE)/kernel/drivers/gpu/drm/panel
DKMS_NAME := panel-rpi-dsi-display
DKMS_VERSION := 1.0
DKMS_SRC_DIR := /usr/src/$(DKMS_NAME)-$(DKMS_VERSION)

ifneq ($(MODEL_EXISTS),)
    MODEL_FILE_STR := $(shell cat $(MODEL_FILE))
    ifeq ($(findstring Pi 4,$(MODEL_FILE_STR)),Pi 4)
        PI_MODEL := pi4
    else ifeq ($(findstring Pi 5,$(MODEL_FILE_STR)),Pi 5)
        PI_MODEL := pi5
    else
        $(error Failed to detect Pi model: $(MODEL_FILE_STR))
    endif
endif

modules:
	@echo "Detected Pi model: $(PI_MODEL)"
	$(MAKE) -C /lib/modules/$(KERNELRELEASE)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(KERNELRELEASE)/build M=$(PWD) clean
	rm -rf *.dtbo

w280bf036i: modules
	dtc -@ -H epapr -I dts -O dtb -o $(DTBO_NAME).dtbo dts/vc4-kms-dsi-w280bf036i-$(PI_MODEL).dts

tdo-qhd0500d5: modules
	dtc -@ -H epapr -I dts -O dtb -o $(DTBO_NAME).dtbo dts/vc4-kms-dsi-tdo-qhd0500d5-$(PI_MODEL).dts

install-overlay:
	install -m 644 -D $(DTBO_NAME).dtbo $(OVERLAY_DIR)
	echo "ignore_lcd=1" >> $(CONFIG_TXT)
	echo "dtoverlay=$(DTBO_NAME)" >> $(CONFIG_TXT)

remove-overlay:
	rm -f $(OVERLAY_DIR)/$(DTBO_NAME).dtbo
	sed -i "/ignore_lcd=1/d" $(CONFIG_TXT)
	sed -i "/dtoverlay=$(DTBO_NAME)/d" $(CONFIG_TXT)

install: remove-overlay
	install -m 644 -D $(MODULE_NAME) $(MODULE_INSTALL_DIR)
	$(MAKE) install-overlay
	depmod -a
	@echo "Please reboot to apply changes"

remove: remove-overlay
	rm -f $(MODULE_INSTALL_DIR)/$(MODULE_NAME)
	depmod -a

dkms_install: remove-overlay
	-rm -rf $(DKMS_SRC_DIR)
	install -d $(DKMS_SRC_DIR)
	install -m 644 Makefile dkms.conf panel-rpi-dsi-display.c $(DKMS_SRC_DIR)
	-dkms remove $(DKMS_NAME)/$(DKMS_VERSION) --all
	dkms add $(DKMS_NAME)/$(DKMS_VERSION)
	dkms build $(DKMS_NAME)/$(DKMS_VERSION)
	dkms install $(DKMS_NAME)/$(DKMS_VERSION)
	$(MAKE) install-overlay
	@echo "Please reboot to apply changes"

dkms_remove:
	-dkms remove $(DKMS_NAME)/$(DKMS_VERSION) --all
	-rm -rf $(DKMS_SRC_DIR)
	$(MAKE) remove-overlay
	@echo "Please reboot to remove the driver"

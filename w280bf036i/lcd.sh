#!/bin/bash
set -e
FILE_NAME='panel-wlk-w280bf036i'
DT_NAME='vc4-kms-dsi-w280bf036i'
MODULE_ROOT_PATH=/lib/modules/`uname -r`
MODULE_INSTALL_PATH='kernel/drivers/gpu/drm/panel'
function install_tools(){
    apt install -y make raspberrypi-kernel-headers
}

function set_config(){
    if grep $1=$2 /boot/config.txt
    then
        return
    else
        echo "$1=$2" >> /boot/config.txt
    fi
}

function remove_config(){
    sed -i "/$1/d" /boot/config.txt
}

function compile_module(){
    make
}

function install_module(){
    cp $1 $MODULE_ROOT_PATH/$MODULE_INSTALL_PATH
    depmod
}

function remove_module(){
    if [ -f $MODULE_ROOT_PATH/$MODULE_INSTALL_PATH/$1 ]
    then
        rm $MODULE_ROOT_PATH/$MODULE_INSTALL_PATH/$1
    fi
}

case $1 in 
install)
    echo "Installing tool..."
    install_tools
    echo "Compiling module..."
    make
    echo "Installing module..."
    install_module $FILE_NAME.ko
    echo "Compiling device tree overlay..."
    dtc -@ -I dts -O dtb -o $DT_NAME.dtbo $DT_NAME.dts
    echo "Installing device tree overlay..."
    cp $DT_NAME.dtbo /boot/overlays
    echo "Updating /boot/config.txt..."
    set_config ignore_lcd 1
    set_config dtoverlay $DT_NAME
    echo "Cleaning..."
    make clean
    echo "Done! You may need to reboot your system. "
;;
remove)
    echo "Removing module..."
    remove_module $FILE_NAME.ko
    echo "Removing device tree overlay..."
    rm -rf /boot/overlays/$DT_NAME.dtbo
    echo "Restoring /boot/config.txt..."
    remove_config ignore_lcd
    remove_config $DT_NAME
    echo "Done! You may need to reboot your system."
;;
*);&
help)
    echo "Usage: $0 <install/remove>"
;;
esac

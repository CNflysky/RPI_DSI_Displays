#!/bin/bash
MODULE_ROOT_PATH=/lib/modules/`uname -r`
MODULE_INSTALL_PATH='kernel/drivers/gpu/drm/panel'
DRIVER_SOURCE_DIR=src
FILE_NAME='panel-raspberrypi-dsi'
DT_NAME='vc4-kms-dsi-raspberrypi'

function install_tools(){
    if apt list --installed 2>/dev/null | grep -q raspberrypi-kernel-headers && apt list --installed 2>/dev/null | grep -q make
    then
        return
    else 
        apt install -y make raspberrypi-kernel-headers
    fi
}

function set_config(){
    if grep -q $1=$2 /boot/config.txt
    then
        return
    else
        echo "$1=$2" >> /boot/config.txt
    fi
}

function remove_config(){
    if grep -q $1 /boot/config.txt
    then
        sed -i "/$1/d" /boot/config.txt
    fi
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

function setup(){
    set -e
    install_tools
    cd src
    make
    install_module $FILE_NAME.ko
    cp $DT_NAME.dts $DT_NAME-1.dts
    sed -i "s/panel-compatible/$1/g" $DT_NAME-1.dts
    dtc -@ -I dts -O dtb -o $DT_NAME.dtbo $DT_NAME-1.dts
    cp $DT_NAME.dtbo /boot/overlays
    set_config ignore_lcd 1
    set_config dtoverlay $DT_NAME
    make clean
    rm $DT_NAME-1.dts
    ask_reboot
}

function remove(){
    set -e
    remove_module $FILE_NAME.ko
    rm -rf /boot/overlays/$DT_NAME.dtbo
    remove_config ignore_lcd
    remove_config $DT_NAME
    ask_reboot
}

function ask_reboot(){
    whiptail --title "LCD Driver Utility" --yes-button "Yes" --no-button "No"  --yesno "Would you like to reboot now?" 10 60 3>&1 1>&2 2>&3
    ret=$?
    if [ $ret -eq 0 ]
    then
        reboot
    elif [ $ret -eq 1 ]
    then
        exit 0
    else
        whiptail --title "LCD Driver Utility" --msgbox "User Canceled." 10 60
        exit 1
    fi
}

function checkroot(){
    if [ `whoami` != "root" ]
    then
        whiptail --title "LCD Driver Utility" --msgbox "This tool requires root privileges.\nTry to run again with sudo..." 10 60
        exit 1
    fi
}

function main(){
    checkroot
    whiptail --title "LCD Driver Utility" --yes-button "Install" --no-button "Remove"  --yesno "Which action would you like to take?" 10 60 3>&1 1>&2 2>&3
    ret=$?
    if [ $ret -eq 0 ]
    then
        action=install
    elif [ $ret -eq 1 ]
    then
        action=remove
    else
        whiptail --title "LCD Driver Utility" --msgbox "User Canceled." 10 60
        exit 1
    fi

    scr=`whiptail --title "LCD Driver Utility" --menu "Which driver would you like to $action?" 15 60 4 \
    "1" "W280BF036i" \
    "2" "W500IE020" \
    3>&1 1>&2 2>&3`

    case $scr in 
    1)
        if [ $action == "install" ]
        then
            setup wlk,w280bf036i
        else
            remove
        fi
    ;;
    2)
        if [ $action == "install" ]
        then
            setup wlk,w500ie020
        else
            remove
        fi
    ;;
    *)
        whiptail --title "LCD Driver Utility" --msgbox "User Canceled." 10 60
        exit 1
    ;;
    esac
}

main

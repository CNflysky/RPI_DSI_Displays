# RPI_DSI_Displays
Hardware & Kernel DRM Driver for RPI DSI Displays.  
*Note: This repository is still under development. Feel free to open issues if you have any questions or suggestions.*  
[Simplified Chinese(简体中文)](./README_zh.md)

# Why DSI?

Compared from SPI/DPI panels，DSI panels has taken these advantages:
- High refresh rate(~60fps)
- Easy to wire
- Consume less gpio resources
- Low power consumption

# Limitation
## Circuit 
Due to the circuit design of Raspberry Pi boards, RPi 4b on-board DSI connector only routes out 2 DSI lanes, with a maximum resolution of 720p.  
If you want to drive a panel with more than 2 lanes, you may need to consider the *compute module* or *RPi 5*.  
## RPiOS
You must enable `DRM` first in order to use this driver.  
In `Raspberry Pi OS` releases after `2022-1-28`, `DRM` is enabled by default.  
Old releases of RPiOS may not support `DRM`,so use latest version of RPiOS is recommended.  

# How to use
*Note: this tutorial and code only keep compatibility with latest RPiOS. If you are using old RPiOS, you are on your own.  
Check for kernel headers:
```bash
ls /lib/modules/`uname -r`/build
```
If not, install it:
```bash
sudo apt install linux-headers-rpi-v8 # for arm64，armv7 users install linux-headers-rpi-v7
```
Install `make` and `dtc`:
```bash
sudo apt install make device-tree-compiler
```
Clone:
```bash
git clone https://github.com/CNflysky/RPI_DSI_Displays
cd RPI_DSI_Displays/src
```
Compile driver:
```
make
```
Move `panel-rpi-dsi-displays.ko` into `/lib/modules`:
```bash
sudo cp panel-rpi-dsi-displays.ko /lib/modules/`uname -r`/kernel/drivers/gpu/drm/panel
sudo depmod
```
Compile device tree overlay：
```bash
dtc -I dts -O dtb -o vc4-kms-dsi-rpidisp.dtbo vc4-kms-dsi-rpidisp.dts
```
Move to `/boot/firmware/overlays`:
```bash
sudo cp vc4-kms-dsi-rpidisp.dtbo /boot/firmware/overlays
```
Edit `/boot/firmware/config.txt`, add these two lines:
```
ignore_lcd=1
dtoverlay=vc4-kms-dsi-rpidisp
```
Reboot.  
If you want to get adapters，take a look at [`adapters`](./adapters)directory.   
You can view it on OSHWHub too:[Link](https://oshwhub.com/cnflysky/RaspberryPi-DSI-Display)(Chinese version only)  

# Rotation
`vc4-kms-dsi-rpidisp.dts`:
```dts
panel:panel@0 {
				compatible    = "wlk,w280bf036i";
				status        = "okay";
				reg           = <0>;
				// reset-gpios   = <&gpio 44 0>;
				reset-gpios   = <&gpio 46 0>; // fix me
				backlight = <&panel_backlight>;
				rotation = <0>; // Rotation: [0, 90, 180, 270]
                ...
}
```

# Port your own panel driver
[Here](https://github.com/CNflysky/RPI_DSI_Drivers/blob/main/docs/how_to_make_your_custom_driver.md)  
*Translated from my blog,may not 100% accurate.*

# Supported(Or WIP) Panel
| Part Number | Diagonal | Resolution | Interface | Connector | TP | Note |
| ---- | ---- | --- | --- | --- | --- | -- |
|W280BF036I| 2.8 Inch| VGA(480x640) | DSI 1 Lane | 24p Connector | None | |

# Gallery
## W280BF036I
![w280bf036i_neofetch](https://user-images.githubusercontent.com/48781081/185400085-0ac27bf6-d49c-43aa-998f-bba86e3f03a0.jpg)
![w280bf036i_htop](https://user-images.githubusercontent.com/48781081/185400206-1bcef701-b001-4589-a75b-26b4d6db5c8d.jpg)
![w280bf036i_vim](https://user-images.githubusercontent.com/48781081/185400265-4c3b49c9-1a6e-41d2-a25e-e2c9e414bae6.jpg)
![w280bf036i_cmatrix](https://user-images.githubusercontent.com/48781081/185400713-61ed19be-53c0-41fe-b528-b3accef58a2d.gif)
![w280bf036i_ufotest](https://user-images.githubusercontent.com/48781081/185400433-1008b845-31b5-4f14-818f-27a5a4eac061.jpg)

# RPI_DSI_Displays
适用于树莓派DSI屏幕的硬件与驱动程序。  
*注意：本仓库仍在开发中. 如果您有任何意见或建议，欢迎提issue。*  
# 为什么使用DSI?

和传统的SPI/DPI屏幕相比，DSI屏幕具有以下优点:
- 刷新速率高(~60帧)
- 接线简单
- 占用GPIO数量少
- 功耗低

# 局限
## 电路局限  
由于树莓派的电路设计原因，RPi 4b板载DSI接口只引出了2 lane(最大分辨率720p)。  
如果想要驱动多于2 lane的屏幕，您可能得考虑使用*计算模块(compute module)*或者是*树莓派5*。
## 系统局限  
要使用本驱动，您必须确保启用了`DRM`。  
在2022-1-28日以后发布的`Raspberry Pi OS`已经默认启用了`DRM`。
旧版本的RPiOS可能不支持`DRM`，因此推荐使用最新版本的RPiOS。

# 如何使用
*注意：教程及代码只保证对于最新版(latest)RPi OS的兼容性。如果您使用的不是最新版系统，则教程可能不适用于您的系统。*  
检查内核头文件是否存在:
```bash
ls /lib/modules/`uname -r`/build
```
如果不存在，您需要安装对应的内核头文件:
```bash
sudo apt install linux-headers-rpi-v8 # arm64的内核头文件，如果是32位系统则需要安装linux-headers-rpi-v7
```
安装`make`和`dtc`:
```bash
sudo apt install make device-tree-compiler
```
克隆仓库:
```bash
git clone https://github.com/CNflysky/RPI_DSI_Displays
cd RPI_DSI_Displays/src
```
编译驱动:
```
make
```
编译完成后当前目录下会出现`panel-rpi-dsi-displays.ko`，将其复制到内核模块文件夹中:
```bash
sudo cp panel-rpi-dsi-displays.ko /lib/modules/`uname -r`/kernel/drivers/gpu/drm/panel
sudo depmod
```
复制完成后，编译设备树overlay文件：
```bash
dtc -I dts -O dtb -o vc4-kms-dsi-rpidisp.dtbo vc4-kms-dsi-rpidisp.dts
```
编译完成后:
```bash
sudo cp vc4-kms-dsi-rpidisp.dtbo /boot/firmware/overlays
```
编辑`/boot/firmware/config.txt`，加入下面两行:
```
ignore_lcd=1
dtoverlay=vc4-kms-dsi-rpidisp
```
重启。  
如果您需要自己制作转接板，请查看本仓库下的[`adapters`](./adapters)文件夹。  
您也可以在OSHWHub上查看:[链接](https://oshwhub.com/cnflysky/RaspberryPi-DSI-Display)  

# 配置
如果需要修改屏幕方向，编辑`vc4-kms-dsi-rpidisp.dts`:
```dts
panel:panel@0 {
				compatible    = "wlk,w280bf036i";
				status        = "okay";
				reg           = <0>;
				// reset-gpios   = <&gpio 44 0>;
				reset-gpios   = <&gpio 46 0>; // fix me
				backlight = <&panel_backlight>;
				rotation = <0>; // 屏幕方向，可选值为0, 90, 180, 270
                ...
}
```

# 移植您自己的屏幕驱动
您可以参考我写的这篇文章：[CSDN链接](https://blog.csdn.net/CNflysky/article/details/123119666)  

# 已经适配(或者正在适配)的屏幕
| 料号 | 尺寸 | 分辨率 | 接口 | 连接器 | 触摸 | 备注 |
| ---- | ---- | --- | --- | --- | --- | --- |
|W280BF036I| 2.8 Inch| VGA(480x640) | DSI 1 Lane | 24p 连接器 | 无 | |

# 展示
## W280BF036I
![w280bf036i_neofetch](https://user-images.githubusercontent.com/48781081/185400085-0ac27bf6-d49c-43aa-998f-bba86e3f03a0.jpg)
![w280bf036i_htop](https://user-images.githubusercontent.com/48781081/185400206-1bcef701-b001-4589-a75b-26b4d6db5c8d.jpg)
![w280bf036i_vim](https://user-images.githubusercontent.com/48781081/185400265-4c3b49c9-1a6e-41d2-a25e-e2c9e414bae6.jpg)
![w280bf036i_cmatrix](https://user-images.githubusercontent.com/48781081/185400713-61ed19be-53c0-41fe-b528-b3accef58a2d.gif)
![w280bf036i_ufotest](https://user-images.githubusercontent.com/48781081/185400433-1008b845-31b5-4f14-818f-27a5a4eac061.jpg)

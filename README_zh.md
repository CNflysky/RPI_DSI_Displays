# RPI_DSI_Drivers
为树莓派设计的几种DSI屏幕的驱动程序。  
*注意：本仓库仍在开发中. 如果您有任何意见或建议，欢迎提issue。*  
# 为什么使用DSI?

和传统的SPI/DPI屏幕相比，DSI屏幕具有以下优点:
- 刷新速率高(~60帧)
- 接线简单
- 占用GPIO数量较少
- 功耗较低

# 局限
由于树莓派的电路设计原因，目前板载DSI接口只支持2 lane(最大分辨率720p)。  
如果想要驱动多于2 lane的屏幕，您可能得考虑使用*计算模块(compute module)*。  


# 如何使用
首先克隆本仓库到您的树莓派中：  
```bash
git clone https://github.com/CNflysky/RPI_DSI_Drivers
```
然后，运行:
```bash
sudo ./lcd.sh
```
如果您需要自己制作转接板，请查看本仓库下的[`adapters`](https://github.com/CNflysky/RPI_DSI_Drivers/tree/main/adapters)文件夹。  
您也可以在OSHWHub上查看:[链接](https://oshwhub.com/cnflysky/RaspberryPi-DSI-Display)(目前只有中文版)  

# 如何制作自己的驱动
您可以参考我写的这篇文章：[CSDN链接](https://blog.csdn.net/CNflysky/article/details/123119666)  

# 已经适配(或者正在适配)的屏幕
| 料号 | 尺寸 | 分辨率 | 接口 | 连接器 | 触摸 | 备注 |
| ---- | ---- | --- | --- | --- | --- | --- |
|W280BF036I| 2.8 Inch| VGA(480x640) | MIPI 1 Lane | 24p 连接器 | 无 | |
|W500IE020| 5.0 Inch | FWVGA(480x854) | MIPI 2 Lane | 30p 连接器 | 无 | 正在适配 |

# 展示
## W280BF036I
![w280bf036i_neofetch](https://user-images.githubusercontent.com/48781081/185400085-0ac27bf6-d49c-43aa-998f-bba86e3f03a0.jpg)
![w280bf036i_htop](https://user-images.githubusercontent.com/48781081/185400206-1bcef701-b001-4589-a75b-26b4d6db5c8d.jpg)
![w280bf036i_vim](https://user-images.githubusercontent.com/48781081/185400265-4c3b49c9-1a6e-41d2-a25e-e2c9e414bae6.jpg)
![w280bf036i_cmatrix](https://user-images.githubusercontent.com/48781081/185400713-61ed19be-53c0-41fe-b528-b3accef58a2d.gif)
![w280bf036i_ufotest](https://user-images.githubusercontent.com/48781081/185400433-1008b845-31b5-4f14-818f-27a5a4eac061.jpg)

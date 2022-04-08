# RPI_DSI_Drivers
为树莓派设计的几种DSI屏幕的驱动程序。

# 为什么使用DSI?

和传统的SPI/DPI屏幕相比，DSI屏幕具有以下优点:
- 刷新速率高(~60帧)
- 易于连接
- 占用GPIO数量较少
- 功耗较低

# 如何使用
首先您需要克隆本仓库到您的树莓派中：  
```bash
git clone https://github.com/CNflysky/RPI_DSI_Drivers
```
然后，运行:
```bash
sudo chmod 755 lcd.sh && sudo ./lcd.sh
```
如果您需要自己制作转接板，请查看本仓库下的[`adapters`](https://github.com/CNflysky/RPI_DSI_Drivers/tree/main/adapters)文件夹。  
# 如何移植自己的驱动
您可以参考我写的这篇文章：[CSDN链接](https://blog.csdn.net/CNflysky/article/details/123119666)  

# 已经适配(或者正在适配)的屏幕
| 料号 | 尺寸 | 分辨率 | 接口 | 连接器 | 触摸 | 备注 |
| ---- | ---- | --- | --- | --- | --- | --- |
|W280BF036I| 2.8 Inch| VGA(480x640) | MIPI 1 Lane | 24p 连接器 | 无 | |
|W500IE020| 5.0 Inch | FWVGA(480x854) | MIPI 2 Lane | 30p 连接器 | 无 | 正在适配 |

# 展示
## W280BF036I
![neofetch](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_neofetch.jpg)
![htop](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_htop.jpg)
![vim](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_vim.jpg)
![cmatrix](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_cmatrix.gif)
![testufo](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_ufotest.jpg)

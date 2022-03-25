# RPI_DSI_Drivers
为树莓派设计的几种DSI屏幕的驱动程序。

# 为什么使用DSI?

和传统的SPI/DPI屏幕相比，DSI屏幕具有以下优点:
- 刷新速率高
- 易于连接
- 占用GPIO数量较少
- 功耗较低

# 如何使用
首先您需要克隆本仓库到您的树莓派中：  
```bash
git clone https://github.com/CNflysky/RPI_DSI_Drivers
```
然后您需要根据您的屏幕型号找到需要的驱动：  
```bash
cd RPI_DSI_Drivers/w280bf036i
```
随后您只需要简单地执行:  
```bash
sudo chmod 755 lcd.sh && sudo ./lcd.sh install
```
安装完成后重启即可。  
如果需要卸载驱动，请回到上述文件夹，执行:  
```bash
sudo ./lcd.sh remove
```
如果您需要自己制作转接板，请查看本仓库下的`adapters`文件夹。
重启后即可卸载驱动。
# 如何移植自己的驱动
您可以参考我写的这篇文章：[CSDN链接](https://blog.csdn.net/CNflysky/article/details/123119666)  

# 已经适配的屏幕
| 料号 | 尺寸 | 分辨率 | 接口 | 连接器 | 备注 |
| ---- | ---- | --- | --- | --- | --- |
|W280BF036I| 2.8 Inch| VGA(480x640) | MIPI 1 Lane | 24p 连接器 ||

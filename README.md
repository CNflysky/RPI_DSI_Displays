# RPI_DSI_Drivers
Various DSI display drivers for the Raspberry PI.  
[Simplified Chinese](https://github.com/CNflysky/RPI_DSI_Drivers/blob/main/README_zh.md)
# Why DSI?

compared from legacy SPI/DPI panels，DSI panels have taken these advantages:
- High refresh rates(~60fps)
- Easy to wire
- low occupancy to gpios
- low power consumption

# How to use
Clone this repository to your Raspberry PI：  
```bash
git clone https://github.com/CNflysky/RPI_DSI_Drivers
```
then find the driver corresponding to your panel：  
```bash
cd RPI_DSI_Drivers/w280bf036i
```
then simply run:  
```bash
sudo chmod 755 lcd.sh && sudo ./lcd.sh install
```
and reboot .
if you need to remove drivers，return back to driver directory，run:  
```bash
sudo ./lcd.sh remove
```
and reboot.  
if you wants to get adapters，take a look at [`adapters`](https://github.com/CNflysky/RPI_DSI_Drivers/tree/main/adapters)directory. 
# Make your own customized driver
[Here](https://github.com/CNflysky/RPI_DSI_Drivers/blob/main/docs/how_to_make_your_custom_driver.md)  
*Translated from my Chinese blog,may not 100% accurate.*

# Supported Panel
| Part Number | Size | Resolution | Interface | Connector | Note |
| ---- | ---- | --- | --- | --- | --- |
|W280BF036I| 2.8 Inch| VGA(480x640) | MIPI 1 Lane | 24p Connector ||

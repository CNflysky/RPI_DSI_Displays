# RPI_DSI_Drivers
Various DSI display drivers for the Raspberry PI.  
[Simplified Chinese](https://github.com/CNflysky/RPI_DSI_Drivers/blob/main/README_zh.md)

# Why DSI?

compared from SPI/DPI panels，DSI panels have taken these advantages:
- High refresh rates(~60fps)
- Easy to wire
- Low occupancy to gpios
- Low power consumption

# Limitation
Beasuse of Raspberry Pi Boards(3b,4b) have only 2 lanes on dsi1 so the max resolution have been limited by 720p.  
If you want drive panel more than 2 lanes,you may need to consider the compute module.  

# How to use
Clone this repository to your Raspberry PI：  
```bash
git clone https://github.com/CNflysky/RPI_DSI_Drivers
```
Then run:
```bash
sudo chmod 755 lcd.sh && sudo ./lcd.sh
```  
if you wants to get adapters，take a look at [`adapters`](https://github.com/CNflysky/RPI_DSI_Drivers/tree/main/adapters)directory. 
# Make your own customized driver
[Here](https://github.com/CNflysky/RPI_DSI_Drivers/blob/main/docs/how_to_make_your_custom_driver.md)  
*Translated from my blog,may not 100% accurate.*

# Supported(Or WIP) Panel
| Part Number | Size | Resolution | Interface | Connector | TP | Note |
| ---- | ---- | --- | --- | --- | --- | -- |
|W280BF036I| 2.8 Inch| VGA(480x640) | MIPI 1 Lane | 24p Connector | None | |
|W500IE020| 5.0 Inch | FWVGA(480x854) | MIPI 2 Lane | 30p Connector | None | Working in progress |

# Gallery
## W280BF036I
![neofetch](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_neofetch.jpg)
![htop](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_htop.jpg)
![vim](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_vim.jpg)
![cmatrix](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_cmatrix.gif)
![testufo](https://github.com/CNflysky/RPI_DSI_Drivers/raw/main/images/w280bf036i/w280bf036i_ufotest.jpg)

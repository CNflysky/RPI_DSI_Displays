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
Due to Raspberry Pi boards' circuit design, currently on-board DSI connector only support 2 lanes(max resolution 720p).
If you want drive panel more than 2 lanes,you may need to consider the compute module.  

# How to use
Clone this repository on your Raspberry Pi：  
```bash
git clone https://github.com/CNflysky/RPI_DSI_Drivers
```
Then run:
```bash
sudo chmod 755 lcd.sh && sudo ./lcd.sh
```  
if you wants to get adapters，take a look at [`adapters`](https://github.com/CNflysky/RPI_DSI_Drivers/tree/main/adapters)directory. 
or you can view it on the OSHWHub:[Link](https://oshwhub.com/cnflysky/RaspberryPi-DSI-Display)(Chinese version only)  

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
![w280bf036i_neofetch](https://user-images.githubusercontent.com/48781081/185400085-0ac27bf6-d49c-43aa-998f-bba86e3f03a0.jpg)
![w280bf036i_htop](https://user-images.githubusercontent.com/48781081/185400206-1bcef701-b001-4589-a75b-26b4d6db5c8d.jpg)
![w280bf036i_vim](https://user-images.githubusercontent.com/48781081/185400265-4c3b49c9-1a6e-41d2-a25e-e2c9e414bae6.jpg)
![w280bf036i_cmatrix](https://user-images.githubusercontent.com/48781081/185400713-61ed19be-53c0-41fe-b528-b3accef58a2d.gif)
![w280bf036i_ufotest](https://user-images.githubusercontent.com/48781081/185400433-1008b845-31b5-4f14-818f-27a5a4eac061.jpg)

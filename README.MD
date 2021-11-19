
<h1 align = "center">🌟LilyGo T-Call SIM800 Series🌟</h1>

![](image/Product.jpg)

<h2 align = "left">Product 📷:</h2>

|                      Examples                      |                           Product  Link                            |                          Schematic                          | Status | Recommended level |
| :------------------------------------------------: | :----------------------------------------------------------------: | :---------------------------------------------------------: | :----: | :---------------: |
| [SIM800L IP5306 20190610](./doc/SIM800L_IP5306.MD) |  [Product link](https://www.aliexpress.com/item/33045221960.html)  | [Schematic](./schematic/LilyGo-SIM800L-IP5306-20190610.pdf) |   ✅    |         ⭐         |
| [SIM800L IP5306 20200811](./doc/SIM800L_IP5306.MD) | [Product link](https://www.aliexpress.com/item/4000959701330.html) | [Schematic](./schematic/LilyGo-SIM800L-IP5306-20200811.pdf) |   ✅    |        ⭐⭐         |
| [SIM800L AXP192 20200327](./doc/SIM800L_AXP192.MD) | [Product link](https://www.aliexpress.com/item/4000959701330.html) |  [Schematic](./schematic/LilyGo-SIM800L-PMU-20200409.pdf)   |   ✅    |        ⭐⭐⭐        |
| [SIM800C AXP192 20200609](./doc/SIM800C_AXP192.MD) | [Product link](https://www.aliexpress.com/item/4001274909689.html) |    [Schematic](./schematic/LilyGo-SIM800C-20200630.pdf)     |   ✅    |        ⭐⭐⭐        |

## Datasheet
- [SIM800 AT Command](./datasheet/sim800_series_at_command_manual_v1.01.pdf)
- [SIM800 Hardware Design](./datasheet/SIM800_Hardware%20Design_V1.08.pdf)
- [AXP192](http://www.x-powers.com/en.php/Info/product_detail/article_id/29)
- [IP5306](./datasheet/IP5306%20REG%20V1.4.pdf)


## FAQ

### 1. If you are unable to download, please try the following steps, if it still does not work, please contact customer service
1. Open the serial monitor
2. Short IO0 to GND
3. Press the RESET button on the board, if the serial port communication is normal, then you should see
    ```
    ets Jun 8 2016 00:22:57

    rst:0x1 (POWERON_RESET),boot:0x3 (DOWNLOAD_BOOT(UART0/UART1/SDIO_REI_REO_V2))
    waiting for download
    ```
4. If you can see the above message, it means that the communication is normal, then all you need is to let it enter the waiting download mode, then you can click upload, usually it can solve the problem of not being able to download automatically

### 2. About the 5V power pin description on the board:

1. For IP5306 power supply, the 5V power pin on the board is the output, which is boosted by the IP5306 power chip. When using USB and battery, it can provide 5V output. According to the manual, it can provide a maximum output current of 2.4A .

2. When using AXP192 power supply, the 5V power pin on the board is the input, and it is connected with USB 5V+. There is no other power output on the board except for the 3.3V output.


### 3. Didn’t connect to the Internet for a long time, or just turned off suddenly?
 
1. It may be due to insufficient USB power supply current, please connect a lithium battery to the board, then plug in the USB and test again.


### 4. Why doesn’t the board turn on when the lithium battery is connected? 

1. If you use the **IP5306** version and use a lithium battery to supply power, you must press the reset button to turn on the board. 

2. If you use the **AXP192** power management chip, you need to press and hold the PWR button for 2 seconds to turn on the power supply. By default, the AXP192 will be powered on when the battery is inserted. 


### 5. About SIM800X built-in firmware update.

1. [How to update firmware](doc/How%20to%20update%20firmware.md)

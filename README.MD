
<h1 align = "center">🌟LilyGo T-Call SIM800 Series🌟</h1>

![](image/Product.jpg)

<h2 align = "left">Product 📷:</h2>

|                      Examples                      |                                 Product  Link                                 |                          Schematic                          |
| :------------------------------------------------: | :---------------------------------------------------------------------------: | :---------------------------------------------------------: |
| [SIM800L IP5306 20200811](./doc/SIM800L_IP5306.MD) | [Product link](https://lilygo.cc/products/t-call-v1-4?variant=42868250869941) | [Schematic](./schematic/LilyGo-SIM800L-IP5306-20200811.pdf) |


## Datasheet

- [SIM800 AT Command](./datasheet/sim800_series_at_command_manual_v1.01.pdf)
- [SIM800 Hardware Design](./datasheet/SIM800_Hardware%20Design_V1.08.pdf)
- [AXP192](http://www.x-powers.com/en.php/Info/product_detail/article_id/29)
- [IP5306](./datasheet/IP5306%20REG%20V1.4.pdf)

## PlatformIO Quick Start

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `LilyGo-T-Call-SIM800` directory
5. Wait for the installation of third-party dependent libraries to complete
6. Click on the `platformio.ini` file, and in the `platformio` column
7. Uncomment one of the lines `src_dir = xxxx` to make sure only one line works
8. Click the (✔) symbol in the lower left corner to compile
9. Connect the board to the computer USB
10. Click (→) to upload firmware
11. Click (plug symbol) to monitor serial output
12. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

## Arduino IDE Manual installation

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install [Arduino ESP32 V2.0.5 or later or latest](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
3. Download `LilyGo-T-Call-SIM800` , move to Arduino library folder (e.g. C:\Users\YourName\Documents\Arduino\libraries)
4. Copy all folders in [lib folder](./lib/) to Arduino library folder (e.g. C:\Users\YourName\Documents\Arduino\libraries)
5. Open ArduinoIDE ,`Tools` , Make your selection according to the table below
   | Arduino IDE Setting                  | Value                             |
   | ------------------------------------ | --------------------------------- |
   | Board                                | **ESP32 Dev Module**            |
   | Port                                 | Your port                         |
   | USB CDC On Boot                      | Enable                            |
   | CPU Frequency                        | 240MHz (WiFi/BT)                      |
   | Core Debug Level                     | None                              |
   | Erase All Flash Before Sketch Upload | Disable                           |
   | Events Run On                        | Core1                             |
   | Flash Mode                           | QIO 80MHZ                         |
   | Flash Size                           | **4MB (32Mb)**                   |
   | Arduino Runs On                      | Core1                             |
   | Partition Scheme                     | **Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)** |
   | PSRAM                                | **QSPI PSRAM**                     |
   | Upload Mode                          | **UART0/Hardware CDC**            |
   | Upload Speed                         | 921600                            |

   - The options in bold are required, others are selected according to actual conditions.

6. `File` -> `Examples` -> `LilyGo-T-Call-SIM800` -> `Arduino Examples`
7. Select `Port`
8. Click `upload` , Wait for compilation and writing to complete

## FAQ

### 1. If you are unable to download, please try the following steps, if it still does not work, please contact customer service

1. Open the serial monitor
2. Short IO0 to GND
3. Press the RESET button on the board, if the serial port communication is normal, then you should see

    ```bash
    ets Jun 8 2016 00:22:57

    rst:0x1 (POWERON_RESET),boot:0x3 (DOWNLOAD_BOOT(UART0/UART1/SDIO_REI_REO_V2))
    waiting for download
    ```

4. If you can see the above message, it means that the communication is normal, then all you need is to let it enter the waiting download mode, then you can click upload, usually it can solve the problem of not being able to download automatically

### 2. About the 5V power pin description on the board

   - For IP5306 power supply, the 5V power pin connected to the USB board is the output, and the voltage is boosted by IP5306 power supply chip.  According to the manual, its maximum output current is 2.4a.  Do not connect USB, can 5V input.
   - When using AXP192 power supply, the 5V power pin on the board is the input, and it is connected with USB 5V+. There is no other power output on the board except for the 3.3V output.

### 3. Didn’t connect to the Internet for a long time, or just turned off suddenly?

   - It may be due to insufficient USB power supply current, please connect a lithium battery to the board, then plug in the USB and test again.


### 4. Why doesn’t the board turn on when the lithium battery is connected?

   - If you use the **IP5306** version and use a lithium battery to supply power, you must press the reset button to turn on the board. 
   - If you use the **AXP192** power management chip, you need to press and hold the PWR button for 2 seconds to turn on the power supply. By default, the AXP192 will be powered on when the battery is inserted. 


### 5. About SIM800X built-in firmware update

   - SIM800 built-in firmware update tool Because SIMCOM has not adapted and updated the new system for a long time, it is not necessarily available to upgrade the built-in firmware tool. Most users are stuck waiting for download. Please be informed
   - [How to update firmware](doc/How%20to%20update%20firmware.md)

### 6. LED indicators

- Blue LED, controlled by GPIO13
- Red LED, network status light
  
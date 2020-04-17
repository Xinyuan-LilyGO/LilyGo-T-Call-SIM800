
<h1 align = "center">🌟LilyGo T-Call SIM800L🌟</h1>

## **English | [中文](docs/cn.md)**


AXP192 is the power supply used on the SIM800L board. It is an upgraded version of the IP5306 version. The following points are upgraded:
1. Change the power IC to AXP192 PMU management, support coulomb counter and high-precision voltage and current detection function, please refer to [X-Power official website](http://www.x-powers.com/en.php/Info/product_detail/article_id/29)
2. SIM800L **RI** Pin leads to ESP32, can wake up from receiving voice or data calls, and receiving messages
3. SIM800L **DTR** Pin leads to ESP32, which can wake up SIM800L from sleep state
4. The old and new versions are fully compatible and can be used as general examples. You don't need to care about the control of the AXP192 power chip, you can replace the new board on your original code without programming the AXP192, and provide AXP192 control example code

## Examples for SIM800L IP5306 Version  [Click me view PINOUT](./image/SIM800L_IP.jpg)

- Loboris Micropython: [/examples/MicroPython_LoBo](/examples/MicroPython_LoBo)
- Arduino + TinyGSM library: [/examples/Arduino_TinyGSM](/examples/Arduino_TinyGSM)
- Arduino + SIM800 library: [/examples/Arduino_sim800](/examples/Arduino_sim800)
- Arduino OTA update over GSM: [/examples/Arduino_GSM_OTA](/examples/Arduino_GSM_OTA)
- Arduino Deepsleep: [/examples/Arduino_Deepsleep](/examples/Arduino_Deepsleep)
- ESP-IDF: https://github.com/espressif/esp-idf/tree/master/examples/protocols/pppos_client

## Examples for SIM800L AXP192 Version [Click me view PINOUT](./image/SIM800L_AXP.jpg)

- Arduino + TinyGSM library: [/examples/Arduino_TinyGSM_AXP192](/examples/Arduino_TinyGSM_AXP192)


## Datasheet
- [SIM800 AT Command](./datasheet/sim800_series_at_command_manual_v1.01.pdf)
- [SIM800 Hardware Design](./datasheet/SIM800_Hardware%20Design_V1.08.pdf)
- [AXP192](http://www.x-powers.com/en.php/Info/product_detail/article_id/29)
- [IP5306](./datasheet/IP5306%20REG%20V1.4.pdf)
- [IP5306 Version Schematic](./datasheet/TTGO_T-Call_SIM800_v1.3_schematic.pdf)
- [AXP192 Version Schematic](./datasheet/LilyGo-SIM800L-PMU-20200409.pdf)

##  Pin definition
|     Name     | [IP5306 Version](https://www.aliexpress.com/item/33045221960.html) | AXP192 Version |
| :----------: | :----------------------------------------------------------------: | :------------: |
|  SIM800_TX   |                                 26                                 |       26       |
|  SIM800_RX   |                                 27                                 |       27       |
| SIM800_PWKEY |                                 4                                  |       4        |
|  SIM800_RST  |                                 5                                  |       5        |
| SIM800_POWER |                                 23                                 |       23       |
|  SIM800_DTR  |                                 ❌                                  |       32       |
|  SIM800_RI   |                                 ❌                                  |       33       |
|     SDA      |                                 21                                 |       21       |
|     SCL      |                                 22                                 |       22       |
|   AXP IRQ    |                                 ❌                                  |       35       |
|     LED      |                                 ❌                                  |       13       |





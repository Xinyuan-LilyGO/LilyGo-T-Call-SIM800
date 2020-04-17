
<h1 align = "center">🌟LilyGo T-Call SIM800L🌟</h1>

## **[English](../README.MD) | 中文**


AXP192为SIM800L板子上使用的电源，它是IP5306版本的升级版本,升级以下几点:
1. 更改电源IC为AXP192 PMU管理，支持库仑计和高精度电压电流检测功能,详情请查阅 [X-Power官网](http://www.x-powers.com/en.php/Info/product_detail/article_id/29)
2. SIM800L **RI** Pin引出到ESP32 ，可以从接受语音或数据呼叫，接收到消息中唤醒
3. SIM800L **DTR** Pin引出到ESP32 ，可以使SIM800L从睡眠状态中唤醒
4. 新旧版本完全兼容,可以通用示例,你完全可以不需要关心AXP192电源芯片的控制,无需对AXP192进行编程也可以在你原有代码上替换新板，提供AXP192控制示例代码


## SIM800L IP5306 版本示例 [点击我查看引脚图](../image/SIM800L_IP.jpg)

- Loboris Micropython: [/examples/MicroPython_LoBo](/examples/MicroPython_LoBo)
- Arduino + TinyGSM library: [/examples/Arduino_TinyGSM](/examples/Arduino_TinyGSM)
- Arduino + SIM800 library: [/examples/Arduino_sim800](/examples/Arduino_sim800)
- Arduino OTA update over GSM: [/examples/Arduino_GSM_OTA](/examples/Arduino_GSM_OTA)
- Arduino Deepsleep: [/examples/Arduino_Deepsleep](/examples/Arduino_Deepsleep)
- ESP-IDF: https://github.com/espressif/esp-idf/tree/master/examples/protocols/pppos_client

## SIM800L AXP192 版本示例 [点击我查看引脚图](../image/SIM800L_AXP.jpg)
- Arduino + TinyGSM library: [/examples/Arduino_TinyGSM_AXP192](/examples/Arduino_TinyGSM_AXP192)

## 数据手册
- [SIM800 AT Command](../datasheet/sim800_series_at_command_manual_v1.01.pdf)
- [SIM800 Hardware Design](../datasheet/SIM800_Hardware%20Design_V1.08.pdf)
- [AXP192](http://www.x-powers.com/en.php/Info/product_detail/article_id/29)
- [IP5306](../datasheet/IP5306%20REG%20V1.4.pdf)
- [IP5306 Version Schematic](../datasheet/TTGO_T-Call_SIM800_v1.3_schematic.pdf)
- [AXP192 Version Schematic](../datasheet/LilyGo-SIM800L-PMU-20200409.pdf)


## 引脚定义
|     Name     | [IP5306 Version](https://www.aliexpress.com/item/33045221960.html) | AXP192 Version |
| :----------: | :----------------------------------------------------------------: | :------------: |
|  SIM800_TX   |                                 26                                 |       26       |
|  SIM800_RX   |                                 27                                 |       27       |
| SIM800_PWKEY |                                 4                                  |       4        |
|  SIM800_RST  |                                 5                                  |       5        |
| SIM800_POWER |                                 23                                 |       23       |
|  SIM800_DTR  |                                 ❌                                  |       32       |
|  SIM800_RI   |                                 ❌                                  |       33       |
|     SDA      |                                 ❌                                  |       21       |
|     SCL      |                                 ❌                                  |       22       |
|    POWER     |                                 ❌                                  |       35       |
|     LED      |                                 ❌                                  |       13       |





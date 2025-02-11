# Cayenne MQTT Arduino Library
The Cayenne MQTT Arduino Library provides functions to easily connect to the [Cayenne IoT project builder](https://www.cayenne-mydevices.com). This library is designed to work with Arduino, ESP8266 and ESP32 devices. With it you can send data to and receive data from Cayenne.

![alt text](https://upload.wikimedia.org/wikipedia/commons/3/38/Arduino_Uno_-_R3.jpg)

[Arduino Uno image](https://commons.wikimedia.org/wiki/File%3AArduino_Uno_-_R3.jpg) by [SparkFun](https://www.sparkfun.com) is licensed under [CC BY 2.0](https://creativecommons.org/licenses/by/2.0/)

![alt text](https://cdn-learn.adafruit.com/assets/assets/000/024/792/medium640/adafruit_products_2471_iso_assembled_01_ORIG.jpg?1429908417)

[Adafruit HUZZAH ESP8266 image](https://learn.adafruit.com/assets/24792) by [lady ada](https://learn.adafruit.com/users/adafruit2) is licensed under [CC BY-SA 3.0](https://creativecommons.org/licenses/by-sa/3.0/)

![alt text](https://cdn.sparkfun.com//assets/parts/1/1/5/6/4/13907-01.jpg)

[SparkFun ESP32 Thing image](https://www.sparkfun.com/products/13907) by [SparkFun](https://www.sparkfun.com) is licensed under [CC BY 2.0](https://creativecommons.org/licenses/by/2.0/)

## Requirements
### Hardware
* An Arduino, ESP8266 or ESP32 module, e.g. the [Arduino Uno](https://store.arduino.cc/usa/arduino-uno-rev3), [Adafruit HUZZAH ESP8266](https://www.adafruit.com/product/2471) or [SparkFun ESP32 Thing](https://www.sparkfun.com/products/13907).
* A USB cable, or if using certain ESP devices, a USB to Serial FTDI cable.

### Software
* [Arduino IDE](https://www.arduino.cc/en/Main/Software) for Windows, Linux or Mac OS.
* [This library](https://github.com/myDevicesIoT/Cayenne-MQTT-Arduino/archive/master.zip).

## Getting Started
### Environment Setup
1. Download and install the [Arduino IDE](https://www.arduino.cc/en/Main/Software).
2. Install the **CayenneMQTT** library from **Sketch -> Include Library -> Manage Libraries**.
3. If using an ESP module, install the Board Package.
   1. For **ESP8266**:
      1. Under **File -> Preferences** add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to the **Additional Boards Manager URLs** field.
      2. Install the **esp8266** platform from **Tools -> Board -> Boards Manager**.
   2. For **ESP32**, manually install the board package by following the instructions here: https://github.com/espressif/arduino-esp32/blob/master/README.md#installation-instructions.
4. Connect your device to your computer using the appropriate USB cable or USB to Serial FTDI cable.
5. Select your Arduino board or ESP module and and the correct port from the **Tools** menu.

### Cayenne Setup
1. Create your Cayenne account at https://www.cayenne-mydevices.com.
2. Add a new device using the **Add new...** page **MicroControllers** or **Bring Your Own Thing** options.

### Building Examples
1. Open an included example connection sketch from **File -> Examples -> CayenneMQTT -> Connections**.
2. Modify the included sketch with the Cayenne authentication info you received when adding your device.
3. If using an ESP module or WiFi shield modify the sketch with your network info.
3. Compile and upload the sketch to your device. For ESP devices uploading may require setting the device into bootload mode. For the Adafruit HUZZAH ESP8266 module this is done with the folowing steps:
   1. Hold down the **GPIO0** button.
   2. Click the **Reset** button.
   3. Let go of the **GPIO0** button.
   4. The red LED should light up, indicating the device is ready for uploading.
   5. Upload your sketch.
4. Check the Cayenne dashboard to ensure it is receiving sample data from your device.

## Compatibility
This library was tested with various Arduino boards including the Uno, Mega, Due, etc. as well as the Adafruit HUZZAH ESP8266 breakout module and an Espressif ESP32 development board. It should also work with other standard ESP8266 and ESP32 modules that are compatible with the Arduino IDE. Boards with very constrained memory like the Arduino Leonardo or Micro may have issues fitting this library alongside other libraries. For those boards you might try using a manually specified IP as shown in the [ManualConnection example sketch](https://github.com/myDevicesIoT/Cayenne-MQTT-Arduino/blob/master/examples/Connections/ManualConnection/ManualConnection.ino) in order to save space.

## Contributions
* This library includes the [Eclipse Paho MQTT C/C++ client](https://github.com/eclipse/paho.mqtt.embedded-c).

## Cayenne MQTT Libraries
Additional libraries are available for connecting to Cayenne on other platforms and devices. These can be found at https://github.com/myDevicesIoT.

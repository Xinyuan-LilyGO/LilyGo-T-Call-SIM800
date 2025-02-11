/*
Cayenne TMP102 Example

This sketch shows how to send TMP102 Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach a TMP102 to your Arduino.
   Schematic:
   TMP102    Arduino
   [VCC] --- [3V3]
   [GND] --- [GND]
   [ADD0] -- [GND]
   [SDA] --- [Analog Pin 4] (The SDA may be different on some devices, e.g. for Arduino Mega the SDA pin is Digital Pin 20)
   [SCL] --- [Analog Pin 5] (The SCL may be different on some devices, e.g. for Arduino Mega the SCL pin is Digital Pin 21)
2. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of the TMP102 Sensor widget you have added) in the Dashboard.
3. Set the tmp102Address variable to match the ADD0. ADD0 connected to GND corresponds to an address of 0x48.
4. Set the Cayenne authentication info to match the authentication info from the Dashboard.
5. Compile and upload this sketch.
6. Once the Arduino connects to the Dashboard it should automatically create temporary display widgets (or update the TMP102 Sensor widgets you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial   // Comment this out to disable prints and save space
#include <CayenneTMP102.h>
#include <CayenneMQTTEthernet.h>   // Change this to use a different communication device. See Communications examples.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define VIRTUAL_CHANNEL 1

// Address used to read from the TMP102. This is determined by the ADD0 pin on the TMP102.
// Connecting it to ground means the sensor will use 0x48 for the address.  See the TMP102 datasheet for more info.
const int tmp102Address = 0x48;

TMP102 tmpSensor(tmp102Address);

void setup()
{
	Serial.begin(9600);
	Wire.begin();
	Cayenne.begin(username, password, clientID);
}

void loop()
{
	Cayenne.loop();
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
	// This command writes the temperature in Celsius to the Virtual Channel.
	Cayenne.celsiusWrite(VIRTUAL_CHANNEL, tmpSensor.getCelsius());
	// To send the temperature in Fahrenheit or Kelvin use the corresponding code below.
	//Cayenne.fahrenheitWrite(VIRTUAL_CHANNEL, tmpSensor.getFahrenheit());
	//Cayenne.kelvinWrite(VIRTUAL_CHANNEL, tmpSensor.getKelvin());
}

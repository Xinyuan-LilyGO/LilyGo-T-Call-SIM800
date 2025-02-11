/*
Cayenne Analog Load Pressure Sensor Example

This sketch shows how to  send Analog Load Pressure Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach an analog load pressure sensor to an analog pin on your Arduino.
   Make sure to use an analog pin, not a digital pin.
   Schematic:
   [Ground -- [10k-resistor] -- | -- [Analog Load Pressure Sensor] -- [5V]
                                |
                           Analog Pin
2. Set the SENSOR_PIN value below to the pin number you used when connecting the sensor.
3. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of an Analog Load Pressure Sensor widget you have added) in the Dashboard.
4. Set the Cayenne authentication info to match the authentication info from the Dashboard.
5. Compile and upload this sketch.
6. Once the Arduino connects to the Dashboard it should automatically create a temporary display widget (or update the Analog Load Pressure Sensor widget you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define SENSOR_PIN 0
#define VIRTUAL_CHANNEL 1

void setup()
{
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
}

void loop()
{
	Cayenne.loop();
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
	Cayenne.virtualWrite(VIRTUAL_CHANNEL, analogRead(SENSOR_PIN));
}

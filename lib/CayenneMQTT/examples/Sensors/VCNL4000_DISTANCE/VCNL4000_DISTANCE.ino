/*
Cayenne VCNL4000 Distance Example

This sketch shows how to send VCNL4000 Distance Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach a VCNL4000 to your Arduino.
   Schematic:
   VCNL4000      Arduino
   [VDD] ------- [3V3]
   [GND] ------- [GND]
   [SDA] ------- [Analog Pin 4] (The SDA may be different on some devices, e.g. for Arduino Mega the SDA pin is Digital Pin 20)
   [SCL] ------- [Analog Pin 5] (The SCL may be different on some devices, e.g. for Arduino Mega the SCL pin is Digital Pin 21)
2. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of the VCNL4000 Distance Sensor widget you have added) in the Dashboard.
3. Set the Cayenne authentication info to match the authentication info from the Dashboard.
4. Compile and upload this sketch.
5. Once the Arduino connects to the Dashboard it should automatically create temporary display widget (or update the VCNL4000 Distance Sensor widget you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneVCNL4000.h>
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define VIRTUAL_CHANNEL 1

VCNL4000 proximitySensor;
bool sensorDetected = true;

void setup()
{
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
	if (!proximitySensor.begin())
	{
		CAYENNE_LOG("No sensor detected");
		sensorDetected = false;
	}
}

void loop()
{
	Cayenne.loop();
}

// This function is called at intervals to send distance data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
	if (sensorDetected)
	{
		//The getMillimeters() function only provides a rough estimate of distance. If greater accuracy is desired
		//settings can be tweaked in CayenneVCNL400.h.
		int distance = proximitySensor.getMillimeters();
		if (distance != NO_PROXIMITY) {
			Cayenne.virtualWrite(VIRTUAL_CHANNEL, distance, TYPE_PROXIMITY, UNIT_MILLIMETER);
		}
	}
	else
	{
		CAYENNE_LOG("No sensor detected");
	}
}
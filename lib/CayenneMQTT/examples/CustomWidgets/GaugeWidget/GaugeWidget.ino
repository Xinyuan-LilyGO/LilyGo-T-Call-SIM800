/*
Cayenne Gauge Widget Example

This sketch shows how to set up a Gauge Custom Widget with Cayenne.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach an analog input device (e.g. a temperature sensor) to your Arduino.
2. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of a Gauge Custom Widget you have added) in the Dashboard.
3. Update the CAYENNE_OUT function below to send the data from your sensor.
4. Set the Cayenne authentication info to match the authentication info from the Dashboard.
5. Compile and upload this sketch.
6. Once the Arduino connects to the Dashboard it should automatically create a temporary display widget (or update the Gauge widget you have added) with data.
To make a temporary widget permanent click the plus sign on the widget. Use the settings gear icon to change between Value, Gauge and Line Chart widgets.
*/

#define CAYENNE_PRINT Serial   // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>   // Change this to use a different communication device. See Communications examples.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

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
	// Read data from the sensor and send it to the virtual channel here.
	// You can write data using virtualWrite or other Cayenne write functions.
	// For example, to send a temperature in Celsius you can use the following:
	// Cayenne.virtualWrite(VIRTUAL_CHANNEL, 25.5, TYPE_TEMPERATURE, UNIT_CELSIUS);
}
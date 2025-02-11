/*
This example sketch shows how a value can be sent from the Arduino to the Cayenne Dashboard at automatic intervals.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
2. Compile and upload the sketch.
3. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial     // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h> // Change this to use a different communication device. See Communications examples.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

// Use Virtual Channel 5 for uptime display.
#define VIRTUAL_CHANNEL 5

void setup()
{
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
}

void loop()
{
	Cayenne.loop();
}

// This function is called at intervals to send data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
	CAYENNE_LOG("Send data for Virtual Channel %d", VIRTUAL_CHANNEL);
	// This command writes the device's uptime in seconds to the Virtual Channel.
	Cayenne.virtualWrite(VIRTUAL_CHANNEL, millis() / 1000);
}

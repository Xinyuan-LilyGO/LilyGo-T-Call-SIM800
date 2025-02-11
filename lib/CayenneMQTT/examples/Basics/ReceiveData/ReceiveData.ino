/*
This example sketch shows how a value can be sent from the Cayenne Dashboard to the Arduino on a Virtual Channel.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Create a Generic Digital Output Actuator attached to Virtual Channel 4 in the Cayenne Dashboard.
2. Set the Cayenne authentication info to match the authentication info from the Dashboard.
3. Compile and upload the sketch.
*/

#define CAYENNE_PRINT Serial     // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h> // Change this to use a different communication device. See Communications examples.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

void setup()
{
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
}

void loop()
{
	Cayenne.loop();
}

// This function will be called every time a Dashboard widget writes a value to Virtual Channel 4.
CAYENNE_IN(4)
{
	CAYENNE_LOG("Got a value: %s", getValue.asStr());
	// You can also use:
	// int i = getValue.asInt() or
	// double d = getValue.asDouble()
}



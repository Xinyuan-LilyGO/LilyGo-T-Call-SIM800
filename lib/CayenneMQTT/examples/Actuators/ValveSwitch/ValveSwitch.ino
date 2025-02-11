/*
Cayenne Valve Switch Example

This sketch shows how to set up a Valve Switch with Cayenne. The Arduino cannot
drive a valve because it does not output the needed current. As a result, in order
to make this example work, various electronic components are necessary to connect
the valve. To keep it simple, you will need an external power source, transistor (eg. TIP120),
diode (eg. 1N4001), and a 1k ohm resistor. Alternatively you could also use a relay switch to 
connect the valve.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. In the Cayenne Dashboard add a new Valve Switch widget.
2. Select a virtual channel number for the widget.
3. Set the VIRTUAL_CHANNEL value below to virtual channel you selected.
4. Set up your valve schematic and attach it to a digital pin.
5. Set the ACTUATOR_PIN value below to the digital pin number you selected.
6. Set the Cayenne authentication info to match the authentication info from the Dashboard.
7. Compile and upload this sketch.
8. Once the Arduino connects to the Dashboard you can use the widget button to open and close the valve.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define VIRTUAL_CHANNEL 1
#define ACTUATOR_PIN 4 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.

void setup()
{
	Serial.begin(9600);
	pinMode(ACTUATOR_PIN, OUTPUT);
	Cayenne.begin(username, password, clientID);
}

void loop()
{
	Cayenne.loop();
}

// This function is called when data is sent from Cayenne.
CAYENNE_IN(VIRTUAL_CHANNEL)
{
	int value = getValue.asInt();
	CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL, ACTUATOR_PIN, value);
	// Write the value received to the digital pin.
	digitalWrite(ACTUATOR_PIN, value);
}
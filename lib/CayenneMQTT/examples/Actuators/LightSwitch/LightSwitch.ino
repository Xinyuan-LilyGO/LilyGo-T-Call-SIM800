/*
Cayenne Light Switch Example

This sketch shows how to set up a Light Switch with Cayenne.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. In the Cayenne Dashboard add a new Light Switch Widget.
2. Select a virtual channel number for the widget.
3. Set the VIRTUAL_CHANNEL value below to virtual channel you selected.
4. Attach the negative leg of an LED to ground and the other leg to the selected digital pin.
   Schematic:
   [Ground] -- [LED] -- [Resistor] -- [Digital Pin]
5. Set the ACTUATOR_PIN value below to the pin number you used when connecting your LED.
6. Set the Cayenne authentication info to match the authentication info from the Dashboard.
7. Compile and upload this sketch.
8. Once the Arduino connects to the Dashboard you can use the widget button to turn the LED on and off.
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
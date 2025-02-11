/*
Cayenne Motor Switch Actuator Example

This sketch shows how to set up a Motor Switch with Cayenne. The Arduino cannot
drive a motor because it does not output the needed current. As a result, in order
to make this example work, various electronic components are necessary to drive
the DC motor. To keep it simple, you will need an external power source, transistor (eg. PN2222),
diode (eg. 1N4001), and a 270 ohm resistor.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. In the Cayenne Dashboard add a new Motor Switch widget.
2. Select a virtual channel number for the widget.
3. Set the VIRTUAL_CHANNEL value below to the virtual channel you selected.
4. Set up your motor schematic and attach it to a PWM pin (3, 5, 6, 9, 10, and 11 on most Arduino boards).
5. Set the ACTUATOR_PIN value below to the PWM pin number you selected.
6. Set the Cayenne authentication info to match the authentication info from the Dashboard.
7. Compile and upload this sketch.
8. Once the Arduino connects to the Dashboard you can use the widget button to turn the motor on and off.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define VIRTUAL_CHANNEL 1
#define ACTUATOR_PIN 3

void setup()
{
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
}

void loop()
{
	Cayenne.loop();
}

// Enable or disable the motor based on value received on virtual channel.
CAYENNE_IN(VIRTUAL_CHANNEL)
{
	int speed = 155;
	// Read the integer value which should be 0 or 1.
	int enabled = getValue.asInt();
	if (enabled == 1) {
		// Turn on the motor at the specified speed.
		analogWrite(ACTUATOR_PIN, speed);
	}
	else {
		// Turn off the motor.
		analogWrite(ACTUATOR_PIN, 0);
	}
}
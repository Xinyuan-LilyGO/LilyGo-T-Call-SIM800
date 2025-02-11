/*
Cayenne Servo Motor Example

This sketch sample file shows how to connect a Servo Motor with CayenneMQTT Library.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Install the Servo library from the Arduino Library Manager.
2. In the Cayenne Dashboard add a new Servo Motor widget.
3. Select a virtual channel number for the widget.
4. Set min value to 0 and max value to 1.
5. Set the VIRTUAL_CHANNEL value below to the virtual channel you selected.
6. Connect the Servo's legs to GND, VCC (5.0), and a digital pin.
7. Set the ACTUATOR_PIN value below to the digital pin number you selected.
8. Set the Cayenne authentication info to match the authentication info from the Dashboard.
9. Compile and upload this sketch.
10. Once the Arduino connects to the Dashboard you can use the slider to change the servo position.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>
#include <Servo.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define VIRTUAL_CHANNEL 1
#define ACTUATOR_PIN 4

Servo s1;

void setup()
{
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID);
  s1.attach(ACTUATOR_PIN);
}

void loop()
{
  Cayenne.loop();
}

// This function is called when data is sent from Cayenne.
CAYENNE_IN(VIRTUAL_CHANNEL)
{
	// Determine angle to set the servo.
	int position = getValue.asDouble() * 180;
	// Move the servo to the specified position.
	s1.write(position);
}